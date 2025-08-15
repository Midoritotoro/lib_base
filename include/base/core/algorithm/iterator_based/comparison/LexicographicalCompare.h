#pragma once

#include <src/core/algorithm/AlgorithmDebug.h>
#include <base/core/utility/Execution.h>


__BASE_ALGORITHM_NAMESPACE_BEGIN

template <
	class _FirstInputIterator_,
	class _SecondInputIterator_,
	class _Function_>
base_nodiscard base_constexpr_cxx20 bool lexicographical_compare(
	const _FirstInputIterator_	firstIterator1,
	const _FirstInputIterator_	lastIterator1,
	const _SecondInputIterator_ firstIterator2,
	const _SecondInputIterator_ lastIterator2,
	_Function_					function)
{

}

template <
	class _FirstInputIterator_,
	class _SecondInputIterator_>
base_nodiscard base_constexpr_cxx20 bool lexicographical_compare(
	const _FirstInputIterator_	firstIterator1,
	const _FirstInputIterator_	lastIterator1,
	const _SecondInputIterator_ firstIterator2,
	const _SecondInputIterator_ lastIterator2) 
{
	return base::algorithm::lexicographical_compare(
		firstIterator1, lastIterator1,
		firstIterator2, lastIterator2, std::less<>{}
	);
}

template <
	class _ExecutionPolicy_,
	class _FirstForwardIterator_,
	class _SecondForwardIterator_,
	class _Function_,
	EnableIfExecutionPolicy<_ExecutionPolicy_> = 0>
base_nodiscard bool lexicographical_compare(
	_ExecutionPolicy_&&,
	const _FirstForwardIterator_	firstIterator1,
	const _FirstForwardIterator_	lastIterator1,
	const _SecondForwardIterator_	firstIterator2,
	const _SecondForwardIterator_	lastIterator2,
	_Function_						function) noexcept
{
	__base_require_parallel_iterator(_FirstForwardIterator_);
	__base_require_parallel_iterator(_SecondForwardIterator_);

	return base::algorithm::lexicographical_compare(
		firstIterator1, lastIterator1, firstIterator2,
		lastIterator2, base::type_traits::passFunction(function)
	);
}

template <
	class _ExecutionPolicy_,
	class _FirstForwardIterator_,
	class _SecondForwardIterator_,
	EnableIfExecutionPolicy<_ExecutionPolicy_> = 0>
base_nodiscard bool lexicographical_compare(
	_ExecutionPolicy_&&,
	const _FirstForwardIterator_	firstIterator1,
	const _FirstForwardIterator_	lastIterator1,
	const _SecondForwardIterator_	firstIterator2,
	const _SecondForwardIterator_	lastIterator2) noexcept
{
	__base_require_parallel_iterator(_FirstForwardIterator_);
	__base_require_parallel_iterator(_SecondForwardIterator_);

	return base::algorithm::lexicographical_compare(
		firstIterator1, lastIterator1, 
		firstIterator2, lastIterator2
	);
}

__BASE_ALGORITHM_NAMESPACE_END

