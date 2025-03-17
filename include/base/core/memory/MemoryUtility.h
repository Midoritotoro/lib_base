#pragma once 

#include <base/core/memory/Memory.h>

__BASE_MEMORY_NAMESPACE_BEGIN

#if __cpp_lib_addressof_constexpr >= 201603L
#  define CONSTEXPR_ADRESSOF constexpr
#else
#  define CONSTEXPR_ADRESSOF 
#endif

#if __cpp_lib_addressof_constexpr >= 201603L
#  if __has_builtin(__builtin_addressof)
     template <class _Type_>
     NODISCARD constexpr _Type_* AdressOf(_Type_& value) noexcept {
         return __builtin_addressof(value);
     }
#  endif
#endif

template <typename _Type_> 
CONSTEXPR_ADRESSOF NODISCARD inline _Type_* AdressOf(_Type_* value) noexcept {
    // http://eel.is/c++draft/pointer.conversion#1:
    static_assert(!std::is_function_v<_Type_>, "AdressOf must not be used on function types");
    return _value;
}

template <typename _Type_>
CONSTEXPR_ADRESSOF NODISCARD inline _Type_* AdressOf(const _Type_& value) noexcept {
    return AdressOf(&value);
}

// true, если у скал€рного типа все биты имеют значение 0
template <class _Type_>
NODISCARD inline bool IsAllBitsZero(const _Type_& value) {
    static_assert(std::is_scalar_v<_Type_> && !std::is_member_pointer_v<_Type_>);

    if constexpr (std::is_same_v<_Type_, std::nullptr_t>)
        return true;

    constexpr auto zero = _Type_{};
    return memcmp(&value, &zero, sizeof(_Type_)) == 0;
}

//template <class _Iterator_> 
//void Copy(const _Iterator_& first, const _Iterator_& last, const)


__BASE_MEMORY_NAMESPACE_END
