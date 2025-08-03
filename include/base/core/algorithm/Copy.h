#pragma once 

#include <src/core/utility/algorithm/AlgorithmDebug.h>
#include <src/core/utility/algorithm/IteratorWrap.h>

#include <src/core/memory/CopyBackward.h>
#include <src/core/utility/simd/SimdAlgorithm.h>

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

template <
	class _BidirectionalIterator_,
	class _OutputIterator_>
CONSTEXPR_CXX20 _OutputIterator_ reverseCopy(
	_BidirectionalIterator_ firstIterator,
	_BidirectionalIterator_ lastIterator,
	_OutputIterator_ destinationIterator)
{
	VerifyRange(firstIterator, lastIterator);

	using _Element_ = std::remove_reference_t<
		IteratorReferenceType<
			std::remove_const_t<_BidirectionalIterator_>
		>
	>;

	using _DestinationElement_ = std::remove_reference_t<
		IteratorReferenceType<_OutputIterator_>
	>;

	constexpr bool allowVectorization = std::conjunction_v<
			std::is_same<
				std::remove_const_t<_Element_>, _DestinationElement_>,
			std::bool_constant<IteratorsAreContiguous<_BidirectionalIterator_, _OutputIterator_>>,
			std::is_trivially_copyable<_Element_>,
		std::negation<std::is_volatile<_Element_>>
	>;

	constexpr size_t nx = sizeof(_Element_);

	if constexpr (allowVectorization && nx <= 8 && (nx & (nx - 1)) == 0) {
#if BASE_HAS_CXX20
		if (is_constant_evaluated() == false)
#endif 
		{
			ReverseCopyVectorized<nx>(
				memory::ToAddress(firstIterator), 
				memory::ToAddress(lastIterator),
				memory::ToAddress(destinationIterator));

			destinationIterator += lastIterator - firstIterator;
			return destinationIterator;
		}
	}

	for (; firstIterator != lastIterator; ++destinationIterator)
		*destinationIterator = *--lastIterator;

	return destinationIterator;
}

template <
	class _ExecutionPolicy_,
	class _BidirectionalIterator_,
	class _ForwardIterator_,
	EnableIfExecutionPolicy<_ExecutionPolicy_> = 0>
_ForwardIterator_ reverseCopy(
	_ExecutionPolicy_&&		executionPolicy, 
	_BidirectionalIterator_ firstIterator, 
	_BidirectionalIterator_ lastIterator,
	_ForwardIterator_		destinationIterator) noexcept 
{
	unused(executionPolicy);

	return reverseCopy(
		std::move(firstIterator), 
		std::move(lastIterator),
		std::move(destinationIterator));
}



__BASE_NAMESPACE_END
