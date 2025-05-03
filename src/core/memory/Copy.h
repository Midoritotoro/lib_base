#pragma once

#include <base/core/utility/TypeTraits.h>
#include <src/core/memory/IteratorCategory.h>

__BASE_MEMORY_NAMESPACE_BEGIN

inline NODISCARD bool MemoryCopy(
    void*       destinationPointer,
    const void* sourcePointer,
    size_t      sourceLength) noexcept
{
    return (
        memcpy(destinationPointer,
            sourcePointer, sourceLength)
        == destinationPointer);
}

template <
    class _InputIterator_, 
    class _SizeType_,
    class _OutIterator_>
// copy _First + [0, _Count) to _Dest + [0, _Count), returning _Dest + _Count
CONSTEXPR_CXX20 inline NODISCARD _OutIterator_ MemoryCopyCountUnchecked(
    _InputIterator_ firstIterator, 
    _SizeType_      count, 
    _OutIterator_   destinationIterator)
{
#if BASE_HAS_CXX20
    static_assert(is_nonbool_integral_v<_SizeType_>);
#endif // BASE_HAS_CXX20

    if constexpr (IteratorCopyCategory<_InputIterator_, _OutIterator_>::BitcopyAssignable)
#if BASE_HAS_CXX20
        if (is_constant_evaluated() == false)
#endif
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
NODISCARD inline CopyResult<_InputIterator_, _OutIterator_> MemoryCopyCommon(
    _InputIterator_ inputFirstIterator,
    _InputIterator_ inputLastIterator,
    _OutIterator_   outFirstIterator,
    _OutIterator_   outLastIterator) noexcept
{
    auto inputFirstAddress      = CheckedToChar(inputFirstIterator);
    const auto inputLastAddress = CheckedToConstChar(inputLastIterator);

    const auto outFirstAddress  = CheckedToChar(outFirstIterator);
    auto outLastAddress         = CheckedToConstChar(outLastIterator);

    const auto countBytes       = static_cast<size_t>((std::min)
        (inputLastAddress - inputFirstAddress, outLastAddress - outFirstAddress));

    if (MemoryCopy(outFirstAddress, inputFirstAddress, countBytes) == false)
        return CopyResult<_InputIterator_, _OutIterator_> {};

    if constexpr (std::is_pointer_v<_InputIterator_>)
        inputFirstIterator = reinterpret_cast<_InputIterator_>(inputFirstAddress + countBytes);
    else
        inputFirstIterator += static_cast<IteratorDifferenceType<_InputIterator_>>(
            countBytes / sizeof(IteratorValueType<_InputIterator_>));

    if constexpr (std::is_pointer_v<_OutIterator_>)
        outFirstIterator = reinterpret_cast<_OutIterator_>(outFirstAddress + countBytes);
    else
        outFirstIterator += static_cast<IteratorDifferenceType<_OutIterator_>>(
            countBytes / sizeof(IteratorValueType<_OutIterator_>));

    return CopyResult {
        std::move(inputFirstIterator),
        std::move(outFirstIterator)
    };
}

template <
    class _InputIterator_,
    class _OutIterator_>
NODISCARD inline CopyResult<_InputIterator_, _OutIterator_> MemoryCopyCount(
    _InputIterator_ inputFirstIterator,
    _OutIterator_   outFirstIterator,
    const sizetype  countObjects) noexcept
{
    const auto inputFirstAddress = CheckedToChar(inputFirstIterator);
    const auto outFirstAddress   = CheckedToChar(outFirstIterator);

    const auto countBytes = countObjects * sizeof(IteratorValueType<_InputIterator_>);

    if (MemoryCopy(outFirstAddress, inputFirstAddress, countBytes) == false)
        return {};

    if constexpr (std::is_pointer_v<_InputIterator_>)
        inputFirstIterator = reinterpret_cast<_InputIterator_>(inputFirstAddress + countBytes);
    else
        inputFirstIterator += static_cast<IteratorDifferenceType<_InputIterator_>>(countObjects);

    if constexpr (std::is_pointer_v<_OutIterator_>)
        outFirstIterator = reinterpret_cast<_OutIterator_>(outFirstAddress + countBytes);
    else
        outFirstIterator += static_cast<IteratorDifferenceType<_OutIterator_>>(countObjects);

    return {
        std::move(inputFirstIterator),
        std::move(outFirstIterator)
    };
}

template <
    class _InputIterator_,
    class _OutIterator_>
NODISCARD inline CopyResult<_InputIterator_, _OutIterator_> MemoryCopyBytes(
    _InputIterator_ inputFirstIterator,
    _OutIterator_   outFirstIterator,
    const sizetype  countBytes) noexcept
{
    const auto inputFirstAddress = CheckedToChar(inputFirst);
    const auto outFirstAddress   = CheckedToChar(outFirst);

    if (MemoryCopy(outFirstAddress, inputFirstAddress, countBytes) == false)
        return {};

    if constexpr (std::is_pointer_v<_InputIterator_>)
        inputFirstIterator = reinterpret_cast<_InputIterator_>(inputFirstAddress + countBytes);
    else
        inputFirstIterator += static_cast<IteratorDifferenceType<_InputIterator_>>(
            countBytes / sizeof(IteratorValueType<_InputIterator_>));

    if constexpr (std::is_pointer_v<_OutIterator_>)
        outFirstIterator = reinterpret_cast<_OutIterator_>(outFirstAddress + countBytes);
    else
        outFirstIterator += static_cast<IteratorDifferenceType<_OutIterator_>>(
            countBytes / sizeof(IteratorValueType<_OutIterator_>));

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
CONSTEXPR_CXX20 inline NODISCARD AllocatorPointerType<_Allocator_> UninitializedCopy(
    _InputIterator_                     _First,
    _Sentinel_                          _Last, 
    AllocatorPointerType<_Allocator_>   _Destination,
    _Allocator_&                        _Allocator)
{
    using _ValuePointer_ = AllocatorValueType<_Allocator_>*;

#if defined(OS_WIN) && defined(CPP_MSVC)

#  if BASE_HAS_CXX20
    auto _UFirst    = std::ranges::_Unwrap_iter<_Sentinel_>(std::move(_First));
    auto _ULast     = std::ranges::_Unwrap_sent<_InputIterator_>(std::move(_Last));
#  else
    auto _UFirst    = std::_Get_unwrapped(std::move(_First));
    auto _ULast     = std::_Get_unwrapped(std::move(_Last));
#  endif

#else 

    auto _UFirst    = UnFancy(std::move(_First));
    auto _ULast     = UnFancy(std::move(_Last));

#endif

    constexpr bool _Can_memmove =
#if defined (CPP_MSVC) && defined(OS_WIN)
        SentinelCopyCategory<
            decltype(_UFirst), decltype(_ULast), _ValuePointer_>::BitcopyConstructible
        && std::_Uses_default_construct<
            _Allocator_, _ValuePointer_, decltype(*_UFirst)>::value;
#else 
#endif

    if constexpr (_Can_memmove) {
#if BASE_HAS_CXX20
        if (!is_constant_evaluated())
#endif // BASE_HAS_CXX20
        {
            if constexpr (std::is_same_v<decltype(_UFirst), decltype(_ULast)>) {
                MemoryMove(
                    ToAddress(_UFirst), ToAdress(_ULast),
                    UnFancy(_Destination));
                _Destination += _ULast - _UFirst;
            }
            else {
                const auto _Count = static_cast<size_t>(_ULast - _UFirst);
                MemoryMove(
                    ToAddress(_UFirst), _Count, 
                    UnFancy(_Destination));

                _Destination += _Count;
            }

            return _Destination;
        }
    }

    UninitializedBackout<_Allocator_> _Backout{ _Destination, _Allocator };

    for (; _UFirst != _ULast; ++_UFirst)
        _Backout.EmplaceBack(*_UFirst);
    
    return _Backout.Release();
}

template <
    class _InputIterator_,
    class _Allocator_>
// Copy _First + [0, _Count) to raw _Dest, using _Allocator
CONSTEXPR_CXX20 inline NODISCARD AllocatorPointerType<_Allocator_> UninitializedCopyCount(
    _InputIterator_                     _First, 
    sizetype                            _Count, 
    AllocatorPointerType<_Allocator_>   _Destination,
    _Allocator_&                        _Allocator) 
{
    using _PointerToValue_ = AllocatorValueType<_Allocator_>*;

    auto _UFirst = _STD _Get_unwrapped(_STD move(_First));

    constexpr bool _Can_memmove =
        std::conjunction_v<
            std::bool_constant<
                IteratorCopyCategory<decltype(_UFirst), _PointerToValue_>::BitcopyConstructible>,
        std::_Uses_default_construct<_Allocator_, _PointerToValue_, decltype(*_UFirst)>>;

    if constexpr (_Can_memmove) {
#if BASE_HAS_CXX20
        if (!is_constant_evaluated())
#endif // BASE_HAS_CXX20
        {
            MemoryMove(_UFirst, _Count, UnFancy(_Destination));
            _Destination += _Count;

            return _Destination;
        }
    }

    UninitializedBackout<_Allocator_> _Backout { _Destination, _Allocator };

    for (; _Count != 0; ++_UFirst, (void) --_Count)
        _Backout.EmplaceBack(*_UFirst);

    return _Backout.Release();
}

template <
    class _InputIterator_, 
    class _NoThrowForwardIterator_>
// copy [_First, _Last) to raw [_Dest, ...)
CONSTEXPR_CXX20 inline NODISCARD _NoThrowForwardIterator_ UninitializedCopyUnchecked(
    _InputIterator_             _First,
    const _InputIterator_       _Last,
    _NoThrowForwardIterator_    _Dest)
{
    if constexpr (IteratorCopyCategory<_InputIterator_, _NoThrowForwardIterator_>::BitcopyConstructible) {
#if BASE_HAS_CXX20
        if (!is_constant_evaluated())
#endif
        {
            return MemoryMove(_First, _Last, _Dest);
        }
    }

    UninitializedBackout<_NoThrowForwardIterator_> _Backout{ _Dest };
    for (; _First != _Last; ++_First) {
        _Backout.EmplaceBack(*_First);
    }

    return _Backout.Release();
}

template <
    class _InputIterator_, 
    class _NoThrowForwardIterator_>
// copy [_First, _Last) to raw [_Dest, ...)
_InputIterator_ uninitialized_copy(
    const _InputIterator_       _First,
    const _InputIterator_       _Last,
    _NoThrowForwardIterator_    _Destination)
{
    _STD _Adl_verify_range(_First, _Last);

#if defined(OS_WIN) && defined(CPP_MSVC)
    auto _UFirst        = std::_Get_unwrapped(_First);
    const auto _ULast   = std::_Get_unwrapped(_Last);
#else 
    auto _UFirst        = UnFancy(_First);
    const auto _ULast   = UnFancy(_Last);
#endif

    auto _UDest = _STD _Get_unwrapped_n(_Destination, _STD _Idl_distance<_InputIterator_>(_UFirst, _ULast));

    _Destination = UninitializedCopyUnchecked(_UFirst, _ULast, _UDest);
    return _Destination;
}


__BASE_MEMORY_NAMESPACE_END