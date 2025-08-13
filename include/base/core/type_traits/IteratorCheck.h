#pragma once 

#include <base/core/BaseNamespace.h>
#include <type_traits>


__BASE_TYPE_TRAITS_NAMESPACE_BEGIN

template <class _Iterator_>
constexpr bool is_iterator_contiguous_v = std::contiguous_iterator<_Iterator_>;


template <class _Iterator_>
constexpr bool is_iterator_volatile_v = std::is_volatile_v<
	std::remove_reference_t<std::iter_reference_t<_Iterator_>>>;

__BASE_TYPE_TRAITS_NAMESPACE_END
