#pragma once 

#include <base/core/BaseNamespace.h>
#include <type_traits>

#include <xutility>

__BASE_TYPE_TRAITS_NAMESPACE_BEGIN


template <
	class	_Element_, 
	bool	_IsEnum_ = std::is_enum_v<_Element_>>
struct _Unwrap_enum {
	// if _Element_ is an enum, gets its underlying type;
	// otherwise leaves _Element_ unchanged
	using type = std::underlying_type_t<_Element_>;
};

template <class _Element_>
struct _Unwrap_enum<_Element_, false> { 
	// passthrough non-enum type
	using type = _Element_;
};

template <class _Element_>
using unwrap_enum_t = typename _Unwrap_enum<_Element_>::type;

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


__BASE_TYPE_TRAITS_NAMESPACE_END
