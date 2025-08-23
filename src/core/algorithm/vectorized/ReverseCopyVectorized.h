#pragma once

#include <base/core/BaseNamespace.h>

#include <base/core/compatibility/FunctionAttributes.h>
#include <base/core/compatibility/Inline.h>


__BASE_ALGORITHM_NAMESPACE_BEGIN

template <size_t typeSizeInBytes>
base_declare_const_function base_constexpr_cxx20   const void* ReverseCopyVectorized(
    void* firstPointer,
    void* lastPointer,
    void* destinationPointer) noexcept;

__BASE_ALGORITHM_NAMESPACE_END
