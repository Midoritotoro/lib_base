#pragma once 


#include <src/core/algorithm/AlgorithmDebug.h>
#include <src/core/utility/simd/SimdAlgorithmSafety.h>

#include <base/core/utility/Execution.h>


__BASE_ALGORITHM_NAMESPACE_BEGIN

template <
	class _ExecutionPolicy_,
	class _ForwardIterator_,
	class _Predicate_,
	EnableIfExecutionPolicy<_ExecutionPolicy_> = 0>
_ForwardIterator_ find_if(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator_	firstIterator,
	_ForwardIterator_	lastIterator,
	_Predicate_			predicate) noexcept
{

}

__BASE_ALGORITHM_NAMESPACE_END
