#include <src/core/memory/crt/BaseMemcpy.h>
#include <base/core/arch/ProcessorFeatures.h>

#include <src/core/memory/crt/BaseMemcpySimd.h>

__BASE_MEMORY_NAMESPACE_BEGIN

BASE_ASM_EXTERN DECLARE_NOALIAS NODISCARD void* __CDECL __base_memcpyScalar(
    void*       destination,
    void const* source,
    size_t      size) noexcept;

BASE_ASM_EXTERN DECLARE_NOALIAS NODISCARD void* __CDECL __base_memcpySse(
    void*       destination,
    void const* source,
    size_t      size) noexcept;

BASE_ASM_EXTERN DECLARE_NOALIAS NODISCARD void* __CDECL __base_memcpyAvx(
    void*       destination,
    void const* source,
    size_t      size) noexcept;

BASE_ASM_EXTERN DECLARE_NOALIAS NODISCARD void* __CDECL __base_memcpyAvx512(
    void*       destination,
    void const* source,
    size_t      size) noexcept;

BASE_ASM_EXTERN DECLARE_NOALIAS NODISCARD void* __CDECL __base_memcpy(
    void*       destination,
    void const* source,
    size_t      size) noexcept
{
    if (ProcessorFeatures::AVX512F())
        return __base_memcpyAvx512(destination, source, size);
    else if (ProcessorFeatures::AVX())
        return __base_memcpyAvx(destination, source, size);
    else if (ProcessorFeatures::SSE())
        return __base_memcpySse(destination, source, size);

    return __base_memcpyScalar(destination, source, size);
}

__BASE_MEMORY_NAMESPACE_END
