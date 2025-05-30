#pragma once 

#include <base/core/arch/CpuFeatureTag.h>
#include <src/core/string/StringConversionOptions.h> 

#include <src/core/string/traits/StringConverterTraits.h>

__BASE_STRING_NAMESPACE_BEGIN

template <class _NarrowingConversionBehaviour_>
class StringConverterTraits<CpuFeatureTag<CpuFeature::AVX512>, _NarrowingConversionBehaviour_> {
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
public:
	template <
		typename _ToChar_,
		typename = std::enable_if_t<IsCompatibleCharType<_ToChar_>::value>>
	// A vector for replacing characters in a narrowing conversion
	static constexpr NODISCARD base_vec512i_t replacementVector() noexcept {
		if constexpr (std::is_same_v<_ToChar_, char>)
			return base_constexpr_mm512_set1_epi8(MaximumIntegralLimit<char>());
#if __cpp_lib_char8_t
		else if constexpr (std::is_same_v<_ToChar_, char8_t>)
			return base_constexpr_mm512_set1_epu8(MaximumIntegralLimit<char8_t>());
#endif
		else if constexpr (std::is_same_v<_ToChar_, char16_t>)
			return base_constexpr_mm512_set1_epu16(MaximumIntegralLimit<char16_t>());

		else if constexpr (std::is_same_v<_ToChar_, char32_t>)
			return base_constexpr_mm512_set1_epu32(MaximumIntegralLimit<char32_t>());

		else if constexpr (std::is_same_v<_ToChar_, wchar_t>)
			if constexpr (sizeof(wchar_t) == 2)
				return base_constexpr_mm512_set1_epu16(MaximumIntegralLimit<wchar_t>());
			else if constexpr (sizeof(wchar_t) == 4)
				return base_constexpr_mm512_set1_epu32(MaximumIntegralLimit<wchar_t>());

		AssertUnreachable();
		return {};
	}

	template <
		typename _FromChar_,
		typename _ToChar_,
		typename = std::enable_if_t<
			IsCompatibleCharType<_FromChar_>::value&&
			IsCompatibleCharType<_ToChar_>::value>>
	// Is data loss possible when converting from _FromChar_ to _ToChar_
	static constexpr NODISCARD bool maybeNarrowingConversion() noexcept {
		return (MaximumIntegralLimit<_FromChar_>() < MaximumIntegralLimit<_ToChar_>());
	}

	template <
		typename _FromChar_,
		typename _ToChar_,
		typename = std::enable_if_t<
			IsCompatibleCharType<_FromChar_>::value	&&
			IsCompatibleCharType<_ToChar_>::value>>
	static NODISCARD StringConversionResult<_ToChar_> convertString(
		const ConversionParameters<_FromChar_, _ToChar_>& parameters) noexcept
	{
		AssertUnreachable();
		return {};
	}

	template <>
	static NODISCARD StringConversionResult<wchar_t> convertString<char, wchar_t>(
		const ConversionParameters<char, wchar_t>& parameters) noexcept
	{
		
	}

#if __cpp_lib_char8_t
	template <>
	static NODISCARD StringConversionResult<char8_t> convertString<char, char8_t>(
		const ConversionParameters<char, wchar_t>& parameters) noexcept
	{

	}
#endif

	template <>
	static NODISCARD StringConversionResult<char16_t> convertString<char, char16_t>(
		const ConversionParameters<char, wchar_t>& parameters) noexcept
	{

	}

	template <>
	static NODISCARD StringConversionResult<char32_t> convertString<char, char32_t>(
		const ConversionParameters<char, wchar_t>& parameters) noexcept
	{

	}
};

__BASE_STRING_NAMESPACE_END