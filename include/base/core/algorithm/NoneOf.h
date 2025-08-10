#pragma once

#include <src/core/algorithm/AlgorithmDebug.h>
#include <base/core/utility/Execution.h>

__BASE_NAMESPACE_BEGIN

template <
	class _Iterator_,
	class _Predicate_>
  base_constexpr_cxx20 bool noneOf(
	const _Iterator_	firstIterator,
	const _Iterator_	lastIterator,
	_Predicate_			predicate)
{
	VerifyRange(firstIterator, lastIterator);

	for (; firstIterator != lastIterator; ++firstIterator)
		if (predicate(*firstIterator))
			return false;

	return true;
}

template <
	class _ExecutionPolicy_,
	class _ForwardIterator_,
	class _Predicate_,
	EnableIfExecutionPolicy<_ExecutionPolicy_> = 0>
  bool noneOf(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator_	firstIterator,
	_ForwardIterator_	lastIterator,
	_Predicate_			predicate) noexcept
{

}

__BASE_NAMESPACE_END