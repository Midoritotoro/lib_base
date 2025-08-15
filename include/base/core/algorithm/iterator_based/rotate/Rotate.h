#pragma once

#include <src/core/algorithm/AlgorithmDebug.h>
#include <base/core/utility/Execution.h>

__BASE_NAMESPACE_BEGIN

template <class _ForwardIterator_>
base_constexpr_cxx20 _ForwardIterator_ rotate(
	_ForwardIterator_ firstIterator,
	_ForwardIterator_ middleIterator,
	_ForwardIterator_ lastIterator)
{

}

template <
	class _ExecutionPolicy_,
	class _ForwardIterator_,
	EnableIfExecutionPolicy<_ExecutionPolicy_> = 0>
_ForwardIterator_ rotate(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator_	firstIterator,
	_ForwardIterator_	middleIterator,
	_ForwardIterator_	lastIterator) noexcept
{

}

__BASE_NAMESPACE_END