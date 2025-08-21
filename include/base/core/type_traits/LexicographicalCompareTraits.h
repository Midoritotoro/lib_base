#pragma once 

#include <base/core/type_traits/IteratorCheck.h>
#include <base/core/type_traits/Ordering.h>


__BASE_TYPE_TRAITS_NAMESPACE_BEGIN

template <
	class _FirstElement_,
	class _SecondElement_>
constexpr bool lexicographical_compare_memcmp_classify_elements_v =
    std::is_integral_v<_FirstElement_> 
		&& std::is_integral_v<_SecondElement_> 
		&& sizeof(_FirstElement_) == sizeof(_SecondElement_)
		&& std::is_unsigned_v<_FirstElement_> == std::is_unsigned_v<_SecondElement_>;

#if defined(__cpp_lib_byte)
	template <>
	inline constexpr bool lexicographical_compare_memcmp_classify_elements_v<byte, byte> = true;
#endif // defined(__cpp_lib_byte)

template <
	class _FirstElement_,
	class _SecondElement_,
	class _Function_>
struct _Lexicographical_compare_memcmp_classify_pred {
    using function_t = void;
};

template <
	class _FirstElement_,
	class _SecondElement_,
	class _ThirdElement_>
struct _Lexicographical_compare_memcmp_classify_pred<
	_FirstElement_,
	_SecondElement_,
	std::less<_ThirdElement_>>
{
    using function_t = std::conditional_t<
		lexicographical_compare_memcmp_classify_elements_v<_ThirdElement_, _ThirdElement_>
			&& IteratorCopyCategory<_FirstElement_*, _ThirdElement_*>::BitcopyConstructible
			&& IteratorCopyCategory<_SecondElement_*, _ThirdElement_*>::BitcopyConstructible,
        std::less<int>, void
	>;
};

template <
	class _FirstElement_,
	class _SecondElement_>
struct _Lexicographical_compare_memcmp_classify_pred<
	_FirstElement_, 
	_SecondElement_, 
	std::less<>> 
{
    using function_t = std::conditional_t<
		lexicographical_compare_memcmp_classify_elements_v<_FirstElement_, _SecondElement_>,
		std::less<int>, void
	>;
};

template <
	class _FirstElement_,
	class _SecondElement_, 
	class _ThirdElement_>
struct _Lexicographical_compare_memcmp_classify_pred<
	_FirstElement_, 
	_SecondElement_, 
	std::greater<_ThirdElement_>> 
{
    using function_t = std::conditional_t<
		lexicographical_compare_memcmp_classify_elements_v<_ThirdElement_, _ThirdElement_>
            && IteratorCopyCategory<_FirstElement_*, _ThirdElement_*>::BitcopyConstructible
            && IteratorCopyCategory<_SecondElement_*, _ThirdElement_*>::BitcopyConstructible,
        std::greater<int>, void
	>;
};

template <
	class _FirstElement_,
	class _SecondElement_>
struct _Lexicographical_compare_memcmp_classify_pred<
	_FirstElement_, 
	_SecondElement_, 
	std::greater<>> 
{
    using function_t = std::conditional_t<
		lexicographical_compare_memcmp_classify_elements_v<_FirstElement_, _SecondElement_>, 
		std::greater<int>, void
	>;
};

#if base_has_cxx20
template <
	class _FirstElement_,
	class _SecondElement_>
struct _Lexicographical_compare_memcmp_classify_pred <
	_FirstElement_,
	_SecondElement_,
	std::ranges::less> 
{
    using function_t = std::conditional_t<
		lexicographical_compare_memcmp_classify_elements_v<_FirstElement_, _SecondElement_>,
		std::less<int>, void
	>;
};

template <
	class _FirstElement_,
	class _SecondElement_>
struct _Lexicographical_compare_memcmp_classify_pred<
	_FirstElement_, 
	_SecondElement_,
	std::ranges::greater> 
{
    using function_t = std::conditional_t<
		lexicographical_compare_memcmp_classify_elements_v<_FirstElement_, _SecondElement_>,
		std::greater<int>, void
	>;
};
#endif // base_has_cxx20

template <
	class _FirstIterator_, 
	class _SecondIterator_, 
	class _Function_>
using _Lexicographical_compare_memcmp_classify =
    std::conditional_t<
		is_iterator_contiguous_v<_FirstIterator_> && is_iterator_contiguous_v<_SecondIterator_>
		&& !is_iterator_volatile_v<_FirstIterator_> && !is_iterator_volatile_v<_SecondIterator_>,
        typename _Lexicographical_compare_memcmp_classify_pred<
			IteratorValueType<_FirstIterator_>,
			IteratorValueType<_SecondIterator_>, _Function_>::function_t, void
		>;


// =================================================================================
//									Three way
// =================================================================================


#if base_has_cxx20

template <
    class _FirstElement_,
    class _SecondElement_,
    class _CompareFunction_>
struct _Lexicographical_compare_three_way_memcmp_classify_comp {
    using compare_t = void;
};

template <
    class _FirstElement_, 
    class _SecondElement_>
struct _Lexicographical_compare_three_way_memcmp_classify_comp<
    _FirstElement_,
    _SecondElement_,
    std::compare_three_way> 
{
    using compare_t = std::conditional_t<
		lexicographical_compare_memcmp_classify_elements_v<_FirstElement_, _SecondElement_>
        && std::three_way_comparable_with<const _FirstElement_&, const _SecondElement_&>,
        std::compare_three_way, void
	>;
};

template <
	class _FirstElement_,
	class _SecondElement_>
struct _Lexicographical_compare_three_way_memcmp_classify_comp<
	_FirstElement_, 
	_SecondElement_,
	synthetic_three_way> 
{
    using compare_t = std::conditional_t<
		lexicographical_compare_memcmp_classify_elements_v<_FirstElement_, _SecondElement_>
        && std::three_way_comparable_with<const _FirstElement_&, const _SecondElement_&>,
		synthetic_three_way, void
	>;
};

template <
	class _FirstElement_,
	class _SecondElement_>
struct _Lexicographical_compare_three_way_memcmp_classify_comp<
	_FirstElement_, 
	_SecondElement_,
	decltype(std::strong_order)> 
{
    using compare_t =
        std::conditional_t<lexicographical_compare_memcmp_classify_elements_v<_FirstElement_, _SecondElement_>
		&& can_strong_order<_FirstElement_, _SecondElement_>, decltype(std::strong_order), void
	>;
};

template <
	class _FirstElement_,
	class _SecondElement_>
struct _Lexicographical_compare_three_way_memcmp_classify_comp<
	_FirstElement_,
	_SecondElement_,
	decltype(std::weak_order)> 
{
    using compare_t =
        std::conditional_t<lexicographical_compare_memcmp_classify_elements_v<_FirstElement_, _SecondElement_> 
		&& can_weak_order<_FirstElement_, _SecondElement_>, decltype(std::weak_order), void
	>;
};

template <
	class _FirstElement_,
	class _SecondElement_>
struct _Lexicographical_compare_three_way_memcmp_classify_comp<
	_FirstElement_,
	_SecondElement_, 
	decltype(std::partial_order)>
{
    using compare_t =
        std::conditional_t<lexicographical_compare_memcmp_classify_elements_v<_FirstElement_, _SecondElement_>
		&& can_partial_order<_FirstElement_, _SecondElement_>, decltype(std::partial_order), void
	>;
};

template <
	class _FirstIterator_,
	class _SecondIterator_, 
	class _CompareFunction_>
using _Lexicographical_compare_three_way_memcmp_classify = std::conditional_t<
    is_iterator_contiguous_v<_FirstIterator_>	&& is_iterator_contiguous_v<_SecondIterator_> && 
	!is_iterator_volatile_v<_FirstIterator_>	&& !is_iterator_volatile_v<_SecondIterator_>,
    typename _Lexicographical_compare_three_way_memcmp_classify_comp<
		IteratorValueType<_FirstIterator_>, IteratorValueType<_SecondIterator_>, _CompareFunction_>::compare_t,
    void
>;

#endif // base_has_cxx20

__BASE_TYPE_TRAITS_NAMESPACE_END
