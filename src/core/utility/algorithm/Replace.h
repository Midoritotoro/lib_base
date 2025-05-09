#pragma once

#include <src/core/utility/algorithm/AlgorithmDebug.h>
#include <base/core/utility/Execution.h>

__BASE_NAMESPACE_BEGIN

template <
	class _ForwardIterator_,
	class _Type_>
CONSTEXPR_CXX20 void replace(
	const _ForwardIterator_ firstIterator,
	const _ForwardIterator_ lastIterator,
	const _Type_&			oldValue,
	const _Type_&			newValue)
{

}

template <
	class _ExecutionPolicy_,
	class _ForwardIterator_,
	class _Type_,
	EnableIfExecutionPolicy<_ExecutionPolicy_> = 0>
void replace(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator_	firstIterator,
	_ForwardIterator_	lastIterator,
	const _Type_&		oldValue,
	const _Type_&		newValue) noexcept
{

}

template <
	class _ForwardIterator_,
	class _Predicate_,
	class _Type_>
CONSTEXPR_CXX20 void replaceIf(
	const _ForwardIterator_ firstIterator,
	const _ForwardIterator_ lastIterator,
	_Predicate_				predicate,
	const _Type_&			value)
{

}

template <
	class _ExecutionPolicy_,
	class _ForwardIterator_,
	class _Predicate_,
	class _Type_,
	EnableIfExecutionPolicy<_ExecutionPolicy_> = 0>
void replaceIf(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator_	firstIterator,
	_ForwardIterator_	lastIterator,
	_Predicate_			predicate,
	const _Type_&		value) noexcept
{

}

__BASE_NAMESPACE_END
