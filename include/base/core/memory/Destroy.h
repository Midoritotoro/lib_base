#pragma once 

#include <base/core/utility/TypeTraits.h>

__BASE_MEMORY_NAMESPACE_BEGIN

template <class _Type_>
base_constexpr_cxx20 inline void DestroyInPlace(_Type_& object) noexcept;

template <class _Allocator_>
base_constexpr_cxx20 inline void DestroyRange(
    AllocatorPointerType<_Allocator_>       firstPointer,
    AllocatorConstPointerType<_Allocator_>  lastPointer,
    _Allocator_&                            allocator) noexcept 
{
    if constexpr (CanDestroyRange<AllocatorValueType<_Allocator_>, _Allocator_>)
        for (; firstPointer != lastPointer; ++firstPointer)
            allocator.destroy(UnFancy(firstPointer));
}

template <
    class _NoThrowForwardIterator_,
    class _NoThrowSentinel_>
base_constexpr_cxx20 inline void DestroyRange(
    _NoThrowForwardIterator_    firstIterator,
    const _NoThrowSentinel_     lastSentinel) noexcept
{
    if constexpr (std::is_trivially_destructible_v<IteratorValueType<_NoThrowForwardIterator_>> == false)
        for (; firstIterator != lastSentinel; ++firstIterator)
            DestroyInPlace(*firstIterator);
}

template <class _Type_>
base_constexpr_cxx20 inline void DestroyInPlace(_Type_& object) noexcept {
    if constexpr (std::is_array_v<_Type_>)
        DestroyRange(object, object + std::extent_v<_Type_>);
    else
        object.~_Type_();
}

__BASE_MEMORY_NAMESPACE_END
