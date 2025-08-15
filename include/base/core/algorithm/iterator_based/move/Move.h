#pragma once

#include <src/core/algorithm/AlgorithmDebug.h>
#include <base/core/utility/Execution.h>

__BASE_NAMESPACE_BEGIN

template <
	class _Iterator_,
	class _OutputIterator_>
base_constexpr_cxx20 _OutputIterator_ move(
	_Iterator_			firstIterator,
	_Iterator_			lastIterator,
	_OutputIterator_	destinationIterator)
{

}

template <
	class _ExecutionPolicy_,
	class _ForwardIterator1_,
	class _ForwardIterator2_,
	EnableIfExecutionPolicy<_ExecutionPolicy_> = 0>
_ForwardIterator2_ move(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator1_	firstIterator,
	_ForwardIterator1_	lastIterator,
	_ForwardIterator2_	destinationIterator) noexcept
{

}

template <
	class _BidirectionalIterator1_,
	class _BidirectionalIterator2_>
base_constexpr_cxx20 _BidirectionalIterator2_ moveBackward(
	_BidirectionalIterator1_ firstIterator,
	_BidirectionalIterator1_ lastIterator,
	_BidirectionalIterator2_ destinationIterator)
{

}

__BASE_NAMESPACE_END
