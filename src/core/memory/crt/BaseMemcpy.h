#pragma once 

#include <base/core/arch/Platform.h>

__BASE_MEMORY_NAMESPACE_BEGIN

base_declare_const_function base_nodiscard void* __base_memcpy(
    void*               destination,
    const void* const   source,
    size_t              size) noexcept;

__BASE_MEMORY_NAMESPACE_END