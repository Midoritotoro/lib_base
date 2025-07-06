#pragma once 

#include <base/core/utility/TypeTraits.h>

__BASE_NAMESPACE_BEGIN

template <
    typename _Type_,
    typename = void>
struct is_comparable_to_nullptr:
    std::false_type
{};

template <typename _Type_>
struct is_comparable_to_nullptr<
    _Type_,
    std::enable_if_t<
        std::is_convertible<
            decltype(std::declval<_Type_>() != nullptr), bool>::value>
    >:
        std::true_type
{};

// Resolves to the more efficient of `const T` or `const T&`, in the context of returning a const-qualified value of type T.
template <typename _Type_>
using value_or_reference_return_t = std::conditional_t<
    sizeof(_Type_) <= 2 * sizeof(void*) && std::is_trivially_copy_constructible<_Type_>::value,
    const _Type_,
    const _Type_&>;

__BASE_NAMESPACE_END