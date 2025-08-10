#pragma once 

#include <base/core/utility/TypeTraits.h>


__BASE_CONTAINER_NAMESPACE_BEGIN


struct _Zero_then_variadic_args_ {
    explicit _Zero_then_variadic_args_() = default;
}; // tag type for value-initializing first, constructing second from remaining args

struct _One_then_variadic_args_ {
    explicit _One_then_variadic_args_() = default;
}; // tag type for constructing first from one arg, constructing second from remaining args

template <
    class _Type1_,
    class _Type2_, 
    bool = std::is_empty_v<_Type1_> 
        && !std::is_final_v<_Type1_>>
class CompressedPair final : private _Type1_ {
public:
    _Type2_ _secondValue;

    template <class... _Other2>
    inline constexpr explicit CompressedPair(
        _Zero_then_variadic_args_,
        _Other2&&... secondValue) noexcept(
            std::conjunction_v<
                std::is_nothrow_default_constructible<_Type1_>,
                std::is_nothrow_constructible<
                    _Type2_,
                    _Other2...>>): 
        _Type1_(), 
        _secondValue(std::forward<_Other2>(secondValue)...)
    {}

    template <class _Other1, class... _Other2>
    inline constexpr CompressedPair(
        _One_then_variadic_args_,
        _Other1&& firstValue, 
        _Other2&&... secondValue) noexcept(
            std::conjunction_v<
                std::is_nothrow_constructible<
                    _Type1_,
                    _Other1>,
                std::is_nothrow_constructible<
                    _Type2_,
                    _Other2...>>): 
        _Type1_(std::forward<_Other1>(firstValue)),
        _secondValue(std::forward<_Other2>(secondValue)...)
    {}

    inline constexpr   _Type1_& first() noexcept {
        return *this;
    }

    inline constexpr   const _Type1_& first() const noexcept {
        return *this;
    }

    inline constexpr   _Type2_& second() noexcept {
        return _secondValue;
    }

    inline constexpr   const _Type2_& second() const noexcept {
        return _secondValue;
    }

    inline constexpr   void swap(
        CompressedPair<_Type1_, _Type2_>& other) noexcept
    {
        // Ќе имеет смысла трогать пустое значение типа _Type1_
        std::swap(_secondValue, other._secondValue);
    }
};

template <
    class _Type1_,
    class _Type2_>
class CompressedPair<_Type1_, _Type2_, false> final {
public:
    _Type1_ _firstValue;
    _Type2_ _secondValue;

    template <class... _Other2>
    inline constexpr explicit CompressedPair(
        _Zero_then_variadic_args_,
        _Other2&&... secondValue) noexcept(
            std::conjunction_v<
                std::is_nothrow_default_constructible<_Type1_>,
                std::is_nothrow_constructible<
                    _Type2_, 
                    _Other2...>>): 
        _firstValue(),
        _secondValue(std::forward<_Other2>(secondValue)...)
    {}

    template <class _Other1, class... _Other2>
    inline constexpr CompressedPair(
        _One_then_variadic_args_, 
        _Other1&& firstValue, 
        _Other2&&... secondValue) noexcept(
            std::conjunction_v<
                std::is_nothrow_constructible<
                    _Type1_,
                    _Other1>,
                std::is_nothrow_constructible<
                    _Type2_,
                    _Other2...>>): 
        _firstValue(std::forward<_Other1>(firstValue)),
        _secondValue(std::forward<_Other2>(secondValue)...)
    {}


    inline constexpr   _Type1_& first() noexcept {
        return _firstValue;
    }

    inline constexpr   const _Type1_& first() const noexcept {
        return _firstValue;
    }

    inline constexpr   _Type2_& second() noexcept {
        return _secondValue;
    }

    inline constexpr   const _Type2_& second() const noexcept {
        return _secondValue;
    }

    inline constexpr   void swap(
        CompressedPair<_Type1_, _Type2_, false>& other) noexcept
    {
        std::swap(_firstValue, other._firstValue);
        std::swap(_secondValue, other._secondValue);
    }
};

__BASE_CONTAINER_NAMESPACE_END
