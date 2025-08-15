#pragma once 

#include <src/core/algorithm/AlgorithmDebug.h>
#include <src/core/algorithm/IteratorWrap.h>

#include <base/core/memory/CopyBackward.h>
#include <src/core/utility/simd/SimdAlgorithm.h>


__BASE_NAMESPACE_BEGIN

template <
	class _BidirectionalIterator1_,
	class _BidirectionalIterator2_>
base_constexpr_cxx20 _BidirectionalIterator2_ copyBackward(
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
