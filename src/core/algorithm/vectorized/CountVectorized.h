#pragma once

#include <base/core/BaseNamespace.h>

#include <base/core/compatibility/FunctionAttributes.h>
#include <base/core/compatibility/Inline.h>


__BASE_ALGORITHM_NAMESPACE_BEGIN

template <class _Type_>
base_declare_const_function base_constexpr_cxx20 size_t CountVectorized(
    const void*     firstPointer,
    const void*     lastPointer,
    const _Type_&   value) noexcept;

__BASE_ALGORITHM_NAMESPACE_END
