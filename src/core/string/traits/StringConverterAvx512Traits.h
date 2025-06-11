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