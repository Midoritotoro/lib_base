#pragma once

#include <src/core/utility/algorithm/AlgorithmDebug.h>

__BASE_NAMESPACE_BEGIN


template <
	class _Iterator_,
	class _Function_>
CONSTEXPR_CXX20 _Function_ forEach(
	_Iterator_ firstIterator,
	_Iterator_ lastIterator,
	_Function_ function)
{

}

template <
	class _ExecutionPolicy_,
	class _ForwardIterator_,
	class _Function_>
void forEach(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator_	firstIterator,
	_ForwardIterator_	lastIterator,
	_Function_			function) noexcept
{

}

template <
	class _Iterator_,
	class _Difference_,
	class _Function_>
CONSTEXPR_CXX20 _Iterator_ forEachN(
	_Iterator_			firstIterator,
	const _Difference_	countRaw,
	_Function_			function) 
{

}

template <
	class _ExecutionPolicy_,
	class _ForwardIterator_,
	class _Difference_,
	class _Function_>
_ForwardIterator_ forEachN(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator_	firstIterator,
	_Difference_		countRaw,
	_Function_			function) noexcept
{

}

__BASE_NAMESPACE_END
