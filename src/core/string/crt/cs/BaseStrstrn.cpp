#include <src/core/string/crt/cs/BaseStrstrn.h>
#include <src/core/string/crt/cs/internal/BaseStrstrnInternal.h>

#include <base/core/arch/ProcessorFeatures.h>

__BASE_STRING_NAMESPACE_BEGIN


DECLARE_NOALIAS const char* CDECL __base_strstrn(
    const char* mainString,
    const sizetype	mainLength,
    const char* subString,
    const sizetype	subLength) noexcept
{
    if (arch::ProcessorFeatures::AVX512F())
        return BaseFeatureAwareStrstrn<arch::CpuFeature::AVX512F>(mainString, mainLength, subString, subLength);
    else if (arch::ProcessorFeatures::AVX2())
        return BaseFeatureAwareStrstrn<arch::CpuFeature::AVX2>(mainString, mainLength, subString, subLength);
    else if (arch::ProcessorFeatures::SSE2())
        return BaseFeatureAwareStrstrn<arch::CpuFeature::SSE2>(mainString, mainLength, subString, subLength);

    return BaseFeatureAwareStrstrn<arch::CpuFeature::None>(mainString, mainLength, subString, subLength);
}

__BASE_STRING_NAMESPACE_END
