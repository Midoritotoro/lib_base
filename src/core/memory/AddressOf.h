#pragma once 

#include <base/core/utility/TypeTraits.h> 

__BASE_MEMORY_NAMESPACE_BEGIN

#if __cpp_lib_addressof_constexpr >= 201603L
#  define CONSTEXPR_ADDRESSOF constexpr
#else
#  define CONSTEXPR_ADDRESSOF 
#endif

template <class _Type_>
#if __has_builtin(__builtin_addressof)

NODISCARD CONSTEXPR_ADDRESSOF _Type_* AddressOf(_Type_& value) noexcept {
    return __builtin_addressof(value);
}

#else

NODISCARD CONSTEXPR_ADDRESSOF _Type_* AddressOf(_Type_& value) noexcept {
    return std::addressof(value);
}

#endif

template <class _Type_>
const _Type_* AddressOf(const _Type_&&) = delete;

__BASE_MEMORY_NAMESPACE_END