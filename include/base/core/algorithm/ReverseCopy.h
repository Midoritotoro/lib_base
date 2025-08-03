#pragma once

#include <src/core/utility/algorithm/AlgorithmDebug.h>
#include <base/core/utility/Execution.h>

__BASE_NAMESPACE_BEGIN

template <
	class _BidirectionalIterator_,
	class _OutputIterator_>
CONSTEXPR_CXX20 _OutputIterator_ reverseCopy(
	_BidirectionalIterator_ firstIterator,
	_BidirectionalIterator_ lastIterator,
	_OutputIterator_		destinationIterator)
{

}

template <
	class _ExecutionPolicy_,
	class _BidirectionalIterator_,
	class _ForwardIterator_,
	EnableIfExecutionPolicy<_ExecutionPolicy_> = 0>
_ForwardIterator_ reverseCopy(
	_ExecutionPolicy_&& executionPolicy,
	_BidirectionalIterator_ firstIterator,
	_BidirectionalIterator_ lastIterator,
	_ForwardIterator_		destinationIterator) noexcept
{

}

__BASE_NAMESPACE_END
