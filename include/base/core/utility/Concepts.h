#pragma once 

#include <base/core/arch/Platform.h>

#if BASE_HAS_CXX20

#include <ranges>
#include <concepts>

__BASE_NAMESPACE_BEGIN

template <class _Type_>
concept HasPointerType = requires {
    typename _Type_::pointer;
};

template <class _Type_>
concept HasConstPointerType = requires {
    typename _Type_::const_pointer;
};

template <class _Type_>
concept HasSizeType = requires {
    typename _Type_::size_type;
};

template <class _Type_>
concept HasDifferenceType = requires {
    typename _Type_::difference_type;
};

template <class _Type_>
concept HasValueType = requires {
    typename _Type_::value_type;
};

template <typename _Type_>
concept IsPredicateWithConvertibleToBoolReturn = requires(_Type_ a) {
    { a() } -> std::convertible_to<bool>;
};

__BASE_NAMESPACE_END

#endif 