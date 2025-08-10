#pragma once 

#include <src/core/string/traits/StringConverterTraits.h>

__BASE_STRING_NAMESPACE_BEGIN

template <>
class StringConverterTraits<arch::CpuFeature::None>
{
public:
	constexpr inline static auto cpuFeature = arch::CpuFeature::None;

	template <
		typename _FromChar_,
		typename _ToChar_>
	static   StringConversionResult<_ToChar_> convertString(
		const StringConversionParameters<_FromChar_, _ToChar_>& parameters) noexcept
	{
		AssertUnreachable();
		return {};
	}
};

__BASE_STRING_NAMESPACE_END