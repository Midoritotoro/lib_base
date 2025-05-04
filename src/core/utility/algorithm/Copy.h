#pragma once 

#include <src/core/utility/algorithm/AlgorithmDebug.h>
#include <src/core/utility/algorithm/IteratorWrap.h>

#include <src/core/memory/CopyBackward.h>

__BASE_NAMESPACE_BEGIN

template <
	class _Iterator_,
	class _OutputIterator_,
	class _Predicate_>
CONSTEXPR_CXX20 _OutputIterator_ copyIf(
	_Iterator_			firstIterator,
	_Iterator_			lastIterator,
	_OutputIterator_	destinationIterator,
	_Predicate_			predicate)
{
	VerifyRange(firstIterator, lastIterator);
	
	for (; firstIterator != lastIterator; unused(++firstIterator)) {
		if (predicate(*firstIterator)) {
			*destinationIterator = *firstIterator;
			unused(++destinationIterator);
		}
	}

	memory::RewindIterator(destinationIterator, destinationIterator);
	return destinationIterator;
}

template <
	class _ExecutionPolicy_,
	class _ForwardIterator1_,
	class _ForwardIterator2_,
	class _Predicate_>
_ForwardIterator2_ copyIf(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator1_	firstIterator1,
	_ForwardIterator1_	lastIterator1,
	_ForwardIterator2_	destinationIterator,
	_Predicate_			predicate) noexcept
{

}

template <
	class _BidirectionalIterator1_,
	class _BidirectionalIterator2_>
CONSTEXPR_CXX20 _BidirectionalIterator2_ copyBackward(
	_BidirectionalIterator1_ firstIterator,
	_BidirectionalIterator1_ lastIterator,
	_BidirectionalIterator2_ destinationIterator)
{
	VerifyRange(firstIterator, lastIterator);

	memory::RewindIterator(
		destinationIterator,
		memory::CopyBackwardUnchecked(
			firstIterator, lastIterator, destinationIterator));

	return destinationIterator;
}


__BASE_NAMESPACE_END
