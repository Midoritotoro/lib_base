#pragma once

#include <src/core/algorithm/AlgorithmDebug.h>
#include <base/core/type_traits/SimdAlgorithmSafety.h>

#include <base/core/utility/Execution.h>


__BASE_ALGORITHM_NAMESPACE_BEGIN

template <
	class _ForwardIterator1_,
	class _ForwardIterator2_,
	class _Predicate_>
base_constexpr_cxx20 _ForwardIterator1_ find_first_of(
	_ForwardIterator1_			firstIterator1,
	const _ForwardIterator1_	lastIterator1,
	const _ForwardIterator2_	firstIterator2,
	const _ForwardIterator2_	lastIterator2,
	_Predicate_					predicate)
{

}

template <
	class _ForwardIterator1_,
	class _ForwardIterator2_>
base_constexpr_cxx20 _ForwardIterator1_ find_first_of(
	const _ForwardIterator1_ firstIterator1,
	const _ForwardIterator1_ lastIterator1,
	const _ForwardIterator2_ firstIterator2,
	const _ForwardIterator2_ lastIterator2)
{

}


template <
	class _ExecutionPolicy_,
	class _ForwardIterator1_,
	class _ForwardIterator2_,
	EnableIfExecutionPolicy<_ExecutionPolicy_> = 0>
_ForwardIterator1_ find_first_of(
	_ExecutionPolicy_&&			executionPolicy,
	const _ForwardIterator1_	firstIterator1,
	const _ForwardIterator1_	lastIterator1,
	const _ForwardIterator2_	firstIterator2,
	const _ForwardIterator2_	lastIterator2) noexcept
{

}

template <
	class _ExecutionPolicy_,
	class _ForwardIterator1_,
	class _ForwardIterator2_,
	class _Predicate_,
	EnableIfExecutionPolicy<_ExecutionPolicy_> = 0>
_ForwardIterator1_ find_first_of(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator1_	firstIterator1,
	_ForwardIterator1_	lastIterator1,
	_ForwardIterator2_	firstIterator2,
	_ForwardIterator2_	lastIterator2,
	_Predicate_			predicate) noexcept
{

}


__BASE_ALGORITHM_NAMESPACE_END

