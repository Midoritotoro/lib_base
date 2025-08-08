#pragma once

#include <src/core/algorithm/AlgorithmDebug.h>
#include <base/core/utility/Execution.h>

__BASE_NAMESPACE_BEGIN

template <
	class _Iterator1_,
	class _Iterator2_, 
	class _Predicate_>
base_nodiscard base_constexpr_cxx20 std::pair<_Iterator1_, _Iterator2_> mismatch(
	_Iterator1_			firstIterator1,
	const _Iterator1_	lastIterator1,
	_Iterator2_			firstIterator2,
	_Predicate_			predicate)
{

}

template <
	class _ExecutionPolicy_, 
	class _ForwardIterator1_, 
	class _ForwardIterator2_,
	class _Predicate_,
	EnableIfExecutionPolicy<_ExecutionPolicy_> = 0>
base_nodiscard std::pair<_ForwardIterator1_, _ForwardIterator2_> mismatch(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator1_	firstIterator1,
	_ForwardIterator1_	lastIterator1,
	_ForwardIterator2_	firstIterator2,
	_Predicate_			predicate) noexcept
{

}

template <
	class _Iterator1_, 
	class _Iterator2_>
base_nodiscard base_constexpr_cxx20 std::pair<_Iterator1_, _Iterator2_> mismatch(
	const _Iterator1_ firstIterator1,
	const _Iterator1_ lastIterator1,
	const _Iterator2_ firstIterator2)
{

}

template <
	class _ExecutionPolicy_, 
	class _ForwardIterator1_, 
	class _ForwardIterator2_,
	EnableIfExecutionPolicy<_ExecutionPolicy_> = 0>
base_nodiscard std::pair<_ForwardIterator1_, _ForwardIterator2_> mismatch(
	_ExecutionPolicy_&&			executionPolicy,
	const _ForwardIterator1_	firstIterator1,
	const _ForwardIterator1_	lastIterator1,
	const _ForwardIterator2_	firstIterator2) noexcept
{

}

template <
	class _Iterator1_,
	class _Iterator2_,
	class _Predicate_>
base_nodiscard base_constexpr_cxx20 std::pair<_Iterator1_, _Iterator2_> mismatch(
	_Iterator1_ firstIterator1,
	_Iterator1_ lastIterator1,
	_Iterator2_ firstIterator2,
	_Iterator2_ lastIterator2,
	_Predicate_ predicate)
{

}

template <
	class _ExecutionPolicy_,
	class _ForwardIterator1_, 
	class _ForwardIterator2_,
	class _Predicate_,
	EnableIfExecutionPolicy<_ExecutionPolicy_> = 0>
base_nodiscard std::pair<_ForwardIterator1_, _ForwardIterator2_> mismatch(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator1_	firstIterator1,
	_ForwardIterator1_	lastIterator1,
	_ForwardIterator2_	firstIterator2,
	_ForwardIterator2_	lastIterator2,
	_Predicate_			predicate) noexcept
{

}

template <
	class _Iterator1_,
	class _Iterator2_>
base_nodiscard base_constexpr_cxx20 std::pair<_Iterator1_, _Iterator2_> mismatch(
	_Iterator1_ firstIterator1,
	_Iterator1_ lastIterator1,
	_Iterator2_ firstIterator2,
	_Iterator2_ lastIterator2)
{

}

template <
	class _ExecutionPolicy_,
	class _ForwardIterator1_,
	class _ForwardIterator2_,
	EnableIfExecutionPolicy<_ExecutionPolicy_> = 0>
base_nodiscard std::pair<_ForwardIterator1_, _ForwardIterator2_> mismatch(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator1_ firstIterator1,
	_ForwardIterator1_ lastIterator1,
	_ForwardIterator2_ firstIterator2,
	_ForwardIterator2_ lastIterator2) noexcept
{

}

__BASE_NAMESPACE_END
