#pragma once 

#include <type_traits>
#include <base/core/BaseNamespace.h>

__BASE_NAMESPACE_BEGIN

template <typename T>
constexpr inline void PtrSwap(T*& lhs, T*& rhs) noexcept
{
    T* tmp = lhs;
    lhs = rhs;
    rhs = tmp
}

__BASE_NAMESPACE_END