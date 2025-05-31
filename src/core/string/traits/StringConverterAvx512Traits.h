#pragma once 

#include <src/core/string/traits/StringConverterTraits.h>

__BASE_STRING_NAMESPACE_BEGIN

template <class _NarrowingConversionBehaviour_>
class StringConverterTraits<
	CpuFeatureTag<CpuFeature::AVX512>, 
	_NarrowingConversionBehaviour_>:
		public StringConverterTraitsBase<
			CpuFeatureTag<CpuFeature::AVX512>,
			_NarrowingConversionBehaviour_> 
{
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

	using FallbackTraits = StringConverterTraits<CpuFeatureTag<CpuFeature::AVX>, _NarrowingConversionBehaviour_>;
public:
	template <typename _FromChar_>
	// A vector for finding characters of type _FromChar_ that exceed the _ToChar_ limit
	static constexpr NODISCARD base_vec512i_t narrowingLimitVector() noexcept {
		if constexpr (std::is_same_v<_FromChar_, char>)
			return base_constexpr_mm512_set1_epi8(MaximumIntegralLimit<char>());
#if __cpp_lib_char8_t
		else if constexpr (std::is_same_v<_FromChar_, char8_t>)
			return base_constexpr_mm512_set1_epu8(MaximumIntegralLimit<char8_t>());
#endif
		else if constexpr (std::is_same_v<_FromChar_, char16_t>)
			return base_constexpr_mm512_set1_epu16(MaximumIntegralLimit<char16_t>());

		else if constexpr (std::is_same_v<_FromChar_, char32_t>)
			return base_constexpr_mm512_set1_epu32(MaximumIntegralLimit<char32_t>());

		else if constexpr (std::is_same_v<_FromChar_, wchar_t>)
			if constexpr (sizeof(wchar_t) == 2)
				return base_constexpr_mm512_set1_epu16(MaximumIntegralLimit<wchar_t>());
			else if constexpr (sizeof(wchar_t) == 4)
				return base_constexpr_mm512_set1_epu32(MaximumIntegralLimit<wchar_t>());

		AssertUnreachable();
		return {};
	}

	template <typename _ToChar_>
	// A vector for replacing characters in a narrowing conversion
	static constexpr NODISCARD base_vec512i_t replacementVector() noexcept {
		if constexpr (std::is_same_v<_ToChar_, char>)
			return base_constexpr_mm512_set1_epi8(_NarrowingConversionBehaviour_::replacementCharacter);
#if __cpp_lib_char8_t
		else if constexpr (std::is_same_v<_ToChar_, char8_t>)
			return base_constexpr_mm512_set1_epu8(_NarrowingConversionBehaviour_::replacementCharacter);
#endif
		else if constexpr (std::is_same_v<_ToChar_, char16_t>)
			return base_constexpr_mm512_set1_epu16(_NarrowingConversionBehaviour_::replacementCharacter);

		else if constexpr (std::is_same_v<_ToChar_, char32_t>)
			return base_constexpr_mm512_set1_epu32(_NarrowingConversionBehaviour_::replacementCharacter);

		else if constexpr (std::is_same_v<_ToChar_, wchar_t>)
			if constexpr (sizeof(wchar_t) == 2)
				return base_constexpr_mm512_set1_epu16(_NarrowingConversionBehaviour_::replacementCharacter);
			else if constexpr (sizeof(wchar_t) == 4)
				return base_constexpr_mm512_set1_epu32(_NarrowingConversionBehaviour_::replacementCharacter);

		AssertUnreachable();
		return {};
	}

	template <
		typename _FromChar_,
		typename _ToChar_>
	static NODISCARD StringConversionResult<_ToChar_> convertString(
		StringConversionParameters<_FromChar_, _ToChar_, CpuFeatureTag<CpuFeature::AVX512>>& parameters) noexcept
	{
		AssertUnreachable();
		return {};
	}

	template <>
	static NODISCARD StringConversionResult<wchar_t> convertString<char, wchar_t>(
		StringConversionParameters<char, wchar_t, CpuFeatureTag<CpuFeature::AVX512>>& parameters) noexcept
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

		//return FallbackTraits::convertString<char, wchar_t>(
		//	static_cast<const char*>(inputStringVoidPointer),
		//	parameters.stringLength - (avx512SizeInBytes / sizeof(char)),
		//	reinterpret_cast<wchar_t*>(outputStringVoidPointer));
	}

//#if __cpp_lib_char8_t
//	template <>
//	static NODISCARD StringConversionResult<char8_t> convertString<char, char8_t>(
//		const StringConversionParameters<char, wchar_t>& parameters) noexcept
//	{
//		/*baseInitConversionPointers(char, parameters, 64);
//
//		if (avx512SizeInBytes != 0) {
//			do {
//				_mm512_storeu_si512(outputStringVoidPointer, _mm512_loadu_si512(inputStringVoidPointer));
//
//				memory::AdvanceBytes(inputStringVoidPointer, 64);
//				memory::AdvanceBytes(outputStringVoidPointer, 64);
//			} while (inputStringVoidPointer != stopAt);
//		}
//
//		if (inputStringVoidPointer == memory::UnCheckedToConstChar(inputStringDataStart + parameters.stringSizeInBytes))
//			return StringConversionResult<wchar_t>(parameters.outputStringDataStart, parameters, false);
//
//		return FallbackTraits::convertString<char, wchar_t>(
//			static_cast<const char*>(inputStringVoidPointer),
//			parameters.stringLength - (avx512SizeInBytes / sizeof(char)),
//			reinterpret_cast<wchar_t*>(outputStringVoidPointer));*/
//	}
//#endif
//
//	template <>
//	static NODISCARD StringConversionResult<char16_t> convertString<char, char16_t>(
//		const StringConversionParameters<char, wchar_t>& parameters) noexcept
//	{
//
//	}
//
//	template <>
//	static NODISCARD StringConversionResult<char32_t> convertString<char, char32_t>(
//		const StringConversionParameters<char, wchar_t>& parameters) noexcept
//	{
//
//	}
};

__BASE_STRING_NAMESPACE_END