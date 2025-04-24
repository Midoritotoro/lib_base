#pragma once 

#include <base/core/memory/Memory.h>
#include <base/core/utility/Algorithm.h>

#include <base/core/utility/TypeTraits.h>
#include <base/core/memory/MemoryPointerConversion.h>

__BASE_MEMORY_NAMESPACE_BEGIN

template <class _BidirectionalIterator_>
inline constexpr void ReverseTail(
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
inline constexpr void ReverseCopyTail(
    _BidirectionalIterator_ _First,
    _BidirectionalIterator_ _Last,
    _OutIterator_           _Destination) noexcept
{
    while (_First != _Last)
        *_Destination++ = *--_Last;
}

NODISCARD constexpr inline std::size_t ByteLength(
    const void* _First,
    const void* _Last) noexcept 
{
    const auto _FirstUChar  = UnCheckedToConstUnsignedChar(_First);
    const auto _LastUChar   = UnCheckedToConstUnsignedChar(_Last);
    
    const auto _Length      = static_cast<std::size_t>(
        _LastUChar - _FirstUChar);
    return _Length;
}

constexpr inline void RewindBytes(
    void*& _Target,
    size_t _Offset) noexcept
{
    _Target = UnCheckedToUnsignedChar(_Target) - _Offset;
}

constexpr inline void RewindBytes(
    const void*&    _Target, 
    size_t          _Offset) noexcept 
{
    _Target = UnCheckedToConstUnsignedChar(_Target) - _Offset;
}

template <class _Integral_>
constexpr inline void AdvanceBytes(
    void*&      _Target, 
    _Integral_  _Offset) noexcept
{
    _Target = UnCheckedToUnsignedChar(_Target) + _Offset;
}

template <class _Integral_>
constexpr inline void AdvanceBytes(
    const void*&    _Target, 
    _Integral_      _Offset) noexcept
{
    _Target = UnCheckedToConstUnsignedChar(_Target) + _Offset;
}

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

inline NODISCARD bool MemoryFill(
    void*       _Destination,
    const int   _Value,
    size_t      _Size) noexcept
{
    const auto _Dest    = memset(_Destination, 
        _Value, _Size);
    const auto _Success = (_Dest == _Destination);
    
    return _Success;
}

template <class _ContiguousIterator_>
// Tries to get the difference between contiguous iterators
inline NODISCARD CONSTEXPR_CXX20 sizetype IteratorsDifference(
    _ContiguousIterator_ _First,
    _ContiguousIterator_ _Last) noexcept
{
    if constexpr (std::is_pointer_v<_ContiguousIterator_>)
        return static_cast<sizetype>(_Last - _First);

    const auto _FirstAdress = CheckedToConstChar(_First);
    const auto _LastAdress  = CheckedToConstChar(_Last);

    const auto _Size        = static_cast<sizetype>(
        _LastAdress - _FirstAdress);

    return _Size;
}

template <
    class _InputIterator_,
    class _OutIterator_>
inline NODISCARD CONSTEXPR_CXX20 bool MemoryMove(
    _InputIterator_ _First,
    sizetype        _Size,
    _OutIterator_   _Destination) noexcept
{
    auto _DestinationAdress         = CheckedToChar(_Destination);
    const auto _FirstAdress         = CheckedToConstChar(_First);

    const auto _Dest                = memmove(_DestinationAdress,
        _FirstAdress, _Size);
    const auto _Success             = (_Dest == _DestinationAdress);

    return _Success;
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

    const auto _Success             = (_Dest = _DestinationAdress);
    return _Success;
}

template <
    class _InputIterator_,
    class _OutIterator_>
inline NODISCARD CONSTEXPR_CXX20 bool MemoryCopyMemmove(
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

    const auto _Success = (_Dest == _DestinationAdress);
    return _Success;
}

template <
    class _InputIterator_,
    class _OutIterator_>
inline NODISCARD CONSTEXPR_CXX20 bool MemoryCopyMemmoveCount(
    _InputIterator_ _First,
    const sizetype  _Count,
    _OutIterator_   _Destination) noexcept
{
    if (_Count <= 0)
        return false;

    auto _DestinationAdress         = CheckedToChar(_Destination);

    const auto _FirstAdress         = CheckedToConstChar(_First);

    const auto _Dest                = memmove(
        _DestinationAdress, _FirstAdress, _Count);

    const auto _Success = (_Dest == _DestinationAdress);
    return _Success;
}

template <
    class _ContiguousIterator_,
    class _Type_>
void FillMemset(
    _ContiguousIterator_        _Destination,
    const _ContiguousIterator_  _Value,
    const size_t                _Count) 
{
    // implicitly convert (a cast would suppress warnings); also handles _Iter_value_t<_CtgIt> being bool
    std::iter_value_t<_ContiguousIterator_> _DestinationValue = _Value;
    memset(
        ToAddress(_Destination), 
        UnCheckedToUnsignedChar(_DestinationValue), _Count);
}

template <class _ContiguousIterator_>
void MemsetZero(
    _ContiguousIterator_    _Destination,
    const size_t            _Count)
{
    const auto _Size = _Count * sizeof(std::iter_value_t<_ContiguousIterator_>);
    memset(ToAddress(_Destination), 0, _Size);
}

__BASE_MEMORY_NAMESPACE_END
