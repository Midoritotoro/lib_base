#pragma once 

#include <base/core/BaseNamespace.h>
#include <type_traits>

#include <xutility>

__BASE_TYPE_TRAITS_NAMESPACE_BEGIN


#if base_has_cxx20
	template <class _Iterator_>
	using IteratorReferenceType		= std::iter_reference_t<_Iterator_>;

	template <class _Iterator_>
	using IteratorValueType			= std::iter_value_t<_Iterator_>;

	template <class _Iterator_>
	using IteratorDifferenceType	= std::iter_difference_t<_Iterator_>;
#else
	template <class _Iterator_>
	using IteratorReferenceType		= typename std::iterator_traits<_Iterator_>::reference;

	template <class _Iterator_>
	using IteratorValueType			= typename std::iterator_traits<_Iterator_>::value_type;

	template <class _Iterator_>
	using IteratorDifferenceType	= typename std::iterator_traits<_Iterator_>::difference_type;
#endif

template <class _Iterator_>
constexpr inline bool is_iterator_contiguous_v = std::contiguous_iterator<_Iterator_>;

template <class _Iterator_>
constexpr inline bool is_iterator_volatile_v = std::is_volatile_v<std::remove_reference_t<std::iter_reference_t<_Iterator_>>>;

template <
	class _Type_,
	class = void>
constexpr inline bool is_iterator_v = false;

template <class _Type_>
constexpr inline bool is_iterator_v<_Type_, std::void_t<
	typename std::iterator_traits<_Type_>::iterator_category>> = true;


template <class _Iterator_>
constexpr inline bool is_iterator_input_cxx17_v = std::is_convertible_v<
	typename std::iterator_traits<_Iterator_>::iterator_category, std::input_iterator_tag>;

template <class _Iterator_>
constexpr inline bool is_iterator_input_ranges_v =
#if base_has_cxx20
    (std::input_iterator<_Iterator_> && std::sentinel_for<_Iterator_, _Iterator_>) ||
#endif
	is_iterator_input_cxx17_v<_Iterator_>;


template <class _Iterator_>
constexpr inline bool is_iterator_bidirectional_cxx17_v = std::is_convertible_v<
	typename std::iterator_traits<_Iterator_>::iterator_category, std::bidirectional_iterator_tag>;

template <class _Iterator_>
constexpr inline bool is_iterator_bidirectional_ranges_v =
#if base_has_cxx20
    std::bidirectional_iterator<_Iterator_> ||
#endif
	is_iterator_bidirectional_cxx17_v<_Iterator_>;


template <class _Iterator_>
constexpr inline bool is_iterator_random_cxx17_v = std::is_convertible_v<
	typename std::iterator_traits<_Iterator_>::iterator_category, std::random_access_iterator_tag>;

template <class _Iterator_>
constexpr inline bool is_iterator_random_ranges_v =
#if base_has_cxx20
    std::random_access_iterator<_Iterator_> ||
#endif
	is_iterator_random_cxx17_v<_Iterator_>;


template <class _Iterator_>
constexpr inline bool is_iterator_forward_cxx17_v = std::is_convertible_v<
    typename std::iterator_traits<_Iterator_>::iterator_category, std::forward_iterator_tag>;

template <class _Iterator_>
constexpr inline bool is_iterator_forward_ranges_v =
#if base_has_cxx20
    std::forward_iterator<_Iterator_> ||
#endif
	is_iterator_forward_cxx17_v<_Iterator_>;


template <class _Iterator_>
constexpr inline bool is_iterator_parallel_v = is_iterator_forward_ranges_v<_Iterator_>;

#if !defined(__base_require_parallel_iterator)
#define __base_require_parallel_iterator(_Iterator_) \
    static_assert(base::type_traits::is_iterator_parallel_v<_Iterator_>, "Parallel algorithms require forward iterators or stronger.")
#endif // !defined(__base_require_parallel_iterator)


__BASE_TYPE_TRAITS_NAMESPACE_END
