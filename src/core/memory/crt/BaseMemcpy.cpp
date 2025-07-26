#include <src/core/memory/crt/BaseMemcpy.h>
#include <base/core/arch/ProcessorFeatures.h>

#include <src/core/memory/crt/BaseMemcpySimd.h>


__BASE_MEMORY_NAMESPACE_BEGIN

BASE_ASM_EXTERN DECLARE_NOALIAS NODISCARD void* __CDECL __base_memcpy(
    void*               destination,
    const void* const   source,
    size_t              size) noexcept
{
}

__BASE_MEMORY_NAMESPACE_END
