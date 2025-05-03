#pragma once 

#include <src/core/memory/AddressOf.h>
#include <src/core/memory/ToAddress.h>

__BASE_MEMORY_NAMESPACE_BEGIN

// ������������ "������" ��������� � �����������. 
// ������� ����������� ��������� ���� � ���������������� ���������� -> � *
template <class _PointerLikeType_>
CONSTEXPR_ADDRESSOF NODISCARD inline decltype(auto)
    UnFancy(_PointerLikeType_ pointerLike) noexcept 
{ 
    return AddressOf(*pointerLike);
}

template <class _PointerLikeType_>
CONSTEXPR_ADDRESSOF NODISCARD inline _PointerLikeType_*
    UnFancy(_PointerLikeType_* pointerLike) noexcept
{
    return pointerLike;
}

// ����������� �� ������������ �������� ������� ��������� � �����������
template <class _PossiblyNullPointerLikeType_>
CONSTEXPR_ADDRESSOF NODISCARD inline decltype(auto)
    UnfancyMaybeNull(
        _PossiblyNullPointerLikeType_ possiblyNullPointerLike) noexcept
{
    return possiblyNullPointerLike
        ? AddressOf(*possiblyNullPointerLike)
        : nullptr;
}

template <class _PossiblyNullPointerLikeType_>
CONSTEXPR_ADDRESSOF NODISCARD inline _PossiblyNullPointerLikeType_*
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
