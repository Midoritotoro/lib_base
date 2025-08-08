#pragma once 

#include <base/core/utility/TypeTraits.h>

__BASE_MEMORY_NAMESPACE_BEGIN

#if base_has_cxx20

template <class _Type_>
concept HasToAdress = requires(const _Type_ & value) {
    typename std::pointer_traits<_Type_>;
    std::pointer_traits<_Type_>::to_address(value);
};

template <class _Type_>
base_nodiscard constexpr _Type_* ToAddress(_Type_* const value) noexcept {
    static_assert(!std::is_function_v<_Type_>, "N4950 [pointer.conversion]/1: Mandates: T is not a function type.");
    return value;
}

template <class _Pointer_>
base_nodiscard constexpr auto ToAddress(const _Pointer_& value) noexcept {
    if constexpr (HasToAdress<_Pointer_>)
        return std::pointer_traits<_Pointer_>::to_address(value);
    else
        return ToAddress(value.operator->());
}

#endif // base_has_cxx20

__BASE_MEMORY_NAMESPACE_END
