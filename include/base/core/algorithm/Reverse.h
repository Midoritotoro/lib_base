#pragma once

#include <src/core/utility/algorithm/AlgorithmDebug.h>
#include <src/core/utility/simd/SimdAlgorithm.h>

#include <base/core/utility/Execution.h>

__BASE_NAMESPACE_BEGIN

template <class _BidirectionalIterator_>
// reverse elements in [firstIterator, lastIterator)
CONSTEXPR_CXX20 void reverse(
    const _BidirectionalIterator_ firstIterator,
    const _BidirectionalIterator_ lastIterator)
{
    VerifyRange(firstIterator, lastIterator);
 
    using _Element_ = std::remove_reference_t<
        IteratorReferenceType<_BidirectionalIterator_>
    >;

    constexpr bool allowVectorization = std::conjunction_v<
        std::bool_constant<IteratorIsContiguous<_BidirectionalIterator_>>,
        IsTriviallySwappable<_Element_>, 
        std::negation<std::is_volatile<_Element_>>
    >;

    constexpr size_t elementSizeInBytes = sizeof(_Element_);

#pragma warning(suppress : 6326) // Potential comparison of a constant with another constant
    if constexpr (
        allowVectorization && elementSizeInBytes <= 8
        && (elementSizeInBytes & (elementSizeInBytes - 1)) == 0) 
#if BASE_HAS_CXX20
        if (is_constant_evaluated() == false)
#endif
            return ReverseVectorized<elementSizeInBytes>(
                memory::ToAddress(firstIterator),
                memory::ToAddress(lastIterator));

    for (; firstIterator != lastIterator && firstIterator != --lastIterator; ++firstIterator)
        std::iter_swap(firstIterator, lastIterator);
}

template <
    class _ExecutionPolicy_,
    class _BidirectionalIterator_,
    EnableIfExecutionPolicy<_ExecutionPolicy_> = 0>
// reverse elements in [firstIterator, lastIterator)
void reverse(
    _ExecutionPolicy_&&     executionPolicy,  
    _BidirectionalIterator_ firstIterator,
    _BidirectionalIterator_ lastIterator) noexcept 
{
    unused(executionPolicy);
    return reverse(firstIterator, lastIterator);
}

__BASE_NAMESPACE_END
