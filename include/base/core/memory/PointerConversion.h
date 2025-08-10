#pragma once 

#include <base/core/memory/AddressOf.h>
#include <base/core/memory/ToAddress.h>

__BASE_MEMORY_NAMESPACE_BEGIN

//  онвертирует "модный" указатель в стандартный. 
// ћодными указател€ми считаютс€ типы с переопределенным оператором -> и *
template <class _PointerLikeType_>
base_constexpr_addressof   base_always_inline decltype(auto)
    UnFancy(_PointerLikeType_ pointerLike) noexcept 
{ 
    return AddressOf(*pointerLike);
}

template <class _PointerLikeType_>
base_constexpr_addressof   base_always_inline _PointerLikeType_*
    UnFancy(_PointerLikeType_* pointerLike) noexcept
{
    return pointerLike;
}

//  онвертаци€ из потенциально нулевого модного указател€ в стандартный
template <class _PossiblyNullPointerLikeType_>
base_constexpr_addressof   base_always_inline decltype(auto)
    UnfancyMaybeNull(
        _PossiblyNullPointerLikeType_ possiblyNullPointerLike) noexcept
{
    return possiblyNullPointerLike
        ? AddressOf(*possiblyNullPointerLike)
        : nullptr;
}

template <class _PossiblyNullPointerLikeType_>
base_constexpr_addressof   base_always_inline _PossiblyNullPointerLikeType_*
    UnfancyMaybeNull(
        _PossiblyNullPointerLikeType_* possiblyNullPointerLike) noexcept
{
    return possiblyNullPointerLike;
}

template <typename _Type_>
  base_always_inline char*
    UnCheckedToChar(_Type_ pointer) noexcept
{
    return const_cast<char*>(
        reinterpret_cast<const volatile char*>(pointer));
}

template <typename _Type_> 
  base_always_inline
    const char* UnCheckedToConstChar(_Type_ pointer) noexcept
{
    return const_cast<const char*>(
        reinterpret_cast<const volatile char*>(pointer));
}

template <typename _Type_>
  base_always_inline
    char* CheckedToChar(_Type_ pointerLike) noexcept
{
    const auto pointerLikeAdress = ToAddress(pointerLike);
    return UnCheckedToChar(pointerLikeAdress);
}

template <typename _Type_>
  base_always_inline
    const char* CheckedToConstChar(_Type_ pointerLike) noexcept
{
    const auto pointerLikeAddress = ToAddress(pointerLike);
    return UnCheckedToConstChar(pointerLikeAddress);
}

template <typename _Type_>
  base_always_inline wchar_t*
    UnCheckedToWChar(_Type_ pointer) noexcept
{
    return const_cast<wchar_t*>(
        reinterpret_cast<const volatile wchar_t*>(pointer));
}

template <typename _Type_> 
  base_always_inline
    const wchar_t* UnCheckedToConstWChar(_Type_ pointer) noexcept
{
    return const_cast<const wchar_t*>(
        reinterpret_cast<const volatile wchar_t*>(pointer));
}

template <typename _Type_>
  base_always_inline wchar_t* CheckedToWChar(_Type_ pointerLike) noexcept
{
    const auto pointerLikeAdress = ToAddress(pointerLike);
    return UnCheckedToWChar(pointerLikeAdress);
}

template <typename _Type_>
  base_always_inline
    const wchar_t* CheckedToConstWChar(_Type_ pointerLike) noexcept
{
    const auto pointerLikeAddress = ToAddress(pointerLike);
    return UnCheckedToConstWChar(pointerLikeAddress);
}

template <typename _Type_>
  inline
unsigned char*
    UnCheckedToUnsignedChar(
        _Type_ pointer) noexcept
{
    return const_cast<unsigned char*>(
        reinterpret_cast<
        const volatile unsigned char*>(pointer));
}

template <typename _Type_>
  base_always_inline
const unsigned char* 
    UnCheckedToConstUnsignedChar(
        _Type_ pointer) noexcept
{
    return const_cast<const unsigned char*>(
        reinterpret_cast<
        const volatile unsigned char*>(pointer));
}

template <typename _Type_>
  base_always_inline
    unsigned char* CheckedToUnsignedChar(
        _Type_ pointerLike) noexcept
{
    const auto pointerLikeAddress = ToAddress(pointerLike);
    return UnCheckedToUnsignedChar(pointerLikeAddress);
}

template <typename _Type_>
  base_always_inline
const unsigned char* 
    CheckedToConstUnsignedChar(
        _Type_ pointerLike) noexcept
{
    const auto pointerLikeAddress = ToAddress(pointerLike);
    return UnCheckedToConstUnsignedChar(pointerLikeAddress);
}

__BASE_MEMORY_NAMESPACE_END
