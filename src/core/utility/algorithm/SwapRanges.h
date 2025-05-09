#pragma once

#include <src/core/utility/algorithm/AlgorithmDebug.h>
#include <base/core/utility/Execution.h>

__BASE_NAMESPACE_BEGIN

template <
	class _ForwardIterator1_,
	class _ForwardIterator2_>
CONSTEXPR_CXX20 _ForwardIterator2_ swapRanges(
	const _ForwardIterator1_	firstIterator1,
	const _ForwardIterator1_	lastIterator1,
	_ForwardIterator2_			firstIterator2)
{

}

template <
	class _ExecutionPolicy_,
	class _ForwardIterator1_,
	class _ForwardIterator2_,
	EnableIfExecutionPolicy<_ExecutionPolicy_> = 0>
_ForwardIterator2_ swapRanges(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator1_	firstIterator1,
	_ForwardIterator1_	lastIterator1,
	_ForwardIterator2_	destinationIterator) noexcept
{

}

__BASE_NAMESPACE_END
