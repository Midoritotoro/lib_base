#pragma once 

#include <src/core/memory/Copy.h>

__BASE_MEMORY_NAMESPACE_BEGIN



template <
    class _InputIterator_,
    class _OutIterator_>
inline NODISCARD CONSTEXPR_CXX20 bool MemoryMove(
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
inline NODISCARD CONSTEXPR_CXX20 bool MemoryMove(
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
    class _OutIterator_>
// move [_First, _Last) to [_Dest, ...)
CONSTEXPR_CXX20 inline NODISCARD _OutIterator_ MoveUnChecked(
    _InputIterator_ _First,
    _InputIterator_ _Last,
    _OutIterator_   _Destination)
{
    if constexpr (IteratorMoveCategory<_InputIterator_, _OutIterator_>::BitcopyAssignable)
#if BASE_HAS_CXX20
            if (!is_constant_evaluated())
#endif
                return MemoryCopyMemmoveCount(_First, _Last, _Destination);

    for (; _First != _Last; ++_Destination, (void) ++_First)
        *_Destination = std::move(*_First);

    return _Destination;
}


template <
    class _InputIterator_, 
    class _Allocator_>
// move [_First, _Last) to raw _Dest, using _Al
CONSTEXPR_CXX20 inline NODISCARD AllocatorPointerType<_Allocator_> UninitializedMove(
    const _InputIterator_               _First,
    const _InputIterator_               _Last, 
    AllocatorPointerType<_Allocator_>   _Destination,
    _Allocator_&                        _Allocator)
{
    using _Ptrval = AllocatorValueType<_Allocator_>*;

    auto _UFirst = std::_Get_unwrapped(_First);
    const auto _ULast = std::_Get_unwrapped(_Last);

    if constexpr (std::conjunction_v<std::bool_constant<IteratorMoveCategory<decltype(_UFirst), _Ptrval>::BitcopyConstructible>,
        std::_Uses_default_construct<_Allocator_, _Ptrval, decltype(std::move(*_UFirst))>>) {
#if BASE_HAS_CXX20
        if (!is_constant_evaluated())
#endif // BASE_HAS_CXX20
        {
            MemoryCopyMemmove(_UFirst, _ULast, UnFancy(_Destination));
            return _Destination + (_ULast - _UFirst);
        }
    }

    UninitializedBackout<_Allocator_> _Backout { _Destination, _Allocator };

    for (; _UFirst != _ULast; ++_UFirst) {
        _Backout.EmplaceBack(std::move(*_UFirst));
    }

    return _Backout.Release();
}

template <
    class _InputIterator_, 
    class _Allocator_>
// move [_First, _First + _Count) to raw _Destination, using _Allocator
CONSTEXPR_CXX20 inline NODISCARD AllocatorPointerType<_Allocator_> UninitializedMoveCount(
    const _InputIterator_               _First,
    const sizetype                      _Count, 
    AllocatorPointerType<_Allocator_>   _Destination,
    _Allocator_&                        _Allocator)
{
    using _Ptrval = AllocatorValueType<_Allocator_>*;

    auto _UFirst = std::_Get_unwrapped(_First);

    if constexpr (std::conjunction_v<std::bool_constant<IteratorMoveCategory<decltype(_UFirst), _Ptrval>::BitcopyConstructible>,
        std::_Uses_default_construct<_Allocator_, _Ptrval, decltype(std::move(*_UFirst))>>) {
#if BASE_HAS_CXX20
        if (!is_constant_evaluated())
#endif // BASE_HAS_CXX20
        {
            MemoryCopyMemmoveCount(_UFirst, _Count, UnFancy(_Destination));
            return _Destination + _Count;
        }
    }

    UninitializedBackout<_Allocator_> _Backout { _Destination, _Allocator };

    for (sizetype _Temp = _Count; _Temp < _Count; ++_UFirst, ++_Temp) {
        _Backout.EmplaceBack(std::move(*_UFirst));
    }

    return _Backout.Release();
}

// ===================================================================

__BASE_MEMORY_NAMESPACE_END