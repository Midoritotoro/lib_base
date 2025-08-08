#pragma once

#include <src/core/algorithm/AlgorithmDebug.h>
#include <base/core/utility/Execution.h>

__BASE_NAMESPACE_BEGIN

template <
	class _ForwardIteratorHaystack_, 
	class _ForwardIteratorPattern_,
	class _Predicate_>
base_nodiscard base_constexpr_cxx20 _ForwardIteratorHaystack_ search(
	_ForwardIteratorHaystack_		firstHaystack,
	_ForwardIteratorHaystack_		lastHaystack,
	const _ForwardIteratorPattern_	firstPattern,
	const _ForwardIteratorPattern_	lastPattern,
	_Predicate_						predicate)
{

}

template <
	class _ExecutionPolicy_,
	class _ForwardIteratorHaystack_,
	class _ForwardIteratorPattern_,
	class _Predicate_,
	EnableIfExecutionPolicy<_ExecutionPolicy_> = 0>
base_nodiscard _ForwardIteratorHaystack_ search(
	_ExecutionPolicy_&&			executionPolicy,
	_ForwardIteratorHaystack_	firstHaystack,
	_ForwardIteratorHaystack_	lastHaystack,
	_ForwardIteratorPattern_	firstPattern,
	_ForwardIteratorPattern_	lastPattern,
	_Predicate_					predicate) noexcept
{

}

template <
	class _ForwardIteratorHaystack_, 
	class _ForwardIteratorPattern_>
base_nodiscard base_constexpr_cxx20 _ForwardIteratorHaystack_ search(
	const _ForwardIteratorHaystack_ firstHaystack,
	const _ForwardIteratorHaystack_ lastHaystack,
	const _ForwardIteratorPattern_	firstPattern,
	const _ForwardIteratorPattern_	lastPattern) 
{

}

template <
	class _ExecutionPolicy_, 
	class _ForwardIteratorHaystack_,
	class _ForwardIteratorPattern_,
	EnableIfExecutionPolicy<_ExecutionPolicy_> = 0>
base_nodiscard _ForwardIteratorHaystack_ search(
	_ExecutionPolicy_&&				executionPolicy,
	const _ForwardIteratorHaystack_ firstHaystack, 
	const _ForwardIteratorHaystack_ lastHaystack,
	const _ForwardIteratorPattern_	firstPattern, 
	const _ForwardIteratorPattern_	lastPattern) noexcept
{

}

template <
	class _ForwardIteratorHaystack_, 
	class _Searcher_>
base_nodiscard base_constexpr_cxx20 _ForwardIteratorHaystack_ search(
	const _ForwardIteratorHaystack_ firstHaystack,
	const _ForwardIteratorHaystack_ lastHaystack,
	const _Searcher_&				searcher)
{

}

template <
	class _ForwardIterator_,
	class _Difference_,
	class _Type_,
	class _Predicate_>
base_nodiscard base_constexpr_cxx20 _ForwardIterator_ searchN(
	const _ForwardIterator_ firstIterator,
	_ForwardIterator_		lastIterator,
	const _Difference_		countRaw,
	const _Type_&			value,
	_Predicate_				predicate)
{

}

template <
	class _ExecutionPolicy_,
	class _ForwardIterator_,
	class _Difference_,
	class _Type_,
	class _Predicate_>
base_nodiscard _ForwardIterator_ searchN(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator_	firstIterator, 
	_ForwardIterator_	lastIterator,
	_Difference_		countRaw,
	const _Type_&		value,
	_Predicate_			predicate) noexcept
{

}

template <
	class _ForwardIterator_,
	class _Difference_, 
	class _Type_>
base_nodiscard base_constexpr_cxx20 _ForwardIterator_ searchN(
	const _ForwardIterator_ firstIterator,
	const _ForwardIterator_ lastIterator,
	const _Difference_		count, 
	const _Type_&			value)
{

}

template <
	class _ExecutionPolicy_,
	class _ForwardIterator_, 
	class _Difference_, 
	class _Type_>
base_nodiscard _ForwardIterator_ searchN(
	_ExecutionPolicy_&&		executionPolicy, 
	const _ForwardIterator_ firstIterator,
	const _ForwardIterator_ lastIterator,
	const _Difference_		count,
	const _Type_&			value) noexcept
{

}

__BASE_NAMESPACE_END
