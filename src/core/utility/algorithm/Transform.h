#include <base/core/utility/Algorithm.h>

__BASE_NAMESPACE_BEGIN

template <
	class _Iterator_,
	class _OutputIterator_,
	class _Function_>
CONSTEXPR_CXX20 _OutputIterator_ transform(
	const _Iterator_	firstIterator,
	const _Iterator_	lastIterator,
	_OutputIterator_	destinationIterator,
	_Function_			function)
{

}

template <
	class _ExecutionPolicy_,
	class _ForwardIterator1_,
	class _ForwardIterator2_,
	class _Function_>
_ForwardIterator2_ transform(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator1_	firstIterator1,
	_ForwardIterator1_	lastIterator1,
	_ForwardIterator2_	destinationIterator,
	_Function_			function) noexcept
{

}

template <
	class _Iterator1_,
	class _Iterator2_,
	class _OutputIterator_,
	class _Function_>
CONSTEXPR_CXX20 _OutputIterator_ transform(
	const _Iterator1_	firstIterator1,
	const _Iterator1_	lastIterator1,
	const _Iterator2_	firstIterator2,
	_OutputIterator_	destinationIterator,
	_Function_			function)
{

}

template <
	class _ExecutionPolicy_,
	class _ForwardIterator1_,
	class _ForwardIterator2_,
	class _ForwardIterator3_,
	class _Function_>
_ForwardIterator3_ transform(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator1_	firstIterator1,
	_ForwardIterator1_	lastIterator1,
	_ForwardIterator2_	firstIterator2,
	_ForwardIterator3_	destinationIterator,
	_Function_			function) noexcept
{

}

__BASE_NAMESPACE_END
