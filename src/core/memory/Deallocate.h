#pragma once 

#include <src/core/memory/Destroy.h>

__BASE_MEMORY_NAMESPACE_BEGIN

template <class _Allocator_>
// Deallocates the range [start, end)
CONSTEXPR_CXX20 inline void DeallocateRange(
    AllocatorPointerType<_Allocator_>   pointerStart,
    AllocatorPointerType<_Allocator_>   pointerEnd,
    _Allocator_&                        allocator) noexcept
{
    if (pointerStart == nullptr || pointerEnd == nullptr)
        return; 

    DestroyRange(pointerStart, pointerEnd, allocator);
    allocator.deallocate(
        pointerStart, static_cast<AllocatorSizeType<_Allocator_>>(
        pointerEnd - pointerStart));
}

template <class _Allocator_>
// Deallocates the range [pointerStart, pointerStart + _Offset)
CONSTEXPR_CXX20 inline void DeallocateRangeCount(
    AllocatorPointerType<_Allocator_>   pointerStart,
    AllocatorSizeType<_Allocator_>      elementsCount,
    _Allocator_&                        allocator) noexcept
{
    if (elementsCount <= 0)
        return;

    const auto bytesCount = elementsCount * sizeof(AllocatorValueType<_Allocator_>);
    AllocatorPointerType<_Allocator_> end = pointerStart + bytesCount;

    DestroyRange(pointerStart, end, allocator);
    allocator.deallocate(pointerStart, bytesCount);
}

template <class _Allocator_>
// Deallocates the range [pointerStart, pointerStart + _BytesCount)
CONSTEXPR_CXX20 inline void DeallocateRangeBytes(
    AllocatorPointerType<_Allocator_>&  pointerStart,
    AllocatorSizeType<_Allocator_>      bytesCount,
    _Allocator_&                        allocator) noexcept
{
    if (bytesCount <= 0)
        return;

    AllocatorPointerType<_Allocator_> end = pointerStart + bytesCount;

    DestroyRange(pointerStart, end, allocator);
    allocator.deallocate(pointerStart, bytesCount);
}

template <class _Allocator_>
// Destroys the range [pointerStart, pointerCurrent), then deletes the range [pointerStart, pointerEnd)
CONSTEXPR_CXX20 inline void DeallocateUsedRange(
    AllocatorPointerType<_Allocator_>   pointerStart,
    AllocatorPointerType<_Allocator_>   pointerEnd,
    AllocatorPointerType<_Allocator_>   pointerCurrent,
    _Allocator_&                        allocator) noexcept
{
    if (pointerStart == nullptr || pointerEnd == nullptr)
        return; 

    DestroyRange(pointerStart, pointerCurrent, allocator);
    allocator.deallocate(
        pointerStart, 
        static_cast<AllocatorSizeType<_Allocator_>>(
            pointerEnd - pointerStart));
}

// ===================================================================

template <class _Allocator_>
// Deallocates the range [pointerStart, pointerEnd) and sets pointerStart and pointerEnd to nullptr
CONSTEXPR_CXX20 inline void FreeRange(
    AllocatorPointerType<_Allocator_>&  pointerStart,
    AllocatorPointerType<_Allocator_>&  pointerEnd,
    _Allocator_&                        allocator) noexcept
{
    DeallocateRange(pointerStart, pointerEnd, allocator);

    pointerStart    = nullptr;
    pointerEnd      = nullptr;
}

template <class _Allocator_>
// Deallocates the range [pointerStart, pointerStart + _Offset) and sets pointerStart to nullptr
CONSTEXPR_CXX20 inline void FreeRangeCount(
    AllocatorPointerType<_Allocator_>&  pointerStart,
    AllocatorSizeType<_Allocator_>      elementsCount,
    _Allocator_&                        allocator) noexcept
{
    DeallocateRangeCount(pointerStart, elementsCount, allocator);
    pointerStart = nullptr;
}

template <class _Allocator_>
// Deallocates the range [pointerStart, pointerStart + _BytesCount) and sets pointerStart to nullptr
CONSTEXPR_CXX20 inline void FreeRangeBytes(
    AllocatorPointerType<_Allocator_>&  pointerStart,
    AllocatorSizeType<_Allocator_>      bytesCount,
    _Allocator_&                        allocator) noexcept
{
    DeallocateRangeBytes(pointerStart, bytesCount, allocator);
    pointerStart = nullptr;
}

// ===================================================================

template <class _Allocator_>
// Destroys the range [pointerStart, pointerCurrent)
// Then deletes the range [pointerStart, pointerEnd) and sets pointerStart, pointerCurrent and pointerEnd to nullptr
CONSTEXPR_CXX20 inline void FreeUsedRange(
    AllocatorPointerType<_Allocator_>&  pointerStart,
    AllocatorPointerType<_Allocator_>&  pointerEnd,
    AllocatorPointerType<_Allocator_>&  pointerCurrent,
    _Allocator_&                        allocator) noexcept
{
    DeallocateUsedRange(
        pointerStart, pointerEnd,
        pointerCurrent, allocator);

    pointerStart    = nullptr;
    pointerEnd      = nullptr;
    pointerCurrent  = nullptr;
}


__BASE_MEMORY_NAMESPACE_END