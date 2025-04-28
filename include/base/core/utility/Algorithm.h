#pragma once

#include <base/core/arch/Platform.h>

#if defined(OS_WIN) && defined(CPP_MSVC) && !defined(__GNUC__) && !defined(__clang__)
#include <__msvc_iter_core.hpp>
#endif

#if BASE_HAS_CXX20
	#include <ranges>
#endif

#include <base/core/Types.h>
#include <base/core/utility/TypeTraits.h>

__BASE_NAMESPACE_BEGIN



template <
	class _Iterator_,
	class _Function_>
CONSTEXPR_CXX20 _Function_ forEach(
	_Iterator_ firstIterator,
	_Iterator_ lastIterator,
	_Function_ function);

template <
	class _ExecutionPolicy_,
	class _ForwardIterator_,
	class _Function_>
void forEach(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator_	firstIterator,
	_ForwardIterator_	lastIterator,
	_Function_			function) noexcept;

template <
	class _Iterator_,
	class _Difference_,
	class _Function_>
CONSTEXPR_CXX20 _Iterator_ forEachN(
	_Iterator_			firstIterator, 
	const _Difference_	countRaw, 
	_Function_			function);

template <
	class _ExecutionPolicy_, 
	class _ForwardIterator_, 
	class _Difference_, 
	class _Function_>
_ForwardIterator_ forEachN(
	_ExecutionPolicy_&& executionPolicy, 
	_ForwardIterator_	firstIterator, 
	_Difference_		countRaw,
	_Function_			function) noexcept;

template <
	class _ExecutionPolicy_,
	class _ForwardIterator_,
	class _Predicate_>
NODISCARD _ForwardIterator_ findIf(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator_	firstIterator,
	_ForwardIterator_	lastIterator, 
	_Predicate_			predicate) noexcept;

template <
	class _Iterator_,
	class _Predicate_>
NODISCARD CONSTEXPR_CXX20 _Iterator_ findIfNot(
	_Iterator_			firstIterator, 
	const _Iterator_	lastIterator,
	_Predicate_			predicate);

template <
	class _ExecutionPolicy_, 
	class _ForwardIterator_, 
	class _Predicate_>
NODISCARD _ForwardIterator_ findIfNot(
	_ExecutionPolicy_&&	executionPolicy, 
	_ForwardIterator_	firstIterator, 
	_ForwardIterator_	lastIterator,
	_Predicate_			predicate) noexcept;

template <
	class _Iterator_,
	class _Predicate_>
NODISCARD CONSTEXPR_CXX20 IteratorDifferenceType<_Iterator_> countIf(
	_Iterator_	firstIterator, 
	_Iterator_	lastIterator,
	_Predicate_ predicate);

template <
	class _ExecutionPolicy_,
	class _ForwardIterator_,
	class _Predicate_>
NODISCARD IteratorDifferenceType<_ForwardIterator_> countIf(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator_	firstIterator,
	_ForwardIterator_	lastIterator,
	_Predicate_			predicate) noexcept;

template <
	class _Iterator_,
	class _Predicate_>
NODISCARD CONSTEXPR_CXX20 bool allOf(
	_Iterator_	firstIterator,
	_Iterator_	lastIterator,
	_Predicate_ predicate);

template <class _ExecutionPolicy_, class _ForwardIterator_, class _Predicate_>
NODISCARD bool allOf(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator_	firstIterator,
	_ForwardIterator_	lastIterator, 
	_Predicate_			predicate) noexcept;

template <
	class _Iterator_,
	class _Predicate_>
NODISCARD CONSTEXPR_CXX20 bool anyOf(
	const _Iterator_	firstIterator,
	const _Iterator_	lastIterator,
	_Predicate_			predicate);

template <
	class _ExecutionPolicy_,
	class _ForwardIterator_,
	class _Predicate_>
NODISCARD bool anyOf(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator_	firstIterator,
	_ForwardIterator_	lastIterator, 
	_Predicate_			predicate) noexcept;

template <
	class _Iterator_,
	class _Predicate_>
NODISCARD CONSTEXPR_CXX20 bool noneOf(
	const _Iterator_	firstIterator, 
	const _Iterator_	lastIterator, 
	_Predicate_			predicate);

template <
	class _ExecutionPolicy_,
	class _ForwardIterator_,
	class _Predicate_>
NODISCARD bool noneOf(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator_	firstIterator, 
	_ForwardIterator_	lastIterator, 
	_Predicate_			predicate) noexcept;

template <
	class _Iterator_,
	class _OutputIterator_, 
	class _Predicate_>
CONSTEXPR_CXX20 _OutputIterator_ copyIf(
	_Iterator_			firstIterator, 
	_Iterator_			lastIterator, 
	_OutputIterator_	destinationIterator, 
	_Predicate_			predicate);

template <
	class _ExecutionPolicy_, 
	class _ForwardIterator1_,
	class _ForwardIterator2_,
	class _Predicate_>
_ForwardIterator2_ copyIf(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator1_	firstIterator1,
	_ForwardIterator1_	lastIterator1, 
	_ForwardIterator2_	destinationIterator,
	_Predicate_			predicate) noexcept;

template <
	class _ForwardIterator1_, 
	class _ForwardIterator2_,
	class _Predicate_>
NODISCARD CONSTEXPR_CXX20 _ForwardIterator1_ findFirstOf(
	_ForwardIterator1_			firstIterator1,
	const _ForwardIterator1_	lastIterator1,
	const _ForwardIterator2_	firstIterator2, 
	const _ForwardIterator2_	lastIterator2, 
	_Predicate_					predicate);

template <
	class _ForwardIterator1_,
	class _ForwardIterator2_>
NODISCARD CONSTEXPR_CXX20 _ForwardIterator1_ findFirstOf(
	const _ForwardIterator1_ firstIterator1,
	const _ForwardIterator1_ lastIterator1, 
	const _ForwardIterator2_ firstIterator2,
	const _ForwardIterator2_ lastIterator2);

template <
	class _ExecutionPolicy_, 
	class _ForwardIterator1_,
	class _ForwardIterator2_,
	class _Predicate_>
NODISCARD _ForwardIterator1_ findFirstOf(
	_ExecutionPolicy_&&	executionPolicy,
	_ForwardIterator1_	firstIterator1,
	_ForwardIterator1_	lastIterator1,
	_ForwardIterator2_	firstIterator2,
	_ForwardIterator2_	lastIterator2,
	_Predicate_			predicate) noexcept;

template <
	class _ExecutionPolicy_,
	class _ForwardIterator1_,
	class _ForwardIterator2_>
NODISCARD _ForwardIterator1_ findFirstOf(
	_ExecutionPolicy_&&			executionPolicy, 
	const _ForwardIterator1_	firstIterator1, 
	const _ForwardIterator1_	lastIterator1, 
	const _ForwardIterator2_	firstIterator2,
	const _ForwardIterator2_	lastIterator2) noexcept;

template <
	class _ForwardIterator1_,
	class _ForwardIterator2_>
CONSTEXPR_CXX20 _ForwardIterator2_ swapRanges(
	const _ForwardIterator1_	firstIterator1,
	const _ForwardIterator1_	lastIterator1,
	_ForwardIterator2_			firstIterator2);

template <
	class _ExecutionPolicy_,
	class _ForwardIterator1_, 
	class _ForwardIterator2_>
_ForwardIterator2_ swapRanges(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator1_	firstIterator1, 
	_ForwardIterator1_	lastIterator1, 
	_ForwardIterator2_	destinationIterator) noexcept;

template <
	class _Iterator_,
	class _OutputIterator_,
	class _Function_>
CONSTEXPR_CXX20 _OutputIterator_ transform(
	const _Iterator_	firstIterator,
	const _Iterator_	lastIterator, 
	_OutputIterator_	destinationIterator,
	_Function_			function);

template <
	class _ExecutionPolicy_,
	class _ForwardIterator1_,
	class _ForwardIterator2_,
	class _Function_>
_ForwardIterator2_ transform(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator1_	firstIterator1,
	_ForwardIterator1_	lastIterator1,
	_ForwardIterator2_	destinationIterator,
	_Function_			function) noexcept;

template <
	class _Iterator1_,
	class _Iterator2_,
	class _OutputIterator_,
	class _Function_>
CONSTEXPR_CXX20 _OutputIterator_ transform(
	const _Iterator1_	firstIterator1, 
	const _Iterator1_	lastIterator1, 
	const _Iterator2_	firstIterator2, 
	_OutputIterator_	destinationIterator, 
	_Function_			function);

template <
	class _ExecutionPolicy_,
	class _ForwardIterator1_,
	class _ForwardIterator2_, 
	class _ForwardIterator3_,
	class _Function_>
_ForwardIterator3_ transform(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator1_	firstIterator1,
	_ForwardIterator1_	lastIterator1,
	_ForwardIterator2_	firstIterator2,
	_ForwardIterator3_	destinationIterator,
	_Function_			function) noexcept;

template <
	class _ForwardIterator_,
	class _Type_>
CONSTEXPR_CXX20 void replace(
	const _ForwardIterator_ firstIterator,
	const _ForwardIterator_ lastIterator,
	const _Type_&			oldValue, 
	const _Type_&			newValue);

template <
	class _ExecutionPolicy_,
	class _ForwardIterator_,
	class _Type_>
void replace(
	_ExecutionPolicy_&& executionPolicy, 
	_ForwardIterator_	firstIterator,
	_ForwardIterator_	lastIterator,
	const _Type_&		oldValue,
	const _Type_&		newValue) noexcept;

template <
	class _ForwardIterator_, 
	class _Predicate_, 
	class _Type_>
CONSTEXPR_CXX20 void replaceIf(
	const _ForwardIterator_ firstIterator, 
	const _ForwardIterator_ lastIterator, 
	_Predicate_				predicate,
	const _Type_&			value);

template <
	class _ExecutionPolicy_,
	class _ForwardIterator_,
	class _Predicate_,
	class _Type_>
void replaceIf(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator_	firstIterator,
	_ForwardIterator_	lastIterator,
	_Predicate_			predicate, 
	const _Type_&		value) noexcept;

template <
	class _ExecutionPolicy_,
	class _ForwardIterator_,
	class _Type_>
NODISCARD_REMOVE_ALGORITHM _ForwardIterator_ remove(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator_	firstIterator,
	_ForwardIterator_	lastIterator,
	const _Type_&		value) noexcept;

template <
	class _ExecutionPolicy_,
	class _ForwardIterator_,
	class _Predicate_>
NODISCARD_REMOVE_ALGORITHM _ForwardIterator_ removeIf(
	_ExecutionPolicy_&& executionPolicy, 
	_ForwardIterator_	firstIterator,
	_ForwardIterator_	lastIterator, 
	_Predicate_			predicate) noexcept;

template <
	class _BidirectionalIterator_,
	class _OutputIterator_>
CONSTEXPR_CXX20 _OutputIterator_ reverseCopy(
	_BidirectionalIterator_ firstIterator,
	_BidirectionalIterator_ lastIterator,
	_OutputIterator_		destinationIterator);

template <
	class _ExecutionPolicy_,
	class _BidirectionalIterator_, 
	class _ForwardIterator_>
_ForwardIterator_ reverseCopy(
	_ExecutionPolicy_&&		executionPolicy,
	_BidirectionalIterator_ firstIterator,
	_BidirectionalIterator_ lastIterator, 
	_ForwardIterator_		destinationIterator) noexcept;

template <
	class _RandomAccessIterator_, 
	class _Predicate_>
CONSTEXPR_CXX20 void sort(
	const _RandomAccessIterator_	firstIterator,
	const _RandomAccessIterator_	lastIterator,
	_Predicate_						predicate);

template <class _RandomAccessIterator_>
CONSTEXPR_CXX20 void sort(
	const _RandomAccessIterator_ firstIterator, 
	const _RandomAccessIterator_ lastIterator);

template <
	class _ExecutionPolicy_,
	class _RandomAccessIterator_, 
	class _Predicate_>
void sort(
	_ExecutionPolicy_&&		executionPolicy,
	_RandomAccessIterator_	firstIterator,
	_RandomAccessIterator_	lastIterator,
	_Predicate_				predicate) noexcept;

template <
	class _ExecutionPolicy_,
	class _RandomAccessIterator_>
void sort(
	_ExecutionPolicy_&&				executionPolicy,
	const _RandomAccessIterator_	firstIterator,
	const _RandomAccessIterator_	lastIterator) noexcept;

template <
	class _ExecutionPolicy_,
	class _RandomAccessIterator_,
	class _Predicate_>
void partialSort(
	_ExecutionPolicy_&&		executionPolicy,
	_RandomAccessIterator_	firstIterator,
	_RandomAccessIterator_	middleIterator,
	_RandomAccessIterator_	lastIterator, 
	_Predicate_				predicate) noexcept;


template <
	class _ExecutionPolicy_,
	class _RandomAccessIterator_>
void partialSort(
	_ExecutionPolicy_&&		executionPolicy, 
	_RandomAccessIterator_	firstIterator,
	_RandomAccessIterator_	middleIterator,
	_RandomAccessIterator_	lastIterator) noexcept;

template <
	class _ExecutionPolicy_,
	class _ForwardIterator1_,
	class _ForwardIterator2_,
	class _ForwardIterator3_, 
	class _Function_>
_ForwardIterator3_ transform(
	_ExecutionPolicy_&&	executionPolicy, 
	_ForwardIterator1_	firstIterator1, 
	_ForwardIterator1_	lastIterator1,
	_ForwardIterator2_	firstIterator2,
	_ForwardIterator3_	destinationIterator,
	_Function_			function) noexcept;

template <
	class _ForwardIterator_,
	class _Type_>
CONSTEXPR_CXX20 void replace(
	const _ForwardIterator_ firstIterator,
	const _ForwardIterator_ lastIterator, 
	const _Type_&			oldValue,
	const _Type_&			newValue);

template <
	class _ExecutionPolicy_,
	class _ForwardIterator_,
	class _Type_>
void replace(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator_	firstIterator, 
	_ForwardIterator_	lastIterator, 
	const _Type_&		oldValue,
	const _Type_&		newValue) noexcept;

template <
	class _ForwardIterator_, 
	class _Predicate_, 
	class _Type_>
CONSTEXPR_CXX20 void replaceIf(
	const _ForwardIterator_ firstIterator, 
	const _ForwardIterator_ lastIterator,
	_Predicate_				predicate,
	const _Type_&			value);

template <
	class _ExecutionPolicy_,
	class _ForwardIterator_,
	class _Predicate_, 
	class _Type_>
void replaceIf(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator_	firstIterator, 
	_ForwardIterator_	lastIterator,
	_Predicate_			predicate, 
	const _Type_&		value) noexcept;

template <
	class _ExecutionPolicy_, 
	class _ForwardIterator_, 
	class _Type_>
NODISCARD_REMOVE_ALGORITHM _ForwardIterator_ remove(
	_ExecutionPolicy_&& executionPolicy, 
	_ForwardIterator_	firstIterator, 
	_ForwardIterator_	lastIterator, 
	const _Type_&		value) noexcept;

template <
	class _ExecutionPolicy_,
	class _ForwardIterator_,
	class _Predicate_>
NODISCARD_REMOVE_ALGORITHM _ForwardIterator_ removeIf(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator_	firstIterator, 
	_ForwardIterator_	lastIterator, 
	_Predicate_			predicate) noexcept;

template <
	class _BidirectionalIterator_, 
	class _OutputIterator_>
CONSTEXPR_CXX20 _OutputIterator_ reverseCopy(
	_BidirectionalIterator_ firstIterator,
	_BidirectionalIterator_ lastIterator,
	_OutputIterator_		destinationIterator);

template <
	class _ExecutionPolicy_,
	class _BidirectionalIterator_, 
	class _ForwardIterator_>
_ForwardIterator_ reverseCopy(
	_ExecutionPolicy_&&		executionPolicy,
	_BidirectionalIterator_ firstIterator,
	_BidirectionalIterator_ lastIterator, 
	_ForwardIterator_		destinationIterator) noexcept;

template <
	class _RandomAccessIterator_,
	class _Predicate_>
CONSTEXPR_CXX20 void sort(
	const _RandomAccessIterator_	firstIterator,
	const _RandomAccessIterator_	lastIterator,
	_Predicate_						predicate);

template <class _RandomAccessIterator_>
CONSTEXPR_CXX20 void sort(
	const _RandomAccessIterator_ firstIterator,
	const _RandomAccessIterator_ lastIterator);

template <
	class _ExecutionPolicy_,
	class _RandomAccessIterator_, 
	class _Predicate_>
void sort(
	_ExecutionPolicy_&&		executionPolicy,
	_RandomAccessIterator_	firstIterator, 
	_RandomAccessIterator_	lastIterator, 
	_Predicate_				predicate) noexcept;

template <
	class _ExecutionPolicy_,
	class _RandomAccessIterator_>
void sort(
	_ExecutionPolicy_&&				executionPolicy,
	const _RandomAccessIterator_	firstIterator,
	const _RandomAccessIterator_	lastIterator) noexcept;

template <
	class _ExecutionPolicy_,
	class _RandomAccessIterator_,
	class _Predicate_>
void partialSort(
	_ExecutionPolicy_&&		executionPolicy, 
	_RandomAccessIterator_	firstIterator,
	_RandomAccessIterator_	middleIterator,
	_RandomAccessIterator_	lastIterator, 
	_Predicate_				predicate) noexcept;

template <
	class _ExecutionPolicy_,
	class _RandomAccessIterator_>
void partialSort(
	_ExecutionPolicy_&&		executionPolicy,
	_RandomAccessIterator_	firstIterator,
	_RandomAccessIterator_	middleIterator,
	_RandomAccessIterator_	lastIterator) noexcept;

using namespace ::std::ranges;

template<typename I, typename S, typename T,
	typename Op = plus, typename P = identity>
	requires ::std::input_iterator<I>&&
::std::sentinel_for<S, I>&&
	indirectly_binary_invocable<Op, T*, ::std::projected<I, P>>&&
	::std::assignable_from<T&, ::std::indirect_result_t<Op&,
	T*, ::std::projected<I, P>>>
always_inline [[nodiscard]] T accumulate(
	I first,
	S last,
	T init,
	Op op = Op{},
	P proj = P{})
{
	for (; first != last; ++first)
		init = std::invoke(op, init, std::invoke(proj, *first));
	return init;
}

template<typename Rng, typename T, typename Op = plus, typename P = identity>
	requires input_range<Rng>&&
indirectly_binary_invocable<Op, T*,
	::std::projected<iterator_t<Rng>, P>>&&
	::std::assignable_from<T&, ::std::indirect_result_t<Op&, T*,
	::std::projected<iterator_t<Rng>, P>>>

always_inline [[nodiscard]] T accumulate(
	Rng&& rng,
	T init,
	Op op = Op{},
	P proj = P{})
{
	return (*this)(::std::ranges::begin(rng), ::std::ranges::end(rng),
		::std::move(init), ::std::move(op), ::std::move(proj));
}

template <typename T>
always_inline void accumulateMax(T& a, const T& b) {
	if (a < b)
		a = b;
}

template <typename T>
always_inline void accumulateMin(T& a, const T& b) {
	if (a > b)
		a = b;
}

template <typename T>
always_inline [[nodiscard]] T&& take(T& value) {
	return ::std::exchange(value, T{});
}

#if defined(OS_WIN)
	[[nodiscard]] bool IsWindowsGreaterThen(int version);
	[[nodiscard]] bool SetAutoRunKey(
		const ::std::wstring& path,
		const ::std::wstring& key);

	#define MINIMUM_WINDOWS_VERSION		NTDDI_WIN10
	#define IS_MINIMUM_WINDOWS_VERSION	IsWindowsGreaterThen(MINIMUM_WINDOWS_VERSION)
#endif // OS_WIN

__BASE_NAMESPACE_END
