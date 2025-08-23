#pragma once 

#include <base/core/BaseNamespace.h>

#include <base/core/type_traits/IteratorCategory.h>
#include <base/core/type_traits/IteratorCheck.h>


__BASE_TYPE_TRAITS_NAMESPACE_BEGIN


template <
	class _FirstElement_,
	class _SecondElement_,
	bool = sizeof(_FirstElement_) == sizeof(_SecondElement_) 
		&& std::is_integral_v<_FirstElement_>
		&& std::is_integral_v<_SecondElement_>>
constexpr bool can_memcmp_elements_v =
		std::is_same_v<_FirstElement_, bool> 
	||	std::is_same_v<_SecondElement_, bool> 
	||	static_cast<_FirstElement_>(-1) == static_cast<_SecondElement_>(-1);

#ifdef __cpp_lib_byte
// Allow memcmping std::byte.
// inline is required here as explicit specializations of variable templates are problematic in C++14.
// However, std::byte is C++17 and above so we are safe.
template <>
inline constexpr bool can_memcmp_elements_v<std::byte, std::byte, false> = true;
#endif // defined(__cpp_lib_byte)

// Pointer elements are eligible for memcmp when they point to the same type, ignoring cv-qualification.
// This handles pointers to object types, pointers to void, and pointers to function types.
template <
	class _FirstType_,
	class _SecondType_>
constexpr bool can_memcmp_elements_v<_FirstType_*, _SecondType_*, false> = 
	is_pointer_address_comparable_v<_FirstType_, _SecondType_>;


template <
	class _FirstElement_,
	class _SecondElement_>
constexpr bool can_memcmp_elements_v<_FirstElement_, _SecondElement_, false> = false;

// _Can_memcmp_elements_with_pred<_FirstElement_, _SecondElement_, _Pr> reports whether the memcmp optimization is applicable,
// given contiguously stored elements. (This avoids having to repeat the metaprogramming that finds the element types.)
// _FirstElement_ and _SecondElement_ aren't top-level const here.
template <
	class _FirstElement_,
	class _SecondElement_,
	class _Function_>
constexpr bool can_memcmp_elements_with_pred_v = false;

// With equal_to<_Elem3> we need to make sure that both _FirstElement_ and _SecondElement_ are convertible to _Elem3 without changing
// object representation (we use _Iter_copy_cat for this task) and _Elem3 can be safely memcmp'ed with itself
template <
	class _FirstElement_,
	class _SecondElement_, 
	class _ThirdElement_>
constexpr bool can_memcmp_elements_with_pred_v<_FirstElement_, _SecondElement_, std::equal_to<_ThirdElement_>> =
	IteratorCopyCategory<_FirstElement_*, _ThirdElement_*>::BitcopyConstructible && 
	IteratorCopyCategory<_SecondElement_*, _ThirdElement_*>::BitcopyConstructible && 
	can_memcmp_elements_v<remove_cv_t<_ThirdElement_>, remove_cv_t<_ThirdElement_>
>;


// equal_to<> is transparent.
template <
	class _FirstElement_, 
	class _SecondElement_>
constexpr bool can_memcmp_elements_with_pred_v<_FirstElement_, _SecondElement_, std::equal_to<>> =
	can_memcmp_elements_v<_FirstElement_, _SecondElement_>;

#if base_has_cxx20
// ranges::equal_to is also transparent.
template <
	class _FirstElement_,
	class _SecondElement_>
constexpr bool can_memcmp_elements_with_pred_v<_FirstElement_, _SecondElement_, std::ranges::equal_to> =
	can_memcmp_elements_v<_FirstElement_, _SecondElement_>;
#endif // base_has_cxx20

// _Equal_memcmp_is_safe<_FirstIterator_, _SecondIterator_, _Pr> reports whether we can activate the memcmp optimization
// for arbitrary iterators and predicates.
// It ignores top-level constness on the iterators and on the elements.
template <
	class _FirstIterator_,
	class _SecondIterator_,
	class _Function_>
constexpr bool _Equal_memcmp_is_safe_helper = 
	is_iterator_contiguous_v<_FirstIterator_> && is_iterator_contiguous_v<_SecondIterator_> 
	&& !is_iterator_volatile_v<_FirstIterator_> && !is_iterator_volatile_v<_SecondIterator_>
	&& can_memcmp_elements_with_pred_v<
		IteratorValueType<_FirstIterator_>,
		IteratorValueType<_SecondIterator_>, _Function_
>;

template <
	class _FirstIterator_,
	class _SecondIterator_, 
	class _Function_>
constexpr bool equal_memcmp_is_safe_v =
	_Equal_memcmp_is_safe_helper<
		std::remove_const_t<_FirstIterator_>,
		std::remove_const_t<_SecondIterator_>,
		std::remove_const_t<_Function_
	>;

__BASE_TYPE_TRAITS_NAMESPACE_END
