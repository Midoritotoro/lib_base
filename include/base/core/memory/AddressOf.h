#pragma once 

#include <base/core/type_traits/TypeTraits.h> 

__BASE_MEMORY_NAMESPACE_BEGIN

#if !defined(base_constexpr_addressof)
#  if __cpp_lib_addressof_constexpr >= 201603L
#    define base_constexpr_addressof constexpr
#  else
#    define base_constexpr_addressof 
#  endif // __cpp_lib_addressof_constexpr >= 201603L
#endif // !defined(base_constexpr_addressof)

template <class _Type_>
#if __has_builtin(__builtin_addressof)

  base_constexpr_addressof base_always_inline _Type_* AddressOf(_Type_& value) noexcept {
    return __builtin_addressof(value);
}

#else

  base_constexpr_addressof base_always_inline _Type_* AddressOf(_Type_& value) noexcept {
    return std::addressof(value);
}

#endif

template <class _Type_>
base_constexpr_addressof const _Type_* AddressOf(const _Type_&&) = delete;

__BASE_MEMORY_NAMESPACE_END