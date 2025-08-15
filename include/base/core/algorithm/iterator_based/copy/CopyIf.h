#pragma once 

#include <src/core/algorithm/AlgorithmDebug.h>
#include <src/core/algorithm/IteratorWrap.h>

#include <base/core/memory/CopyBackward.h>
#include <src/core/utility/simd/SimdAlgorithm.h>

__BASE_NAMESPACE_BEGIN

template <
	class _Iterator_,
	class _OutputIterator_,
	class _Predicate_>
base_constexpr_cxx20 _OutputIterator_ copyIf(
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
	class _Predicate_,
	EnableIfExecutionPolicy<_ExecutionPolicy_> = 0>
_ForwardIterator2_ copyIf(
	_ExecutionPolicy_&& executionPolicy,
	_ForwardIterator1_	firstIterator1,
	_ForwardIterator1_	lastIterator1,
	_ForwardIterator2_	destinationIterator,
	_Predicate_			predicate) noexcept
{

}

__BASE_NAMESPACE_END
