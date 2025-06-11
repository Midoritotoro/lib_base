#pragma once 

#include <src/core/string/traits/StringConverterTraits.h>

__BASE_STRING_NAMESPACE_BEGIN

template <class _NarrowingConversionBehaviour_>
class StringConverterTraits<
	CpuFeature::SSE, 
	_NarrowingConversionBehaviour_>
{
public:
	constexpr inline static auto cpuFeature = CpuFeature::SSE;

	__BASE_DECLARE_CONVERTER_TRAITS_NARROWING_CONVERSION_LIMIT_VECTOR(base_vec128i_t, 128);
	__BASE_DECLARE_CONVERTER_TRAITS_REPLACEMENT_VECTOR(base_vec128i_t, 128);
	__BASE_DECLARE_CONVERTER_TRAITS_MAYBE_NARROWING_CONVERSION();

	template <
		typename _FromChar_,
		typename _ToChar_>
	static NODISCARD StringConversionResult<_ToChar_> convertString(
		StringConversionParameters<_FromChar_, _ToChar_, CpuFeature::SSE>& parameters) noexcept
	{
		AssertUnreachable();
		return {};
	}
private:
	using WCharSseMaskIntegerType = uint8;

	template <size_t conversionToLimit>
	static constexpr NODISCARD base_vec128i_t wcharSseLessThenCompareVector() noexcept {
		if constexpr (sizeof(wchar_t) == 2)
			return base_constexpr_mm_set1_epu16(
				static_cast<uint16>(conversionToLimit));
		else if constexpr (sizeof(wchar_t) == 4)
			return base_constexpr_mm_set1_epu32(
				static_cast<uint32>(conversionToLimit));
	}

	using FallbackTraits = StringConverterTraits<
		CpuFeature::None, _NarrowingConversionBehaviour_>;
};


__BASE_STRING_NAMESPACE_END