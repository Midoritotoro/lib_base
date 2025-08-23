#pragma once

#include <base/core/BaseNamespace.h>
#include <base/core/compatibility/FunctionAttributes.h>

#include <base/core/compatibility/Inline.h>
#include <base/core/compatibility/CallingConventions.h>


__BASE_ALGORITHM_NAMESPACE_BEGIN

base_declare_const_function base_constexpr_cxx20 void base_cdecl SwapRangesVectorized(
    void* firstPointer1,
    void* lastPointer1,
    void* firstPointer2) noexcept;

__BASE_ALGORITHM_NAMESPACE_END
