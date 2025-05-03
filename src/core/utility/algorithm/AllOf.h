#pragma once 

#include <src/core/utility/algorithm/AlgorithmDebug.h>

__BASE_NAMESPACE_BEGIN

template <
	class _Iterator_,
	class _Predicate_>
NODISCARD CONSTEXPR_CXX20 bool allOf(
	_Iterator_	firstIterator,
	_Iterator_	lastIterator,
	_Predicate_ predicate)
{
	VerifyRange(firstIterator, lastIterator);

	auto first		= UnwrapIterator(firstIterator);
	const auto last	= UnwrapIterator(lastIterator);

	for (; first != last; ++first)
		if (predicate(*first) == false)
			return false;

	return true;
}

template <
	class _ExecutionPolicy_, 
	class _ForwardIterator_, 
	class _Predicate_>
NODISCARD bool allOf(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator_	firstIterator,
	_ForwardIterator_	lastIterator,
	_Predicate_			predicate) noexcept
{

}


__BASE_NAMESPACE_END