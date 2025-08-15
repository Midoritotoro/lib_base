#pragma once 

#include <src/core/algorithm/AlgorithmDebug.h>
#include <src/core/algorithm/IteratorWrap.h>

#include <base/core/memory/CopyBackward.h>
#include <src/core/utility/simd/SimdAlgorithm.h>

__BASE_NAMESPACE_BEGIN

template <
	class _BidirectionalIterator_,
	class _OutputIterator_>
base_constexpr_cxx20 _OutputIterator_ reverseCopy(
	_BidirectionalIterator_ firstIterator,
	_BidirectionalIterator_ lastIterator,
	_OutputIterator_		destinationIterator)
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
#if base_has_cxx20
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
	_ExecutionPolicy_&&, 
	_BidirectionalIterator_ firstIterator, 
	_BidirectionalIterator_ lastIterator,
	_ForwardIterator_		destinationIterator) noexcept 
{
	return reverseCopy(
		std::move(firstIterator), 
		std::move(lastIterator),
		std::move(destinationIterator)
	);
}



__BASE_NAMESPACE_END
