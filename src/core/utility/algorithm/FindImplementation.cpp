#include <base/core/utility/Algorithm.h>

__BASE_NAMESPACE_BEGIN


template <
	class _Iterator_,
	class _Type_>
NODISCARD CONSTEXPR_CXX20 _Iterator_ find(
	_Iterator_			firstIterator,
	const _Iterator_	lastIterator,
	const _Type_&		value)
{
	 
}

template <
	class _ExecutionPolicy_,
	class _ForwardIterator_,
	class _Type_>
NODISCARD _ForwardIterator_ find(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator_	firstIterator,
	_ForwardIterator_	lastIterator,
	const _Type_&		value) noexcept
{

}

template <
	class _ForwardIterator1_,
	class _ForwardIterator2_,
	class _Predicate_>
NODISCARD CONSTEXPR_CXX20 _ForwardIterator1_ findFirstOf(
	_ForwardIterator1_			firstIterator1,
	const _ForwardIterator1_	lastIterator1,
	const _ForwardIterator2_	firstIterator2,
	const _ForwardIterator2_	lastIterator2,
	_Predicate_					predicate)
{

}

template <
	class _ForwardIterator1_,
	class _ForwardIterator2_>
NODISCARD CONSTEXPR_CXX20 _ForwardIterator1_ findFirstOf(
	const _ForwardIterator1_ firstIterator1,
	const _ForwardIterator1_ lastIterator1,
	const _ForwardIterator2_ firstIterator2,
	const _ForwardIterator2_ lastIterator2)
{

}

template <
	class _ExecutionPolicy_,
	class _ForwardIterator1_,
	class _ForwardIterator2_,
	class _Predicate_>
NODISCARD _ForwardIterator1_ findFirstOf(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator1_	firstIterator1,
	_ForwardIterator1_	lastIterator1,
	_ForwardIterator2_	firstIterator2,
	_ForwardIterator2_	lastIterator2,
	_Predicate_			predicate) noexcept
{

}

template <
	class _ExecutionPolicy_,
	class _ForwardIterator1_,
	class _ForwardIterator2_>
NODISCARD _ForwardIterator1_ findFirstOf(
	_ExecutionPolicy_&&			executionPolicy,
	const _ForwardIterator1_	firstIterator1,
	const _ForwardIterator1_	lastIterator1,
	const _ForwardIterator2_	firstIterator2,
	const _ForwardIterator2_	lastIterator2) noexcept
{

}

template <
	class _ExecutionPolicy_,
	class _ForwardIterator_,
	class _Predicate_>
NODISCARD _ForwardIterator_ findIf(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator_	firstIterator,
	_ForwardIterator_	lastIterator,
	_Predicate_			predicate) noexcept
{

}

template <
	class _Iterator_,
	class _Predicate_>
NODISCARD CONSTEXPR_CXX20 _Iterator_ findIfNot(
	_Iterator_			firstIterator,
	const _Iterator_	lastIterator,
	_Predicate_			predicate)
{

}

template <
	class _ExecutionPolicy_,
	class _ForwardIterator_,
	class _Predicate_>
NODISCARD _ForwardIterator_ findIfNot(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator_	firstIterator,
	_ForwardIterator_	lastIterator,
	_Predicate_			predicate) noexcept
{

}

__BASE_NAMESPACE_END
