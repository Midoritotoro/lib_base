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
    _Type2_ _Myval2;

    using _Mybase = _Type1_;

    template <class... _Other2>
    inline constexpr explicit CompressedPair(_Zero_then_variadic_args_, _Other2&&... _Val2) noexcept(
        std::conjunction_v<std::is_nothrow_default_constructible<_Type1_>,
        std::is_nothrow_constructible<_Type2_, _Other2...>>)
        : 
        _Type1_(), 
        _Myval2(std::forward<_Other2>(_Val2)...) {}

    template <class _Other1, class... _Other2>
    inline constexpr CompressedPair(_One_then_variadic_args_, _Other1&& _Val1, _Other2&&... _Val2) noexcept(
        std::conjunction_v<std::is_nothrow_constructible<_Type1_, _Other1>,
        std::is_nothrow_constructible<_Type2_, _Other2...>>)
        : 
        _Type1_(std::forward<_Other1>(_Val1)), 
        _Myval2(std::forward<_Other2>(_Val2)...) {}

    inline constexpr NODISCARD _Type1_& _Get_first() noexcept {
        return *this;
    }

    inline constexpr NODISCARD const _Type1_& _Get_first() const noexcept {
        return *this;
    }
};

template <
    class _Type1_,
    class _Type2_>
class CompressedPair<_Type1_, _Type2_, false> final {
public:
    _Type1_ _Myval1;
    _Type2_ _Myval2;

    template <class... _Other2>
    inline constexpr explicit CompressedPair(_Zero_then_variadic_args_, _Other2&&... _Val2) noexcept(
        std::conjunction_v<std::is_nothrow_default_constructible<_Type1_>,
        std::is_nothrow_constructible<_Type2_, _Other2...>>)
        : 
        _Myval1(),
        _Myval2(std::forward<_Other2>(_Val2)...) {}

    template <class _Other1, class... _Other2>
    inline constexpr CompressedPair(_One_then_variadic_args_, _Other1&& _Val1, _Other2&&... _Val2) noexcept(
        std::conjunction_v<std::is_nothrow_constructible<_Type1_, _Other1>,
        std::is_nothrow_constructible<_Type2_, _Other2...>>)
        : 
        _Myval1(std::forward<_Other1>(_Val1)),
        _Myval2(std::forward<_Other2>(_Val2)...) {}

    inline constexpr NODISCARD _Type1_& _Get_first() noexcept {
        return _Myval1;
    }

    inline constexpr NODISCARD const _Type1_& _Get_first() const noexcept {
        return _Myval1;
    }
};

__BASE_CONTAINER_NAMESPACE_END