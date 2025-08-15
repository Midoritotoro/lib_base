#pragma once

#include <src/core/algorithm/AlgorithmDebug.h>
#include <base/core/utility/Execution.h>

__BASE_NAMESPACE_BEGIN


template <
	class _RandomAccessIterator_,
	class _Predicate_>
void partialSort(
	_RandomAccessIterator_	firstIterator,
	_RandomAccessIterator_	middleIterator,
	_RandomAccessIterator_	lastIterator,
	_Predicate_				predicate) noexcept
{

}

template <class _RandomAccessIterator_>
void partialSort(
	_RandomAccessIterator_	firstIterator,
	_RandomAccessIterator_	middleIterator,
	_RandomAccessIterator_	lastIterator) noexcept
{

}

template <
	class _ExecutionPolicy_,
	class _RandomAccessIterator_,
	class _Predicate_,
	EnableIfExecutionPolicy<_ExecutionPolicy_> = 0>
void partialSort(
	_ExecutionPolicy_&&		executionPolicy,
	_RandomAccessIterator_	firstIterator,
	_RandomAccessIterator_	middleIterator,
	_RandomAccessIterator_	lastIterator,
	_Predicate_				predicate) noexcept
{

}

template <
	class _ExecutionPolicy_,
	class _RandomAccessIterator_,
	EnableIfExecutionPolicy<_ExecutionPolicy_> = 0>
void partialSort(
	_ExecutionPolicy_&&		executionPolicy,
	_RandomAccessIterator_	firstIterator,
	_RandomAccessIterator_	middleIterator,
	_RandomAccessIterator_	lastIterator) noexcept
{

}

__BASE_NAMESPACE_END
