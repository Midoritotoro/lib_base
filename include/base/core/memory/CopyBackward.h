#pragma once 

#include <base/core/type_traits/IteratorCategory.h>
#include <base/core/memory/PointerConversion.h>

__BASE_MEMORY_NAMESPACE_BEGIN

template <
    class _ContiguousIterator1_,
    class _ContiguousIterator2_>
_ContiguousIterator2_ CopyBackwardMemmove(
    _ContiguousIterator1_ firstIterator,
    _ContiguousIterator1_ lastIterator,
    _ContiguousIterator2_ destinationIterator)
{
    const char* const   first       = CheckedToConstChar(firstIterator);
    const char* const   last        = CheckedToConstChar(lastIterator);

    char* const         destination = CheckedToConstChar(destinationIterator);
    const auto          difference  = static_cast<size_t>(IteratorsDifference(first, last));

    auto result = memmove(destination - difference, first, difference);

    if constexpr (std::is_pointer_v<_ContiguousIterator2_>)
        return static_cast<_ContiguousIterator2_>(result);
    else
        return destinationIterator - (last - first);
}

template <
    class _BidirectionalIterator1_, 
    class _BidirectionalIterator2_>
_BidirectionalIterator2_ CopyBackwardMemmove(
    std::move_iterator<_BidirectionalIterator1_>    firstMoveIterator,
    std::move_iterator<_BidirectionalIterator1_>    lastMoveIterator,
    _BidirectionalIterator2_                        destinationIterator) 
{
    return CopyBackwardMemmove(
        firstMoveIterator.base(), 
        lastMoveIterator.base(), 
        destinationIterator);
}

template <
    class _BidirectionalIterator1_,
    class _BidirectionalIterator2_>
// copy [_First, _Last) backwards to [..., _Dest)
  base_constexpr_cxx20 _BidirectionalIterator2_ CopyBackwardUnchecked(
    _BidirectionalIterator1_ firstIterator, 
    _BidirectionalIterator1_ lastIterator, 
    _BidirectionalIterator2_ destinationIterator) 
{
    if constexpr (IteratorCopyCategory<_BidirectionalIterator1_, _BidirectionalIterator2_>::BitcopyAssignable) {
#if base_has_cxx20
        if (is_constant_evaluated() == false)
#endif // base_has_cxx20
        {
            return CopyBackwardMemmove(
                firstIterator, lastIterator,
                destinationIterator);
        }
    }

    while (firstIterator != lastIterator)
        *--destinationIterator = *--lastIterator;

    return destinationIterator;
}

template <
    class _Iterator1_, 
    class _Iterator2_>
base_constexpr_cxx20 void RewindIterator(
    _Iterator1_&    iterator,
    _Iterator2_&&   iteratorTo)
{
    iterator = std::forward<_Iterator2_>(iteratorTo);
}

__BASE_MEMORY_NAMESPACE_END