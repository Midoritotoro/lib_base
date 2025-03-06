#pragma once 

#include <type_traits>

namespace base {
    template <typename T>
    constexpr inline void PtrSwap(T*& lhs, T*& rhs) noexcept
    {
        T* tmp = lhs;
        lhs = rhs;
        rhs = tmp;
    }
} // namespace base