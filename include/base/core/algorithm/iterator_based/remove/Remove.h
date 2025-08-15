#pragma once

#include <src/core/algorithm/AlgorithmDebug.h>
#include <base/core/utility/Execution.h>

__BASE_NAMESPACE_BEGIN


template <
	class _ForwardIterator_,
	class _Type_>
 _REMOVE_ALGORITHM _ForwardIterator_ remove(
	_ForwardIterator_	firstIterator,
	_ForwardIterator_	lastIterator,
	const _Type_&		value) noexcept
{

}

template <
	class _ForwardIterator_,
	class _Predicate_>
 _REMOVE_ALGORITHM _ForwardIterator_ removeIf(
	_ForwardIterator_	firstIterator,
	_ForwardIterator_	lastIterator,
	_Predicate_			predicate) noexcept
{

}

template <
	class _ExecutionPolicy_,
	class _ForwardIterator_,
	class _Type_,
	EnableIfExecutionPolicy<_ExecutionPolicy_> = 0>
 _REMOVE_ALGORITHM _ForwardIterator_ remove(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator_	firstIterator,
	_ForwardIterator_	lastIterator,
	const _Type_&		value) noexcept
{

}

template <
	class _ExecutionPolicy_,
	class _ForwardIterator_,
	class _Predicate_,
	EnableIfExecutionPolicy<_ExecutionPolicy_> = 0>
 _REMOVE_ALGORITHM _ForwardIterator_ removeIf(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator_	firstIterator,
	_ForwardIterator_	lastIterator,
	_Predicate_			predicate) noexcept
{

}

__BASE_NAMESPACE_END
