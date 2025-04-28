#include <base/core/utility/Algorithm.h>

__BASE_NAMESPACE_BEGIN

template <
	class _ForwardIterator_,
	class _Type_>
CONSTEXPR_CXX20 void fill(
	const _ForwardIterator_ firstIterator,
	const _ForwardIterator_ lastIterator,
	const _Type_& value)
{

}

template <
	class _ExecutionPolicy_,
	class _ForwardIterator_,
	class _Type_>
void fill(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator_	firstIterator,
	_ForwardIterator_	lastIterator,
	const _Type_& value) noexcept
{

}

template <
	class _OutputIterator_,
	class _DifferenceType_,
	class _Type_>
CONSTEXPR_CXX20 _OutputIterator_ fillN(
	_OutputIterator_		destinationIterator,
	const _DifferenceType_	_Count,
	const _Type_& value)
{

}

template <
	class _ExecutionPolicy_,
	class _ForwardIterator_,
	class _DifferenceType_,
	class _Type_>
_ForwardIterator_ fillN(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator_	destinationIterator,
	_DifferenceType_	_Count,
	const _Type_& value) noexcept
{

}

__BASE_NAMESPACE_END
