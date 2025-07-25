#include <src/core/string/crt/cs/BaseStrlen.h>

#include <base/core/arch/ProcessorFeatures.h>
#include <src/core/string/crt/cs/internal/BaseStrlenInternal.h>

__BASE_STRING_NAMESPACE_BEGIN


DECLARE_NOALIAS std::size_t	__CDECL __base_strlen(const char* string) noexcept {
	if (arch::ProcessorFeatures::AVX512BW())
		return BaseFeatureAwareStrlen<arch::CpuFeature::AVX512BW>(string);
	else if (arch::ProcessorFeatures::AVX2())
		return BaseFeatureAwareStrlen<arch::CpuFeature::AVX2>(string);
	else if (arch::ProcessorFeatures::SSE2())
		return BaseFeatureAwareStrlen<arch::CpuFeature::SSE2>(string);

	return BaseFeatureAwareStrlen<arch::CpuFeature::None>(string);
}

__BASE_STRING_NAMESPACE_END
