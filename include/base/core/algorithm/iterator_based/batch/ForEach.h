#pragma once

#include <src/core/algorithm/AlgorithmDebug.h>
#include <base/core/utility/Execution.h>


__BASE_ALGORITHM_NAMESPACE_BEGIN


template <
	class _Iterator_,
	class _Function_>
base_constexpr_cxx20 _Function_ for_each(
	_Iterator_ firstIterator,
	_Iterator_ lastIterator,
	_Function_ function)
{

}

template <
	class _ExecutionPolicy_,
	class _ForwardIterator_,
	class _Function_,
	EnableIfExecutionPolicy<_ExecutionPolicy_> = 0>
void for_each(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator_	firstIterator,
	_ForwardIterator_	lastIterator,
	_Function_			function) noexcept
{

}

__BASE_ALGORITHM_NAMESPACE_END

