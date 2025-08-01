#pragma once 

#include <src/core/string/traits/StringConverterTraits.h>
#include <src/core/string/traits/StringConverterSseTraits.h>


__BASE_STRING_NAMESPACE_BEGIN

template <>
class StringConverterTraits<arch::CpuFeature::AVX>
{
public:
	constexpr inline auto static cpuFeature = arch::CpuFeature::AVX;
	using FallbackTraits = StringConverterTraits<arch::CpuFeature::SSE>;

	template <
		typename _FromChar_,
		typename _ToChar_>
	static NODISCARD StringConversionResult<_ToChar_> convertString(
		const StringConversionParameters<_FromChar_, _ToChar_>& parameters) noexcept
	{
		AssertUnreachable();
		return {};
	}
private:
	using WCharAvxMaskIntegerType = std::conditional_t<sizeof(wchar_t) == 2,
		uint16,
		std::conditional_t<sizeof(wchar_t) == 4, uint8, void>>;

	template <size_t conversionToLimit>
	static constexpr NODISCARD base_vec512i_t wcharAvxLessThenCompareVector() noexcept {
		if constexpr (sizeof(wchar_t) == 2)
			return base_constexpr_mm256_set1_epu16(
				static_cast<uint16>(conversionToLimit));
		else if constexpr (sizeof(wchar_t) == 4)
			return base_constexpr_mm256_set1_epu32(
				static_cast<uint32>(conversionToLimit));
	}
};

__BASE_STRING_NAMESPACE_END