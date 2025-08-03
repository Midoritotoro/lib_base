#pragma once 

#include <src/core/memory/Copy.h>

__BASE_MEMORY_NAMESPACE_BEGIN

template <
    class _InputIterator_,
    class _OutIterator_>
// move [_First, _Last) to [_Dest, ...)
CONSTEXPR_CXX20 inline NODISCARD _OutIterator_ MoveUnChecked(
    _InputIterator_ firstIterator,
    _InputIterator_ lastIterator,
    _OutIterator_   destinationIterator)
{
    if constexpr (IteratorMoveCategory<_InputIterator_, _OutIterator_>::BitcopyAssignable)
#if BASE_HAS_CXX20
            if (!is_constant_evaluated())
#endif
                return MemoryCopyMemmoveCount(firstIterator, lastIterator, destinationIterator);

    for (; firstIterator != lastIterator; ++destinationIterator, unused(++firstIterator))
        *destinationIterator = std::move(*firstIterator);

    return destinationIterator;
}


template <
    class _InputIterator_, 
    class _Allocator_>
// move [_First, _Last) to raw _Dest, using _Al
CONSTEXPR_CXX20 inline NODISCARD AllocatorPointerType<_Allocator_> UninitializedMove(
    _InputIterator_                     firstIterator,
    _InputIterator_                     lastIterator, 
    AllocatorPointerType<_Allocator_>   destinationPointer,
    _Allocator_&                        allocator)
{
    using _Ptrval = AllocatorValueType<_Allocator_>*;

    if constexpr (IteratorMoveCategory<_InputIterator_, _Ptrval>::BitcopyConstructible) {
#if BASE_HAS_CXX20
        if (!is_constant_evaluated())
#endif // BASE_HAS_CXX20
        {
            MemoryMove(firstIterator, lastIterator, UnFancy(destinationPointer));
            return destinationPointer + (lastIterator - firstIterator);
        }
    }

    UninitializedBackout<_Allocator_> backout { destinationPointer, allocator };

    for (; firstIterator != lastIterator; ++firstIterator)
        backout.emplaceBack(std::move(*firstIterator));

    return backout.release();
}

template <
    class _InputIterator_, 
    class _Allocator_>
// move [_First, _First + _Count) to raw _Destination, using _Allocator
CONSTEXPR_CXX20 inline NODISCARD AllocatorPointerType<_Allocator_> UninitializedMoveCount(
    const _InputIterator_               firstIterator,
    const sizetype                      count, 
    AllocatorPointerType<_Allocator_>   destinationPointer,
    _Allocator_&                        allocator)
{
    using _Ptrval = AllocatorValueType<_Allocator_>*;

    if constexpr (IteratorMoveCategory<_InputIterator_, _Ptrval>::BitcopyConstructible) {
#if BASE_HAS_CXX20
        if (!is_constant_evaluated())
#endif // BASE_HAS_CXX20
        {
            MemoryCopyCount(firstIterator, count, UnFancy(destinationPointer));
            return destinationPointer + count;
        }
    }

    UninitializedBackout<_Allocator_> backout{ destinationPointer, allocator };

    for (sizetype temp = count; temp < count; ++firstIterator, ++temp)
        backout.emplaceBack(std::move(*firstIterator));

    return backout.release();
}

// ===================================================================

__BASE_MEMORY_NAMESPACE_END