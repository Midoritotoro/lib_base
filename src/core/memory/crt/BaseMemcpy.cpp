#include <src/core/memory/crt/BaseMemcpy.h>
#include <base/core/arch/ProcessorFeatures.h>

#include <src/core/memory/crt/internal/BaseMemcpyInternal.h>


__BASE_MEMORY_NAMESPACE_BEGIN

base_declare_const_function   void* base_cdecl __base_memcpy(
    void*               destination,
    const void* const   source,
    size_t              size) noexcept
{
    if (arch::ProcessorFeatures::AVX512F())
        return BaseFeatureAwareMemcpy<arch::CpuFeature::AVX512F>(destination, source, size);
    else if (arch::ProcessorFeatures::AVX())
        return BaseFeatureAwareMemcpy<arch::CpuFeature::AVX>(destination, source, size);
    else if (arch::ProcessorFeatures::SSE())
        return BaseFeatureAwareMemcpy<arch::CpuFeature::SSE>(destination, source, size);

    return BaseFeatureAwareMemcpy<arch::CpuFeature::None>(destination, source, size);
}

__BASE_MEMORY_NAMESPACE_END
