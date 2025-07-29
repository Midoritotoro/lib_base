#pragma once 

#include <src/core/memory/PointerConversion.h>

__BASE_MEMORY_NAMESPACE_BEGIN

template <class _BidirectionalIterator_>
constexpr void ReverseTail(
    _BidirectionalIterator_ _First,
    _BidirectionalIterator_ _Last) noexcept
{
    for (; _First != _Last && _First != --_Last; ++_First) {
        const auto _Temp = *_First;

        *_First = *_Last;
        *_Last = _Temp;
    }
}

template <
    class _BidirectionalIterator_,
    class _OutIterator_>
constexpr void ReverseCopyTail(
    _BidirectionalIterator_ _First,
    _BidirectionalIterator_ _Last,
    _OutIterator_           _Destination) noexcept
{
    while (_First != _Last)
        *_Destination++ = *--_Last;
}

NODISCARD always_inline std::size_t ByteLength(
    const void* _First,
    const void* _Last) noexcept
{
    const auto _FirstUChar = UnCheckedToConstUnsignedChar(_First);
    const auto _LastUChar = UnCheckedToConstUnsignedChar(_Last);

    return static_cast<std::size_t>(
        _LastUChar - _FirstUChar);
}

always_inline void RewindBytes(
    void*& _Target,
    size_t _Offset) noexcept
{
    _Target = UnCheckedToUnsignedChar(_Target) - _Offset;
}

always_inline void RewindBytes(
    const void*& _Target,
    size_t          _Offset) noexcept
{
    _Target = UnCheckedToConstUnsignedChar(_Target) - _Offset;
}

template <
    typename T,
    class _Integral_>
always_inline void AdvanceBytes(
    T*& _Target,
    _Integral_  _Offset) noexcept
{
    _Target = reinterpret_cast<T*>(UnCheckedToUnsignedChar(_Target) + _Offset);
}

template <
    typename T,
    class _Integral_>
always_inline void AdvanceBytes(
    const T*&       _Target,
    _Integral_      _Offset) noexcept
{
    _Target = reinterpret_cast<const T*>(UnCheckedToConstUnsignedChar(_Target) + _Offset);
}

__BASE_MEMORY_NAMESPACE_END
