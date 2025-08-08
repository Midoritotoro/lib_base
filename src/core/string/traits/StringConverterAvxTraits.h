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
	static base_nodiscard StringConversionResult<_ToChar_> convertString(
		const StringConversionParameters<_FromChar_, _ToChar_>& parameters) noexcept
	{
		AssertUnreachable();
		return {};
	}
private:
};

__BASE_STRING_NAMESPACE_END