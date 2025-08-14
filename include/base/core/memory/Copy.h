#pragma once

#include <base/core/type_traits/TypeTraits.h>

#include <base/core/type_traits/IteratorCategory.h>
#include <base/core/memory/AllocatorUtility.h>

#include <src/core/memory/UninitializedBackout.h>
#include <base/core/memory/PointerConversion.h>

#include <base/core/type_traits/IsConstantEvaluated.h>
#include <base/core/type_traits/IntegralProperties.h>

__BASE_MEMORY_NAMESPACE_BEGIN

inline   bool MemoryCopy(
    void*       destinationPointer,
    const void* sourcePointer,
    size_t      sourceLength) noexcept
{
    return (memcpy(destinationPointer, sourcePointer, sourceLength) == destinationPointer);
}

template <
    class _InputIterator_,
    class _OutIterator_>
inline   base_constexpr_cxx20 bool MemoryMove(
    _InputIterator_ _First,
    sizetype        _Size,
    _OutIterator_   _Destination) noexcept
{
    if (_Size <= 0)
        return false;

    auto _DestinationAdress = CheckedToChar(_Destination);
    const auto _Dest        = memmove(_DestinationAdress, CheckedToConstChar(_First), _Size);

    return (_Dest == _DestinationAdress);;
}

template <
    class _InputIterator_,
    class _OutIterator_> 
inline   base_constexpr_cxx20 bool MemoryMove(
    _InputIterator_ _First,
    _InputIterator_ _Last,
    _OutIterator_   _Destination) noexcept
{
    auto _DestinationAdress         = CheckedToChar(_Destination);
        
    const auto _FirstAdress         = CheckedToConstChar(_First);
    const auto _LastAdress          = CheckedToConstChar(_Last);

    const auto _Size                = static_cast<sizetype>(
        _LastAdress - _FirstAdress);
    const auto _Dest                = memmove(
        _DestinationAdress, _FirstAdress, _Size);

    return (_Dest == _DestinationAdress);
}


template <
    class _InputIterator_, 
    class _SizeType_,
    class _OutIterator_>
// copy _First + [0, _Count) to _Dest + [0, _Count), returning _Dest + _Count
base_constexpr_cxx20 inline   _OutIterator_ MemoryCopyCountUnchecked(
    _InputIterator_ firstIterator, 
    _SizeType_      count, 
    _OutIterator_   destinationIterator)
{
#if base_has_cxx20
    static_assert(base::type_traits::is_nonbool_integral_v<_SizeType_>);
#endif // base_has_cxx20

    if constexpr (base::type_traits::IteratorCopyCategory<_InputIterator_, _OutIterator_>::BitcopyAssignable)
#if base_has_cxx20
        if (base::type_traits::is_constant_evaluated() == false)
#endif // base_has_cxx20
            return MemoryCopyMemmoveCount(
                std::move(firstIterator), 
                static_cast<size_t>(count), 
                std::move(destinationIterator));

    for (; count != 0; ++destinationIterator, unused(++firstIterator), --count)
        *destinationIterator = *firstIterator;

    return destinationIterator;
}

template <
    class _FirstIterator_,
    class _SecondIterator_>
struct CopyResult {
    CopyResult() noexcept = default;
    CopyResult(
        _FirstIterator_     _First, 
        _SecondIterator_    _Second) noexcept:
            first(_First),
            second(_Second)
    {}

    _FirstIterator_     first  = {};
    _SecondIterator_    second = {};
};

template <
    class _InputIterator_,
    class _OutIterator_> 
  inline CopyResult<_InputIterator_, _OutIterator_> MemoryCopyCommon(
    _InputIterator_ inputFirstIterator,
    _InputIterator_ inputLastIterator,
    _OutIterator_   outFirstIterator,
    _OutIterator_   outLastIterator) noexcept
{
    auto inputFirstAddress      = CheckedToChar(inputFirstIterator);
    const auto inputLastAddress = CheckedToConstChar(inputLastIterator);

    auto outFirstAddress        = CheckedToChar(outFirstIterator);
    const auto outLastAddress   = CheckedToConstChar(outLastIterator);

    const auto countBytes       = static_cast<size_t>((std::min)
        (inputLastAddress - inputFirstAddress, outLastAddress - outFirstAddress));

    if (MemoryCopy(outFirstAddress, inputFirstAddress, countBytes) == false)
        return CopyResult<_InputIterator_, _OutIterator_> {};

    if constexpr (std::is_pointer_v<_InputIterator_>)
        inputFirstIterator = reinterpret_cast<_InputIterator_>(inputFirstAddress + countBytes);
    else
        inputFirstIterator += static_cast<base::type_traits::IteratorDifferenceType<_InputIterator_>>(
            countBytes / sizeof(base::type_traits::IteratorValueType<_InputIterator_>));

    if constexpr (std::is_pointer_v<_OutIterator_>)
        outFirstIterator = reinterpret_cast<_OutIterator_>(outFirstAddress + countBytes);
    else
        outFirstIterator += static_cast<base::type_traits::IteratorDifferenceType<_OutIterator_>>(
            countBytes / sizeof(base::type_traits::IteratorValueType<_OutIterator_>));

    return CopyResult {
        std::move(inputFirstIterator),
        std::move(outFirstIterator)
    };
}

template <
    class _InputIterator_,
    class _OutIterator_>
  inline CopyResult<_InputIterator_, _OutIterator_> MemoryCopyCount(
    _InputIterator_ inputFirstIterator,
    _OutIterator_   outFirstIterator,
    const sizetype  countObjects) noexcept
{
    auto inputFirstAddress = CheckedToChar(inputFirstIterator);
    auto outFirstAddress   = CheckedToChar(outFirstIterator);

    const auto countBytes = countObjects * sizeof(base::type_traits::IteratorValueType<_InputIterator_>);

    if (MemoryCopy(outFirstAddress, inputFirstAddress, countBytes) == false)
        return {};

    if constexpr (std::is_pointer_v<_InputIterator_>)
        inputFirstIterator = reinterpret_cast<_InputIterator_>(inputFirstAddress + countBytes);
    else
        inputFirstIterator += static_cast<base::type_traits::IteratorDifferenceType<_InputIterator_>>(countObjects);

    if constexpr (std::is_pointer_v<_OutIterator_>)
        outFirstIterator = reinterpret_cast<_OutIterator_>(outFirstAddress + countBytes);
    else
        outFirstIterator += static_cast<base::type_traits::IteratorDifferenceType<_OutIterator_>>(countObjects);

    return {
        std::move(inputFirstIterator),
        std::move(outFirstIterator)
    };
}

template <
    class _InputIterator_,
    class _OutIterator_>
  inline CopyResult<_InputIterator_, _OutIterator_> MemoryCopyBytes(
    _InputIterator_ inputFirstIterator,
    _OutIterator_   outFirstIterator,
    const sizetype  countBytes) noexcept
{
    auto inputFirstAddress = CheckedToChar(inputFirstIterator);
    auto outFirstAddress   = CheckedToChar(outFirstIterator);

    if (MemoryCopy(outFirstAddress, inputFirstAddress, countBytes) == false)
        return {};

    if constexpr (std::is_pointer_v<_InputIterator_>)
        inputFirstIterator = reinterpret_cast<_InputIterator_>(inputFirstAddress + countBytes);
    else
        inputFirstIterator += static_cast<base::type_traits::IteratorDifferenceType<_InputIterator_>>(
            countBytes / sizeof(base::type_traits::IteratorValueType<_InputIterator_>));

    if constexpr (std::is_pointer_v<_OutIterator_>)
        outFirstIterator = reinterpret_cast<_OutIterator_>(outFirstAddress + countBytes);
    else
        outFirstIterator += static_cast<base::type_traits::IteratorDifferenceType<_OutIterator_>>(
            countBytes / sizeof(base::type_traits::IteratorValueType<_OutIterator_>));

    return {
        std::move(inputFirstIterator),
        std::move(outFirstIterator)
    };
}


// ===================================================================

template <
    class _InputIterator_,
    class _Sentinel_, 
    class _Allocator_>
// copy [_First, _Last) to raw _Destination, using _Allocator
base_constexpr_cxx20 inline   AllocatorPointerType<_Allocator_> UninitializedCopy(
    _InputIterator_                     firstIterator,
    _Sentinel_                          lastSentinel, 
    AllocatorPointerType<_Allocator_>   destinationPointer,
    _Allocator_&                        allocator)
{
    using _ValuePointer_ = AllocatorValueType<_Allocator_>*;

    constexpr bool canMemmove = base::type_traits::SentinelCopyCategory<
        _InputIterator_, _Sentinel_, _ValuePointer_>::BitcopyConstructible;

    if constexpr (canMemmove) {
#if base_has_cxx20
        if (!base::type_traits::is_constant_evaluated())
#endif // base_has_cxx20
        {
            if constexpr (std::is_same_v<_InputIterator_, _Sentinel_>) {
                MemoryMove(
                    ToAddress(firstIterator), ToAdress(lastSentinel),
                    UnFancy(destinationPointer));
                destinationPointer += lastSentinel - firstIterator;
            }
            else {
                const auto _Count = static_cast<size_t>(lastSentinel - firstIterator);
                MemoryMove(
                    ToAddress(firstIterator), _Count,
                    UnFancy(destinationPointer));

                destinationPointer += _Count;
            }

            return destinationPointer;
        }
    }

    UninitializedBackout<_Allocator_> backout{ destinationPointer, allocator };

    for (; firstIterator != lastSentinel; ++firstIterator)
        backout.emplaceBack(*firstIterator);
    
    return backout.release();
}

template <
    class _InputIterator_,
    class _Allocator_>
// Copy _First + [0, _Count) to raw _Dest, using _Allocator
base_constexpr_cxx20 inline AllocatorPointerType<_Allocator_> UninitializedCopyCount(
    _InputIterator_                     firstIterator, 
    sizetype                            count, 
    AllocatorPointerType<_Allocator_>   destinationPointer,
    _Allocator_&                        allocator) 
{
    using _PointerToValue_ = AllocatorValueType<_Allocator_>*;

    constexpr bool canMemove = std::bool_constant<
        base::type_traits::IteratorCopyCategory<
            _InputIterator_, _PointerToValue_>::BitcopyConstructible>::value;

    if constexpr (canMemove) {
#if base_has_cxx20
        if (!base::type_traits::is_constant_evaluated())
#endif // base_has_cxx20
        {
            MemoryMove(firstIterator, count, UnFancy(destinationPointer));
            destinationPointer += count;

            return destinationPointer;
        }
    }

    UninitializedBackout<_Allocator_> backout { destinationPointer, allocator };

    for (; count != 0; ++firstIterator, base_unused(--count))
        backout.emplaceBack(*firstIterator);

    return backout.release();
}

template <
    class _InputIterator_, 
    class _NoThrowForwardIterator_>
// copy [firstIterator, lastIterator) to raw [destinationIterator, ...)
base_constexpr_cxx20 inline   _NoThrowForwardIterator_ UninitializedCopyUnchecked(
    _InputIterator_             firstIterator,
    const _InputIterator_       lastIterator,
    _NoThrowForwardIterator_    destinationIterator)
{
    if constexpr (base::type_traits::IteratorCopyCategory<_InputIterator_, _NoThrowForwardIterator_>::BitcopyConstructible)
#if base_has_cxx20
        if (base::type_traits::is_constant_evaluated() == false)
#endif
            return MemoryMove(firstIterator, lastIterator, destinationIterator);

    UninitializedBackout<_NoThrowForwardIterator_> backout{ destinationIterator };

    for (; firstIterator != lastIterator; unused(++firstIterator))
        backout.emplaceBack(*firstIterator);

    return backout.release();
}

template <
    class _InputIterator_, 
    class _NoThrowForwardIterator_>
// copy [firstIterator, lastIterator) to raw [destinationIterator, ...)
_InputIterator_ uninitialized_copy(
    const _InputIterator_       firstIterator,
    const _InputIterator_       lastIterator,
    _NoThrowForwardIterator_    destinationIterator)
{
    VerifyRange(firstIterator, lastIterator);
    return UninitializedCopyUnchecked(firstIterator, lastIterator, destinationIterator);
}


__BASE_MEMORY_NAMESPACE_END