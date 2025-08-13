#pragma once

#include <src/core/algorithm/AlgorithmDebug.h>
#include <src/core/utility/simd/SimdAlgorithmSafety.h>

#include <base/core/utility/Execution.h>

__BASE_ALGORITHM_NAMESPACE_BEGIN


template <
	class _Iterator_,
	class _Type_>
  base_constexpr_cxx20 _Iterator_ find(
	_Iterator_			firstIterator,
	const _Iterator_	lastIterator,
	const _Type_&		value)
{
	 
}

template <
	class _ExecutionPolicy_,
	class _ForwardIterator_,
	class _Type_,
	EnableIfExecutionPolicy<_ExecutionPolicy_> = 0>
_ForwardIterator_ find(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator_	firstIterator,
	_ForwardIterator_	lastIterator,
	const _Type_&		value) noexcept
{

}

__BASE_ALGORITHM_NAMESPACE_END
