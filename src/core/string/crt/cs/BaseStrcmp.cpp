#include <src/core/string/crt/cs/BaseStrcmp.h>
#include <base/core/arch/ProcessorFeatures.h>

#include <src/core/string/crt/cs/internal/BaseStrcmpInternal.h>


__BASE_STRING_NAMESPACE_BEGIN

DECLARE_NOALIAS int	__CDECL __base_strcmp(
	const char* firstString,
	const char* secondString) noexcept 
{
	//BaseFeatureAwareStrcmp<arch::CpuFeature::ADX>(firstString, secondString);

	if (arch::ProcessorFeatures::AVX512BW())
		return BaseFeatureAwareStrcmp<arch::CpuFeature::AVX512BW>(firstString, secondString);
	else if (arch::ProcessorFeatures::AVX2())
		return BaseFeatureAwareStrcmp<arch::CpuFeature::AVX2>(firstString, secondString);
	else if (arch::ProcessorFeatures::SSE2())
		return BaseFeatureAwareStrcmp<arch::CpuFeature::SSE2>(firstString, secondString);

	return BaseFeatureAwareStrcmp<arch::CpuFeature::None>(firstString, secondString);
}

__BASE_STRING_NAMESPACE_END
