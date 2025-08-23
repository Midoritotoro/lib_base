#pragma once 

#include <base/core/type_traits/IteratorCategory.h>

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
template <>
inline constexpr bool can_memcmp_elements_v<std::byte, std::byte, false> = true;
#endif // defined(__cpp_lib_byte)

template <
	class _FirstType_,
	class _SecondType_>
constexpr bool can_memcmp_elements_v<_FirstType_*, _SecondType_*, false> = 
	is_pointer_address_comparable_v<_FirstType_, _SecondType_>;


template <
	class _FirstElement_,
	class _SecondElement_>
constexpr bool can_memcmp_elements_v<_FirstElement_, _SecondElement_, false> = false;

template <
	class _FirstElement_,
	class _SecondElement_,
	class _Function_>
constexpr bool can_memcmp_elements_with_pred_v = false;

template <
	class _FirstElement_,
	class _SecondElement_, 
	class _ThirdElement_>
constexpr bool can_memcmp_elements_with_pred_v<_FirstElement_, _SecondElement_, std::equal_to<_ThirdElement_>> =
	IteratorCopyCategory<_FirstElement_*, _ThirdElement_*>::BitcopyConstructible && 
	IteratorCopyCategory<_SecondElement_*, _ThirdElement_*>::BitcopyConstructible && 
	can_memcmp_elements_v<std::remove_cv_t<_ThirdElement_>, std::remove_cv_t<_ThirdElement_>
>;


template <
	class _FirstElement_, 
	class _SecondElement_>
constexpr bool can_memcmp_elements_with_pred_v<_FirstElement_, _SecondElement_, std::equal_to<>> =
	can_memcmp_elements_v<_FirstElement_, _SecondElement_>;

#if base_has_cxx20
template <
	class _FirstElement_,
	class _SecondElement_>
constexpr bool can_memcmp_elements_with_pred_v<_FirstElement_, _SecondElement_, std::ranges::equal_to> =
	can_memcmp_elements_v<_FirstElement_, _SecondElement_>;
#endif // base_has_cxx20

template <
	class _FirstIterator_,
	class _SecondIterator_,
	class _Function_>
constexpr bool equal_memcmp_is_safe_helper = 
	is_iterator_contiguous_v<_FirstIterator_> && is_iterator_contiguous_v<_SecondIterator_> 
	&& !is_iterator_volatile_v<_FirstIterator_> && !is_iterator_volatile_v<_SecondIterator_>
	&& can_memcmp_elements_with_pred_v<
		IteratorValueType<_FirstIterator_>,
		IteratorValueType<_SecondIterator_>, _Function_>;

template <
	class _FirstIterator_,
	class _SecondIterator_, 
	class _Function_>
constexpr bool equal_memcmp_is_safe_v =
	equal_memcmp_is_safe_helper<
		std::remove_const_t<_FirstIterator_>,
		std::remove_const_t<_SecondIterator_>,
		std::remove_const_t<_Function_>
	>;

__BASE_TYPE_TRAITS_NAMESPACE_END
