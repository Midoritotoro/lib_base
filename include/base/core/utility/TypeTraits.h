#pragma once 

#include <type_traits>
#include <base/core/utility/Concepts.h>

#include <optional>
#include <variant>

#include <base/core/compatibility/Warnings.h>

__BASE_NAMESPACE_BEGIN

using namespace ::std::ranges;

// --------------------------------------------------



template <
    std::size_t A,
    std::size_t B>
struct MinimumSize: 
    std::integral_constant<std::size_t, (A < B) ? A : B> 
{};

template <
    std::size_t A,
    std::size_t B>
struct MaximumSize: 
    std::integral_constant<std::size_t, (A > B) ? A : B> 
{};

template <class _Type_>
struct MaximumObjects: 
    std::integral_constant<std::size_t,
        ~static_cast<std::size_t>(0) / sizeof(_Type_)> 
{};



//template <class _Iterator_>
//constexpr bool isIteratorForwardCxx17 = std::is_convertible_v<memory::IteratorCategory<_Iterator_>, std::forward_iterator_tag>;
//
//template <class _Iter>
//constexpr bool _Is_ranges_fwd_iter_v =
//#if _HAS_CXX20
//    forward_iterator<_Iter> ||
//#endif
//    _Is_cpp17_fwd_iter_v<_Iter>;

__BASE_NAMESPACE_END
