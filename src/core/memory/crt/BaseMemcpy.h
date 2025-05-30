#pragma once 

#include <base/core/arch/Platform.h>

__BASE_MEMORY_NAMESPACE_BEGIN

DECLARE_NOALIAS void* CDECL __base_memcpy(
    void*       destination,
    void const* source,
    size_t      size);

__BASE_MEMORY_NAMESPACE_END