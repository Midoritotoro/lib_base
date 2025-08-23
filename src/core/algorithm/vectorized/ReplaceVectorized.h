#pragma once

#include <base/core/BaseNamespace.h>

#include <base/core/compatibility/FunctionAttributes.h>
#include <base/core/compatibility/Inline.h>

__BASE_ALGORITHM_NAMESPACE_BEGIN

template <typename _Type_>
base_declare_const_function void base_stdcall ReplaceVectorized(
    void*           firstPointer,
    void*           lastPointer,
    const _Type_&   oldValue,
    const _Type_&   newValue) noexcept;

__BASE_ALGORITHM_NAMESPACE_END
