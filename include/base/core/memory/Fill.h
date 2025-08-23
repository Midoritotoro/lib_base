#pragma once 

#include <base/core/memory/AllocatorUtility.h>
#include <base/core/type_traits/FillMemsetSafety.h>

#include <base/core/memory/ToAddress.h>
#include <src/core/memory/UninitializedBackout.h>

#include <base/core/memory/FixedMemcmp.h>
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
    base::type_traits::IteratorValueType<_ContiguousIterator_> destinationValue = value;
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
        count * sizeof(base::type_traits::IteratorValueType<_ContiguousIterator_>));
}

inline   bool MemoryFill(
    void*       destinationPointer,
    const int   value,
    size_t      size) noexcept
{
    return (memset(destinationPointer, value, size) == destinationPointer);
}


template <class _Allocator_>
// copy _Count copies of _Val to raw _First, using _Al
base_constexpr_cxx20 inline AllocatorPointerType<_Allocator_> UninitializedFillCount(
    AllocatorPointerType<_Allocator_>       firstPointer,
    AllocatorSizeType<_Allocator_>          count,
    const AllocatorValueType<_Allocator_>&  value,
    _Allocator_&                            allocator)
{
    using _ValueType_ = AllocatorValueType<_Allocator_>;

    if constexpr (base::type_traits::IsFillMemsetSafe<_ValueType_*, _ValueType_>) {
#if base_has_cxx20
        if (!base::type_traits::is_constant_evaluated())
#endif // base_has_cxx20
        {
            FillMemset(UnFancy(firstPointer), value, static_cast<size_t>(count));
            return firstPointer + count;
        }
    }
    else if constexpr (base::type_traits::IsFillZeroMemsetSafe<_ValueType_*, _ValueType_>) {
#if base_has_cxx20
        if (!base::type_traits::is_constant_evaluated())
#endif // base_has_cxx20
        {
            if (base::memory::IsAllBitsZero(value)) {
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
base_constexpr_cxx20 inline   AllocatorPointerType<_Allocator_> UninitializedFill(
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
     
    if constexpr (base::type_traits::IsFillMemsetSafe<_WithoutCvref_, _Type_>)
        return FillMemset(
            first, value,
            static_cast<size_t>(last - first));

    if constexpr (base::type_traits::IsFillZeroMemsetSafe<_WithoutCvref_, _Type_>)
        if (IsAllBitsZero(value))
            return MemsetZero(first, static_cast<size_t>(last - first));
            
    UninitializedBackout<_WithoutCvref_> backout{ first };

    while (backout._lastPointer != last)
        backout.emplaceBack(value);

    backout.release();
}


__BASE_MEMORY_NAMESPACE_END
