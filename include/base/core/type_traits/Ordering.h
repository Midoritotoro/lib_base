#pragma once 

#include <type_traits>
#include <base/core/compatibility/CxxVersionDetection.h>


#if base_has_cxx20

#include <compare>

#include <base/core/BaseNamespace.h>
#include <base/core/compatibility/Nodiscard.h>

__BASE_TYPE_TRAITS_NAMESPACE_BEGIN

template <class _Type_>
concept _Boolean_testable_impl = std::convertible_to<_Type_, bool>;

template <class _Type_>
concept boolean_testable = _Boolean_testable_impl<_Type_> && requires(_Type_ && __t) {
    { !static_cast<_Type_&&>(__t) } -> _Boolean_testable_impl;
};

template <
    class _FirstType_,
    class _SecondType_>
concept can_strong_order = requires(
    _FirstType_ & left,
    _SecondType_ & right)
{
    std::strong_order(left, right);
};

template <
    class _FirstType_,
    class _SecondType_>
concept can_weak_order = requires(
    _FirstType_ & left,
    _SecondType_ & right)
{
    std::weak_order(left, right);
};

template <
    class _FirstType_,
    class _SecondType_>
concept can_partial_order = requires(
    _FirstType_ & left,
    _SecondType_ & right)
{
    std::partial_order(left, right);
};


struct synthetic_three_way {
    template <
        class _FirstType_,
        class _SecondType_>
    base_nodiscard static constexpr auto operator()(
        const _FirstType_& left,
        const _SecondType_& right)
        requires requires {
            { left < right } -> boolean_testable;
            { right < left } -> boolean_testable;
    }
    {
        if constexpr (std::three_way_comparable_with<_FirstType_, _SecondType_>) {
            return left <=> right;
        }
        else {
            if (left < right)
                return std::weak_ordering::less;

            else if (right < left)
                return std::weak_ordering::greater;

            else
                return std::weak_ordering::equivalent;
        }
    }
};

__BASE_TYPE_TRAITS_NAMESPACE_END

#endif // base_has_cxx20
