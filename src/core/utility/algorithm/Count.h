#pragma once

#include <base/core/async/Concurrent.h>

#include <src/core/utility/algorithm/IteratorWrap.h>
#include <src/core/utility/algorithm/AlgorithmDebug.h>

__BASE_NAMESPACE_BEGIN

template <
	class _Iterator_,
	class _Predicate_>
NODISCARD CONSTEXPR_CXX20 IteratorDifferenceType<_Iterator_> countIf(
	_Iterator_	firstIterator,
	_Iterator_	lastIterator,
	_Predicate_ predicate) noexcept
{
	IteratorDifferenceType<_Iterator_> count = 0;

	for (; firstIterator < lastIterator; ++firstIterator)
		if (predicate(*firstIterator))
			++count;

	return count;
}

template <
	class _ExecutionPolicy_,
	class _ForwardIterator_,
	class _Predicate_>
NODISCARD IteratorDifferenceType<_ForwardIterator_> countIf(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator_	firstIterator,
	_ForwardIterator_	lastIterator,
	_Predicate_			predicate) noexcept
{
	if constexpr (std::remove_reference_t<_ExecutionPolicy_>::Parallelize) {
		
	}

	return countIf(
		std::move(firstIterator),
		std::move(lastIterator), 
		predicate);
}

template <
	class _Iterator_,
	class _Type_>
NODISCARD CONSTEXPR_CXX20 IteratorDifferenceType<_Iterator_> count(
	const _Iterator_	firstIterator,
	const _Iterator_	lastIterator,
	const _Type_&		value) noexcept
{
	IteratorDifferenceType<_Iterator_> count = 0;

	for (; firstIterator < lastIterator; ++firstIterator)
		if (*firstIterator == value)
			++count;

	return count;
}

template <
	class _ExecutionPolicy_,
	class _ForwardIterator_,
	class _Type_>
NODISCARD IteratorDifferenceType<_ForwardIterator_> count(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator_	firstIterator,
	_ForwardIterator_	lastIterator,
	const _Type_&		value) noexcept
{
	if constexpr (std::remove_reference_t<_ExecutionPolicy_>::Parallelize) {

	}

	return count(
		std::move(firstIterator),
		std::move(lastIterator),
		value);
}

__BASE_NAMESPACE_END
