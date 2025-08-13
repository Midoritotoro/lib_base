#pragma once

#include <src/core/algorithm/AlgorithmDebug.h>
#include <base/core/utility/Execution.h>


__BASE_ALGORITHM_NAMESPACE_BEGIN

template <
	class _Iterator_,
	class _Difference_,
	class _Function_>
base_constexpr_cxx20 _Iterator_ for_each_n(
	_Iterator_			firstIterator,
	const _Difference_	countRaw,
	_Function_			function)
{

}

template <
	class _ExecutionPolicy_,
	class _ForwardIterator_,
	class _Difference_,
	class _Function_,
	EnableIfExecutionPolicy<_ExecutionPolicy_> = 0>
_ForwardIterator_ for_each_n(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator_	firstIterator,
	_Difference_		countRaw,
	_Function_			function) noexcept
{

}

__BASE_ALGORITHM_NAMESPACE_END

