#pragma once

#include <base/core/utility/TypeTraits.h>

__BASE_MEMORY_NAMESPACE_BEGIN

inline NODISCARD bool MemoryCopy(
    void*       _Destination,
    const void* _Source,
    size_t      _SourceLength) noexcept
{
    const auto _Dest    = memcpy(_Destination,
        _Source, _SourceLength);
    const auto _Success = (_Dest == _Destination);

    return _Success;
}

template <
    class _InputIterator_, 
    class _SizeType_,
    class _OutIterator_>
// copy _First + [0, _Count) to _Dest + [0, _Count), returning _Dest + _Count
CONSTEXPR_CXX20 inline NODISCARD _OutIterator_ MemoryCopyCountUnChecked(
    _InputIterator_ _First, 
    _SizeType_      _Count, 
    _OutIterator_   _Dest)
{
#if BASE_HAS_CXX20
    static_assert(is_nonbool_integral_v<_SizeType_>);
#endif // BASE_HAS_CXX20

    if constexpr (IteratorCopyCategory<_InputIterator_, _OutIterator_>::BitcopyAssignable) {
#if BASE_HAS_CXX20
        if (!is_constant_evaluated())
#endif
        {
            return MemoryCopyMemmoveCount(_First, static_cast<size_t>(_Count), _Dest);
        }
    }

    for (; _Count != 0; ++_Dest, (void) ++_First, --_Count)
        *_Dest = *_First;

    return _Dest;
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
    _InputIterator_ inputFirst,
    _InputIterator_ inputLast,
    _OutIterator_   outFirst,
    _OutIterator_   outLast) noexcept
{
    using OutCopy               = CopyResult<_InputIterator_, _OutIterator_>;

    const auto inputFirstChar   = CheckedToChar(inputFirst);
    const auto inputLastChar    = CheckedToConstChar(inputLast);

    const auto outFirstChar     = CheckedToChar(outFirst);
    const auto outLastChar      = CheckedToConstChar(outLast);

    const auto countBytes       = static_cast<sizetype>((std::min)
        (inputLastChar - inputFirstChar, outLastChar - outFirstChar));

    if (MemoryCopy(outFirstChar, inputFirstChar, countBytes) == false)
        return OutCopy{};

    if constexpr (std::is_pointer_v<_InputIterator_>)
        inputFirst = reinterpret_cast<_InputIterator_>(inputFirstChar + countBytes);
    else
        inputFirst += static_cast<IteratorDifferenceType<_InputIterator_>>(
            countBytes / sizeof(IteratorValueType<_InputIterator_>));

    if constexpr (std::is_pointer_v<_OutIterator_>)
        outFirst = reinterpret_cast<_OutIterator_>(outFirstChar + countBytes);
    else
        outFirst += static_cast<IteratorDifferenceType<_OutIterator_>>(
            countBytes / sizeof(IteratorValueType<_OutIterator_>));

    return CopyResult {
        std::move(inputFirst), 
        std::move(outFirst) 
    };
}

template <
    class _InputIterator_,
    class _OutIterator_>
NODISCARD inline CopyResult<_InputIterator_, _OutIterator_> MemoryCopyCount(
    _InputIterator_ inputFirst,
    _OutIterator_   outFirst,
    const sizetype  countObjects) noexcept
{
    const auto inputFirstChar   = CheckedToChar(inputFirst);
    const auto outFirstChar     = CheckedToChar(outFirst);

    const auto countBytes       = countObjects 
        * sizeof(IteratorValueType<_InputIterator_>);

    if (MemoryCopy(outFirstChar, inputFirstChar, countBytes) == false)
        return {};

    if constexpr (std::is_pointer_v<_InputIterator_>)
        inputFirst = reinterpret_cast<_InputIterator_>(inputFirstChar + countBytes);
    else
        inputFirst += static_cast<IteratorDifferenceType<_InputIterator_>>(countObjects);

    if constexpr (std::is_pointer_v<_OutIterator_>)
        outFirst = reinterpret_cast<_OutIterator_>(outFirstChar + countBytes);
    else
        outFirst += static_cast<IteratorDifferenceType<_OutIterator_>>(countObjects);

    return {
        std::move(inputFirst),
        std::move(outFirst)
    };
}

template <
    class _InputIterator_,
    class _OutIterator_>
NODISCARD inline CopyResult<_InputIterator_, _OutIterator_> MemoryCopyBytes(
    _InputIterator_ inputFirst,
    _OutIterator_   outFirst,
    const sizetype  countBytes) noexcept
{
    const auto inputFirstChar = CheckedToChar(inputFirst);
    const auto outFirstChar = CheckedToChar(outFirst);

    if (MemoryCopy(outFirstChar, inputFirstChar, countBytes) == false)
        return {};

    if constexpr (std::is_pointer_v<_InputIterator_>)
        inputFirst = reinterpret_cast<_InputIterator_>(inputFirstChar + countBytes);
    else
        inputFirst += static_cast<IteratorDifferenceType<_InputIterator_>>(
            countBytes / sizeof(IteratorValueType<_InputIterator_>));

    if constexpr (std::is_pointer_v<_OutIterator_>)
        outFirst = reinterpret_cast<_OutIterator_>(outFirstChar + countBytes);
    else
        outFirst += static_cast<IteratorDifferenceType<_OutIterator_>>(
            countBytes / sizeof(IteratorValueType<_OutIterator_>));

    return {
        std::move(inputFirst),
        std::move(outFirst)
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