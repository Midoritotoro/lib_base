#pragma once 

#include <base/core/arch/Platform.h>

__BASE_MEMORY_NAMESPACE_BEGIN

BASE_ASM_EXTERN DECLARE_NOALIAS NODISCARD void* __CDECL __base_memcpy(
    void*       destination,
    void const* source,
    size_t      size) noexcept;

__BASE_MEMORY_NAMESPACE_END