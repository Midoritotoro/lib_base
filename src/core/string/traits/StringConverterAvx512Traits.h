#pragma once 

#include <src/core/string/traits/StringConverterTraits.h>

__BASE_STRING_NAMESPACE_BEGIN

template <class _NarrowingConversionBehaviour_>
class StringConverterTraits<
	CpuFeature::AVX512, 
	_NarrowingConversionBehaviour_>
{
public:
	constexpr inline auto static cpuFeature = CpuFeature::AVX512;

	__BASE_DECLARE_CONVERTER_TRAITS_NARROWING_CONVERSION_LIMIT_VECTOR(base_vec512i_t, 512);
	__BASE_DECLARE_CONVERTER_TRAITS_REPLACEMENT_VECTOR(base_vec512i_t, 512);
	__BASE_DECLARE_CONVERTER_TRAITS_MAYBE_NARROWING_CONVERSION();

	template <
		typename _FromChar_,
		typename _ToChar_>
	static NODISCARD StringConversionResult<_ToChar_> convertString(
		StringConversionParameters<_FromChar_, _ToChar_, CpuFeature::AVX512>& parameters) noexcept
	{
		AssertUnreachable();
		return {};
	}

	template <>
	static NODISCARD StringConversionResult<wchar_t> convertString<char, wchar_t>(
		StringConversionParameters<char, wchar_t, CpuFeature::AVX512>& parameters) noexcept
	{
		baseInitConversionPointers(char, parameters, 64)

		if (avx512SizeInBytes != 0) {
			do {
				if constexpr (sizeof(wchar_t) == 4) {
					const char* inputStringDataStartChar = memory::UnCheckedToConstChar(inputStringVoidPointer);
					char* outputStringCharPointer = memory::UnCheckedToChar(outputStringVoidPointer);

					_mm512_store_si512(outputStringVoidPointer, _mm512_cvtepi8_epi32(
						_mm_loadu_si128(reinterpret_cast<const __m128i*>(inputStringDataStartChar))));

					_mm512_store_si512(outputStringCharPointer + 64, _mm512_cvtepi8_epi32(
						_mm_loadu_si128(reinterpret_cast<const __m128i*>(inputStringDataStartChar + 16))));

					_mm512_store_si512(outputStringCharPointer + 128, _mm512_cvtepi8_epi32(
						_mm_loadu_si128(reinterpret_cast<const __m128i*>(inputStringDataStartChar + 32))));

					_mm512_store_si512(outputStringCharPointer + 192, _mm512_cvtepi8_epi32(
						_mm_loadu_si128(reinterpret_cast<const __m128i*>(inputStringDataStartChar + 48))));

					memory::AdvanceBytes(outputStringVoidPointer, 256);
				}
				else if constexpr (sizeof(wchar_t) == 2) {
					_mm512_store_si512(outputStringVoidPointer, _mm512_cvtepi8_epi16(
						_mm256_loadu_si256(reinterpret_cast<const __m256i*>(inputStringVoidPointer))));

					_mm512_store_si512(memory::UnCheckedToChar(outputStringVoidPointer) + 64, _mm512_cvtepi8_epi16(
						_mm256_loadu_si256(reinterpret_cast<const __m256i*>(
							memory::UnCheckedToConstChar(inputStringVoidPointer) + 32))));

					memory::AdvanceBytes(outputStringVoidPointer, 128);
				}

				memory::AdvanceBytes(inputStringVoidPointer, 64);
			} while (inputStringVoidPointer != stopAt);
		}

		if (inputStringVoidPointer == memory::UnCheckedToConstChar(parameters.inputStringDataStart + parameters.stringLength))
			return StringConversionResult<wchar_t>(parameters.outputStringDataStart, parameters.stringLength, false);

		return FallbackTraits::convertString<char, wchar_t>(
			static_cast<const char*>(inputStringVoidPointer),
			parameters.stringLength - (avx512SizeInBytes / sizeof(char)),
			reinterpret_cast<wchar_t*>(outputStringVoidPointer));
	}

	template <>
	static NODISCARD StringConversionResult<char> convertStringImplementation<wchar_t, char, CpuFeatureTag<CpuFeature::AVX512F>>(
		const wchar_t* const	string,
		const size_t            stringLength,
		char*					outputString)
	{
		baseInitConversionPointers(wchar_t, parameters, 64)

		if (avx512SizeInBytes != 0) {
			const auto runtimeReplacementVector = base_vec512i_t_pointer_as_m512i(&replacementVector);
			const auto runtimeLessThanCompare = base_vec512i_t_pointer_as_m512i(&lessThanCompare);

			do {
				const auto loaded1 = _mm512_loadu_si512(stringDataStart);
				const auto loaded2 = _mm512_loadu_si512(memory::UnCheckedToConstChar(stringDataStart) + 64);

				uint64 comparedGreaterThan = 0;

				__m512i convertedToCharVector;

				if constexpr (sizeof(wchar_t) == 2) {
					uint32 first64BytesMask = _mm512_cmpgt_epu16_mask(loaded1, runtimeLessThanCompare);
					uint32 second64BytesMask = _mm512_cmpgt_epu16_mask(loaded2, runtimeLessThanCompare);

					comparedGreaterThan = base_combine_u32ints_to_u64int(first64BytesMask, second64BytesMask);

					convertedToCharVector = _mm512_castsi256_si512(_mm512_cvtepi16_epi8(loaded1));
					convertedToCharVector = _mm512_inserti32x8(convertedToCharVector, _mm512_cvtepi16_epi8(loaded2), 1);
				}
				else if constexpr (sizeof(wchar_t) == 4) {
					const auto loaded3 = _mm512_loadu_si512(memory::UnCheckedToConstChar(stringDataStart) + 128);
					const auto loaded4 = _mm512_loadu_si512(memory::UnCheckedToConstChar(stringDataStart) + 192);

					uint16 first64BytesMask = _mm512_cmpgt_epu32_mask(loaded1, runtimeLessThanCompare);
					uint16 second64BytesMask = _mm512_cmpgt_epu32_mask(loaded2, runtimeLessThanCompare);

					uint16 third64BytesMask = _mm512_cmpgt_epu32_mask(loaded1, runtimeLessThanCompare);
					uint16 fourth64BytesMask = _mm512_cmpgt_epu32_mask(loaded2, runtimeLessThanCompare);

					comparedGreaterThan = base_combine_u16ints_to_u64int(
						first64BytesMask, second64BytesMask, third64BytesMask, fourth64BytesMask);

					convertedToCharVector = _mm512_castsi128_si512(_mm512_cvtepi32_epi8(loaded1));

					convertedToCharVector = _mm512_inserti32x4(convertedToCharVector, _mm512_cvtepi32_epi8(loaded2), 1);
					convertedToCharVector = _mm512_inserti32x4(convertedToCharVector, _mm512_cvtepi32_epi8(loaded3), 2);
					convertedToCharVector = _mm512_inserti32x4(convertedToCharVector, _mm512_cvtepi32_epi8(loaded4), 3);
				}

				// Narrowing conversion
				if (comparedGreaterThan != 0) {
					isNarrowingConversion = true;

					switch (_NarrowingConversionBehaviour_::narrowingConversionMode) {
					case NarrowingConversionMode::CustomReplacement:
					case NarrowingConversionMode::DefaultReplacement:
						// If a character in 'convertedToCharVector' is greater than the threshold (mask bit is 1), it's replaced with
						// 'replacementCharacter'. Otherwise, it's copied from 'convertedToCharVector'.

						_mm512_mask_storeu_epi8(outputStringVoidPointer, ~comparedGreaterThan, convertedToCharVector);
						_mm512_mask_storeu_epi8(outputStringVoidPointer, comparedGreaterThan, runtimeReplacementVector);

						break;
					default:
						AssertUnreachable();
						break;
					};
				}
				else
					_mm512_store_si512(outputStringVoidPointer, convertedToCharVector);

				if constexpr (sizeof(wchar_t) == 2)
					memory::AdvanceBytes(stringDataStart, 128);
				else if constexpr (sizeof(wchar_t) == 4)
					memory::AdvanceBytes(stringDataStart, 256);

				memory::AdvanceBytes(outputStringVoidPointer, 64);
			} while (stringDataStart != stopAt);
		}

		if (stringDataStart == stringDataEnd)
			return StringConversionResult<char>(
				outputString - fromSizeInBytes,
				stringLength, isNarrowingConversion);
			return StringConversionResult<char>(outputString, stringLength, isNarrowingConversion);

		return convertStringImplementation<char16_t, char, CpuFeatureTag<CpuFeature::AVX2>>(
			static_cast<const char16_t*>(stringDataStart), stringLength - (avx512SizeInBytes / sizeof(char16_t)),
			outputString, CpuFeatureTag<CpuFeature::AVX2>{});
	}


#if __cpp_lib_char8_t
	template <>
	static NODISCARD StringConversionResult<char8_t> convertString<char, char8_t>(
		const StringConversionParameters<char, wchar_t>& parameters) noexcept
	{
		baseInitConversionPointers(char, parameters, 64);

		if (avx512SizeInBytes != 0) {
			do {
				_mm512_storeu_si512(outputStringVoidPointer, _mm512_loadu_si512(inputStringVoidPointer));

				memory::AdvanceBytes(inputStringVoidPointer, 64);
				memory::AdvanceBytes(outputStringVoidPointer, 64);
			} while (inputStringVoidPointer != stopAt);
		}

		if (inputStringVoidPointer == memory::UnCheckedToConstChar(inputStringDataStart + parameters.stringSizeInBytes))
			return StringConversionResult<wchar_t>(parameters.outputStringDataStart, parameters, false);

		return FallbackTraits::convertString<char, wchar_t>(
			static_cast<const char*>(inputStringVoidPointer),
			parameters.stringLength - (avx512SizeInBytes / sizeof(char)),
			reinterpret_cast<wchar_t*>(outputStringVoidPointer));
	}
#endif

	template <>
	static NODISCARD StringConversionResult<char16_t> convertString<char, char16_t>(
		const StringConversionParameters<char, wchar_t>& parameters) noexcept
	{

	}

	template <>
	static NODISCARD StringConversionResult<char32_t> convertString<char, char32_t>(
		const StringConversionParameters<char, wchar_t>& parameters) noexcept
	{

	}

private:
	using WCharAvx512MaskIntegerType = std::conditional_t<sizeof(wchar_t) == 2,
		uint32,
		std::conditional_t<sizeof(wchar_t) == 4, uint16, void>>;

	template <size_t conversionToLimit>
	static constexpr NODISCARD base_vec512i_t wcharAvx512LessThenCompareVector() noexcept {
		if constexpr (sizeof(wchar_t) == 2)
			return base_constexpr_mm512_set1_epu16(
				static_cast<uint16>(conversionToLimit));
		else if constexpr (sizeof(wchar_t) == 4)
			return base_constexpr_mm512_set1_epu32(
				static_cast<uint32>(conversionToLimit));
	}

	using FallbackTraits = StringConverterTraits<
		CpuFeature::AVX, _NarrowingConversionBehaviour_>;
};

__BASE_STRING_NAMESPACE_END