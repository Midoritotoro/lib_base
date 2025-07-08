#pragma once 

#include <src/core/string/traits/StringConverterTraits.h>

__BASE_STRING_NAMESPACE_BEGIN

template <>
class StringConverterTraits<CpuFeature::None>
{
public:
	constexpr inline static auto cpuFeature = CpuFeature::None;

	__BASE_DECLARE_CONVERTER_TRAITS_MAYBE_NARROWING_CONVERSION();

	template <
		typename _FromChar_,
		typename _ToChar_>
	static NODISCARD StringConversionResult<_ToChar_> convertString(
		const StringConversionParameters<_FromChar_, _ToChar_>& parameters) noexcept
	{
		AssertUnreachable();
		return {};
	}
};

__BASE_STRING_NAMESPACE_END