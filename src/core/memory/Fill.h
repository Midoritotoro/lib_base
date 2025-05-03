#pragma once 

#include <src/core/memory/AllocatorUtility.h>
#include <src/core/memory/FillMemsetSafety.h>

#include <src/core/memory/ToAddress.h>
#include <src/core/memory/UninitializedBackout.h>

#include <cstring>

__BASE_MEMORY_NAMESPACE_BEGIN

template <
    class _ContiguousIterator_,
    class _Type_>
void FillMemset(
    _ContiguousIterator_        destinationIterator,
    const _Type_&               value,
    const size_t                count)
{
    IteratorValueType<_ContiguousIterator_> destinationValue = value;
    memset(
        ToAddress(destinationIterator),
        UnCheckedToUnsignedChar(destinationValue), count);
}

template <class _ContiguousIterator_>
void MemsetZero(
    _ContiguousIterator_    destinationIterator,
    const size_t            count)
{
    memset(
        ToAddress(destinationIterator), 0,
        count * sizeof(IteratorValueType<_ContiguousIterator_>));
}

inline NODISCARD bool MemoryFill(
    void*       destinationPointer,
    const int   value,
    size_t      size) noexcept
{
    return (memset(destinationPointer, value, size) == destinationPointer);
}


template <class _Allocator_>
// copy _Count copies of _Val to raw _First, using _Al
CONSTEXPR_CXX20 inline NODISCARD AllocatorPointerType<_Allocator_> UninitializedFillCount(
    AllocatorPointerType<_Allocator_>       firstPointer,
    AllocatorSizeType<_Allocator_>          count,
    const AllocatorValueType<_Allocator_>&  value,
    _Allocator_&                            allocator)
{
    using _ValueType_ = AllocatorValueType<_Allocator_>;

    if constexpr (IsFillMemsetSafe<_ValueType_*, _ValueType_>) {
#if BASE_HAS_CXX20
        if (!is_constant_evaluated())
#endif // BASE_HAS_CXX20
        {
            FillMemset(UnFancy(firstPointer), value, static_cast<size_t>(count));
            return firstPointer + count;
        }
    }
    else if constexpr (IsFillZeroMemsetSafe<_ValueType_*, _ValueType_>) {
#if BASE_HAS_CXX20
        if (!is_constant_evaluated())
#endif // BASE_HAS_CXX20
        {
            if (IsAllBitsZero(value)) {
                MemsetZero(UnFancy(firstPointer), static_cast<size_t>(count));
                return firstPointer + count;
            }
        }
    }

    UninitializedBackout<_Allocator_> backout { firstPointer, allocator };

    for (; 0 < count; --count)
        backout.emplaceBack(value);

    return backout.release();
}

template <class _Allocator_>
// copy _Count copies of _Val to raw _First, using _Al
CONSTEXPR_CXX20 inline NODISCARD AllocatorPointerType<_Allocator_> UninitializedFill(
    AllocatorPointerType<_Allocator_>       firstPointer,
    AllocatorPointerType<_Allocator_>       lastPointer,
    const AllocatorValueType<_Allocator_>&  value,
    _Allocator_&                            allocator)
{
    return UninitializedFillCount(
        firstPointer, 
        IteratorsDifference(firstPointer, lastPointer), 
        value, allocator);
}

template <
    class _NoThrowForwardIterator_,
    class _Type_>
// copy value throughout raw [firstIterator, lastIterator)
void uninitialized_fill(
    const _NoThrowForwardIterator_  firstIterator, 
    const _NoThrowForwardIterator_  lastIterator,
    const _Type_&                   value) 
{
    using _WithoutCvref_ = std::remove_cvref_t<const _NoThrowForwardIterator_&>;

    auto first        = UnwrapIterator(firstIterator);
    const auto last   = UnwrapIterator(lastIterator);
     
    if constexpr (IsFillMemsetSafe<_WithoutCvref_, _Type_>) 
        return FillMemset(
            first, value,
            static_cast<size_t>(last - first));

    if constexpr (IsFillZeroMemsetSafe<_WithoutCvref_, _Type_>)
        if (IsAllBitsZero(value))
            return MemsetZero(first, static_cast<size_t>(last - first));
            
    UninitializedBackout<_WithoutCvref_> backout{ first };

    while (backout._lastPointer != last)
        backout.emplaceBack(value);

    backout.release();
}


__BASE_MEMORY_NAMESPACE_END
