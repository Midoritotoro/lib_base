#pragma once

#include <src/core/utility/algorithm/AlgorithmDebug.h>

__BASE_NAMESPACE_BEGIN

template <
	class _ExecutionPolicy_,
	class _RandomAccessIterator_,
	class _Predicate_>
void partialSort(
	_ExecutionPolicy_&& executionPolicy,
	_RandomAccessIterator_	firstIterator,
	_RandomAccessIterator_	middleIterator,
	_RandomAccessIterator_	lastIterator,
	_Predicate_				predicate) noexcept
{

}

template <
	class _ExecutionPolicy_,
	class _RandomAccessIterator_>
void partialSort(
	_ExecutionPolicy_&& executionPolicy,
	_RandomAccessIterator_	firstIterator,
	_RandomAccessIterator_	middleIterator,
	_RandomAccessIterator_	lastIterator) noexcept
{

}

__BASE_NAMESPACE_END
