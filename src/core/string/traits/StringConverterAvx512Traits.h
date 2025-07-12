#pragma once 

#include <src/core/string/traits/StringConverterTraits.h>

#include <src/core/string/traits/StringConverterAvxTraits.h>

__BASE_STRING_NAMESPACE_BEGIN

template <>
class StringConverterTraits<CpuFeature::AVX512>
{
public:
	constexpr inline auto static cpuFeature = CpuFeature::AVX512;
	using FallbackTraits = StringConverterTraits<CpuFeature::AVX>;

	template <
		typename _FromChar_,
		typename _ToChar_>
	static NODISCARD StringConversionResult<_ToChar_> convertString(
		StringConversionParameters<_FromChar_, _ToChar_> parameters) noexcept
	{
		AssertUnreachable();
		return {};
	}

	template <>
	static NODISCARD StringConversionResult<wchar_t> convertString<char, wchar_t>(
		 StringConversionParameters<char, wchar_t> parameters) noexcept
	{
		const size_t alignedBytes = (parameters.stringLength * sizeof(char)) & ~size_t(0x3F);

		const void* stopAt = parameters.originalInputStringDataStart;
		memory::AdvanceBytes(stopAt, alignedBytes);

		if (alignedBytes != 0) {
			do {
				if constexpr (sizeof(wchar_t) == 4) {
					const char* inputStringDataStartChar = memory::UnCheckedToConstChar(parameters.inputStringDataStart);
					char* outputStringCharPointer = memory::UnCheckedToChar(parameters.outputStringDataStart);
					
					_mm512_store_si512(parameters.outputStringDataStart, _mm512_cvtepi8_epi32(
						_mm_loadu_si128(reinterpret_cast<const __m128i*>(inputStringDataStartChar))));

					_mm512_store_si512(outputStringCharPointer + 64, _mm512_cvtepi8_epi32(
						_mm_loadu_si128(reinterpret_cast<const __m128i*>(inputStringDataStartChar + 16))));

					_mm512_store_si512(outputStringCharPointer + 128, _mm512_cvtepi8_epi32(
						_mm_loadu_si128(reinterpret_cast<const __m128i*>(inputStringDataStartChar + 32))));

					_mm512_store_si512(outputStringCharPointer + 192, _mm512_cvtepi8_epi32(
						_mm_loadu_si128(reinterpret_cast<const __m128i*>(inputStringDataStartChar + 48))));

					memory::AdvanceBytes(parameters.outputStringDataStart, 256);
				}
				else if constexpr (sizeof(wchar_t) == 2) {
					_mm512_store_si512(parameters.outputStringDataStart, _mm512_cvtepi8_epi16(
						_mm256_loadu_si256(reinterpret_cast<const __m256i*>(parameters.inputStringDataStart))));

					_mm512_store_si512(memory::UnCheckedToChar(parameters.outputStringDataStart) + 64, _mm512_cvtepi8_epi16(
						_mm256_loadu_si256(reinterpret_cast<const __m256i*>(
							memory::UnCheckedToConstChar(parameters.inputStringDataStart) + 32))));

					memory::AdvanceBytes(parameters.outputStringDataStart, 128);
				}

				memory::AdvanceBytes(parameters.inputStringDataStart, 64);
			} while (parameters.inputStringDataStart != stopAt);
		}

		if (parameters.inputStringDataStart == base::memory::UnCheckedToConstChar(parameters.originalInputStringDataStart + parameters.stringLength))
			return StringConversionResult<wchar_t>(static_cast<wchar_t*>(parameters.outputStringDataStart), parameters.stringLength, false);

		return FallbackTraits::convertString<char, wchar_t>(parameters);
	}

//	template <>
//	static NODISCARD StringConversionResult<char> convertString<wchar_t, char>(
//		StringConversionParameters<wchar_t, char, CpuFeature::AVX512>& parameters)
//	{
//		__baseInitConversionParameters(wchar_t, 64);
//		bool isNarrowingConversion = false;
//
//		if (alignedBytes != 0) {
//			do {
//				const auto loaded1 = _mm512_loadu_si512(inputStringVoidPointer);
//				const auto loaded2 = _mm512_loadu_si512(memory::UnCheckedToConstChar(inputStringVoidPointer) + 64);
//
//				uint64 comparedGreaterThan = 0;
//				__m512i convertedToCharVector;
//
//				if constexpr (sizeof(wchar_t) == 2) {
//					uint32 first64BytesMask = _mm512_cmpgt_epu16_mask(loaded1, parameters.narrowingLimitVector);
//					uint32 second64BytesMask = _mm512_cmpgt_epu16_mask(loaded2, parameters.narrowingLimitVector);
//
//					comparedGreaterThan = base_combine_u32ints_to_u64int(first64BytesMask, second64BytesMask);
//
//					convertedToCharVector = _mm512_castsi256_si512(_mm512_cvtepi16_epi8(loaded1));
//					convertedToCharVector = _mm512_inserti32x8(convertedToCharVector, _mm512_cvtepi16_epi8(loaded2), 1);
//				}
//				else if constexpr (sizeof(wchar_t) == 4) {
//					const auto loaded3 = _mm512_loadu_si512(memory::UnCheckedToConstChar(inputStringVoidPointer) + 128);
//					const auto loaded4 = _mm512_loadu_si512(memory::UnCheckedToConstChar(inputStringVoidPointer) + 192);
//
//					uint16 first64BytesMask = _mm512_cmpgt_epu32_mask(loaded1, parameters.narrowingLimitVector);
//					uint16 second64BytesMask = _mm512_cmpgt_epu32_mask(loaded2, parameters.narrowingLimitVector);
//
//					uint16 third64BytesMask = _mm512_cmpgt_epu32_mask(loaded1, parameters.narrowingLimitVector);
//					uint16 fourth64BytesMask = _mm512_cmpgt_epu32_mask(loaded2, parameters.narrowingLimitVector);
//
//					comparedGreaterThan = base_combine_u16ints_to_u64int(
//						first64BytesMask, second64BytesMask, third64BytesMask, fourth64BytesMask);
//
//					convertedToCharVector = _mm512_castsi128_si512(_mm512_cvtepi32_epi8(loaded1));
//
//					convertedToCharVector = _mm512_inserti32x4(convertedToCharVector, _mm512_cvtepi32_epi8(loaded2), 1);
//					convertedToCharVector = _mm512_inserti32x4(convertedToCharVector, _mm512_cvtepi32_epi8(loaded3), 2);
//					convertedToCharVector = _mm512_inserti32x4(convertedToCharVector, _mm512_cvtepi32_epi8(loaded4), 3);
//				}
//
//				// Narrowing conversion
//				if (comparedGreaterThan != 0) {
//					isNarrowingConversion = true;
//
//					switch (_NarrowingConversionBehaviour_::narrowingConversionMode) {
//					case NarrowingConversionMode::CustomReplacement:
//					case NarrowingConversionMode::DefaultReplacement:
//						// If a character in 'convertedToCharVector' is greater than the threshold (mask bit is 1), it's replaced with
//						// 'replacementCharacter'. Otherwise, it's copied from 'convertedToCharVector'.
//
//						// _mm512_mask_storeu_epi8(outputStringVoidPointer, ~comparedGreaterThan, convertedToCharVector);
//						// _mm512_mask_storeu_epi8(outputStringVoidPointer, comparedGreaterThan, parameters.replacementVector);
// 
//						convertedToCharVector = _mm512_mask_blend_epi64(comparedGreaterThan, convertedToCharVector, parameters.replacementVector)
//						_mm512_storeu_si512(outputStringVoidPointer, convertedToCharVector);
// 
//						break;
//					default:
//						AssertUnreachable();
//						break;
//					};
//				}
//				else
//					_mm512_store_si512(outputStringVoidPointer, convertedToCharVector);
//
//				if constexpr (sizeof(wchar_t) == 2)
//					memory::AdvanceBytes(inputStringVoidPointer, 128);
//				else if constexpr (sizeof(wchar_t) == 4)
//					memory::AdvanceBytes(inputStringVoidPointer, 256);
//
//				memory::AdvanceBytes(outputStringVoidPointer, 64);
//			} while (inputStringVoidPointer != stopAt);
//		}
//
//		__baseConvertOrFallback(wchar_t, char, isNarrowingConversion);
//	}
//
//
//#if __cpp_lib_char8_t
//	template <>
//	static NODISCARD StringConversionResult<char8_t> convertString<char, char8_t>(
//		const StringConversionParameters<char, char8_t, CpuFeature::AVX512>& parameters) noexcept
//	{
//		__baseInitConversionParameters(char, 64);
//
//		if (alignedBytes != 0) {
//			do {
//				_mm512_storeu_si512(outputStringVoidPointer, _mm512_loadu_si512(inputStringVoidPointer));
//
//				memory::AdvanceBytes(inputStringVoidPointer, 64);
//				memory::AdvanceBytes(outputStringVoidPointer, 64);
//			} while (inputStringVoidPointer != stopAt);
//		}
//
//		__baseConvertOrFallback(char, char8_t, false)
//	}
//#endif
//
//	template <>
//	static NODISCARD StringConversionResult<char16_t> convertString<char, char16_t>(
//		const StringConversionParameters<char, wchar_t, CpuFeature::AVX512>& parameters) noexcept
//	{
//
//	}
//
//	template <>
//	static NODISCARD StringConversionResult<char32_t> convertString<char, char32_t>(
//		const StringConversionParameters<char, wchar_t, CpuFeature::AVX512>& parameters) noexcept
//	{
//
//	}

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
};

__BASE_STRING_NAMESPACE_END