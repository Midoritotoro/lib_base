#pragma once 

#include <base/core/memory/MemoryTypeTraits.h>

__BASE_MEMORY_NAMESPACE_BEGIN

#if __cpp_lib_addressof_constexpr >= 201603L
#  define CONSTEXPR_ADDRESSOF constexpr
#else
#  define CONSTEXPR_ADDRESSOF 
#endif

#if __has_builtin(__builtin_addressof)
     template <class _Type_>
     NODISCARD CONSTEXPR_ADDRESSOF _Type_* AddressOf(_Type_& value) noexcept {
         return __builtin_addressof(value);
     }
#else
    template <class _Type_>
    NODISCARD CONSTEXPR_ADDRESSOF _Type_* AddressOf(_Type_& value) noexcept {
         return std::addressof(value);
    }
#endif

template <class _Type_>
const _Type_* AddressOf(const _Type_&&) = delete;

#if BASE_HAS_CXX20

template <class _Type_>
concept HasToAdress = requires(const _Type_ & _Val) {
    typename std::pointer_traits<_Type_>;
    std::pointer_traits<_Type_>::to_address(_Val);
};

template <class _Type_>
NODISCARD constexpr _Type_* ToAddress(_Type_* const _Val) noexcept {
    static_assert(!std::is_function_v<_Type_>, "N4950 [pointer.conversion]/1: Mandates: T is not a function type.");
    return _Val;
}

template <class _Ptr>
NODISCARD constexpr auto ToAddress(const _Ptr& _Val) noexcept {
    if constexpr (HasToAdress<_Ptr>)
        return std::pointer_traits<_Ptr>::to_address(_Val);
    else
        return ToAddress(_Val.operator->());
}

#endif

// true, если у скал€рного типа все биты имеют значение 0
template <class _Type_>
CONSTEXPR_CXX20 NODISCARD inline bool IsAllBitsZero(const _Type_& value) {
    static_assert(std::is_scalar_v<_Type_> && !std::is_member_pointer_v<_Type_>);

    if constexpr (std::is_same_v<_Type_, std::nullptr_t>)
        return true;

    constexpr auto zero = _Type_{};
    return memcmp(&value, &zero, sizeof(_Type_)) == 0;
}

//  онвертирует "модный" указатель в стандартный. 
// ћодными указател€ми считаютс€ типы с переопределенным оператором -> и *
template <class _PointerLikeType_>
NODISCARD inline constexpr decltype(auto)
    UnFancy(_PointerLikeType_ pointerLike) noexcept 
{ 
    return AddressOf(*pointerLike);
}

template <class _PointerLikeType_>
NODISCARD inline constexpr _PointerLikeType_* 
    UnFancy(_PointerLikeType_* pointerLike) noexcept
{
    return pointerLike;
}

//  онвертаци€ из потенциально нулевого модного указател€ в стандартный
template <class _PossiblyNullPointerLikeType_>
NODISCARD inline constexpr decltype(auto) 
    UnfancyMaybeNull(
        _PossiblyNullPointerLikeType_ possiblyNullPointerLike) noexcept
{
    return possiblyNullPointerLike
        ? AddressOf(*possiblyNullPointerLike)
        : nullptr;
}

template <class _PossiblyNullPointerLikeType_>
NODISCARD inline constexpr _PossiblyNullPointerLikeType_* 
    UnfancyMaybeNull(
        _PossiblyNullPointerLikeType_* possiblyNullPointerLike) noexcept
{
    return possiblyNullPointerLike;
}

template <typename _Type_>
NODISCARD inline constexpr char* 
    UnCheckedToChar(_Type_ pointer) noexcept
{
    return const_cast<char*>(
        reinterpret_cast<const volatile char*>(pointer));
}

template <typename _Type_> 
NODISCARD inline constexpr 
    const char* UnCheckedToConstChar(_Type_ pointer) noexcept
{
    return const_cast<const char*>(
        reinterpret_cast<const volatile char*>(pointer));
}

template <typename _Type_>
NODISCARD inline constexpr
    char* CheckedToChar(_Type_ pointerLike) noexcept
{
    const auto pointerLikeAdress = ToAddress(pointerLike);
    return UnCheckedToChar(pointerLikeAdress);
}

template <typename _Type_>
NODISCARD inline constexpr
    const char* CheckedToConstChar(_Type_ pointerLike) noexcept
{
    const auto pointerLikeAddress = ToAddress(pointerLike);
    return UnCheckedToConstChar(pointerLikeAddress);
}

template <typename _Type_>
NODISCARD inline constexpr
unsigned char*
    UnCheckedToUnsignedChar(
        _Type_ pointer) noexcept
{
    return const_cast<unsigned char*>(
        reinterpret_cast<
        const volatile unsigned char*>(pointer));
}

template <typename _Type_>
NODISCARD inline constexpr
const unsigned char* 
    UnCheckedToConstUnsignedChar(
        _Type_ pointer) noexcept
{
    return const_cast<const unsigned char*>(
        reinterpret_cast<
        const volatile unsigned char*>(pointer));
}

template <typename _Type_>
NODISCARD inline constexpr
    unsigned char* CheckedToUnsignedChar(
        _Type_ pointerLike) noexcept
{
    const auto pointerLikeAddress = ToAddress(pointerLike);
    return UnCheckedToUnsignedChar(pointerLikeAddress);
}

template <typename _Type_>
NODISCARD inline constexpr
const unsigned char* 
    CheckedToConstUnsignedChar(
        _Type_ pointerLike) noexcept
{
    const auto pointerLikeAddress = ToAddress(pointerLike);
    return UnCheckedToConstUnsignedChar(pointerLikeAddress);
}

__BASE_MEMORY_NAMESPACE_END