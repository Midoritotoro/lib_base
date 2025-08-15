#pragma once

#include <src/core/algorithm/AlgorithmDebug.h>
#include <base/core/utility/Execution.h>

__BASE_NAMESPACE_BEGIN

template <
	class _RandomAccessIterator_,
	class _Predicate_>
base_constexpr_cxx20 void sort(
	const _RandomAccessIterator_	firstIterator,
	const _RandomAccessIterator_	lastIterator,
	_Predicate_						predicate)
{

}

template <class _RandomAccessIterator_>
base_constexpr_cxx20 void sort(
	const _RandomAccessIterator_ firstIterator,
	const _RandomAccessIterator_ lastIterator)
{

}

template <
	class _ExecutionPolicy_,
	class _RandomAccessIterator_,
	class _Predicate_,
	EnableIfExecutionPolicy<_ExecutionPolicy_> = 0>
void sort(
	_ExecutionPolicy_&&		executionPolicy,
	_RandomAccessIterator_	firstIterator,
	_RandomAccessIterator_	lastIterator,
	_Predicate_				predicate) noexcept
{

}

template <
	class _ExecutionPolicy_,
	class _RandomAccessIterator_,
	EnableIfExecutionPolicy<_ExecutionPolicy_> = 0>
void sort(
	_ExecutionPolicy_&&				executionPolicy,
	const _RandomAccessIterator_	firstIterator,
	const _RandomAccessIterator_	lastIterator) noexcept
{

}


__BASE_NAMESPACE_END
