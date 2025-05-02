#pragma once 

#include <base/core/memory/Memory.h>
#include <base/core/utility/Algorithm.h>

#include <base/core/utility/TypeTraits.h>

__BASE_MEMORY_NAMESPACE_BEGIN




// true, ���� � ���������� ���� ��� ���� ����� �������� 0
template <class _Type_>
CONSTEXPR_CXX20 NODISCARD inline bool IsAllBitsZero(const _Type_& value) {
    static_assert(std::is_scalar_v<_Type_> && !std::is_member_pointer_v<_Type_>);

    if constexpr (std::is_same_v<_Type_, std::nullptr_t>)
        return true;

    constexpr auto zero = _Type_{};
    return memcmp(&value, &zero, sizeof(_Type_)) == 0;
}

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
    
    return static_cast<std::size_t>(
        _LastUChar - _FirstUChar);
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
    class _DifferenceType_,
    class _ContiguousIterator_>
// Tries to get the difference between contiguous iterators
inline NODISCARD CONSTEXPR_CXX20 _DifferenceType_ IteratorsDifference(
    _ContiguousIterator_ _First,
    _ContiguousIterator_ _Last) noexcept
{
    if constexpr (std::is_pointer_v<_ContiguousIterator_>)
        return static_cast<_DifferenceType_>(_Last - _First);

    return static_cast<sizetype>(
        CheckedToConstChar(_Last) - CheckedToConstChar(_First));
}

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
    class _ContiguousIterator_,
    class _Type_>
void FillMemset(
    _ContiguousIterator_        _Destination,
    const _Type_&               _Value,
    const size_t                _Count) 
{
    IteratorValueType<_ContiguousIterator_> destinationValue = _Value;
    memset(
        ToAddress(_Destination), 
        UnCheckedToUnsignedChar(destinationValue), _Count);
}

template <class _ContiguousIterator_>
void MemsetZero(
    _ContiguousIterator_    _Destination,
    const size_t            _Count)
{
    const auto _Size = _Count * sizeof(IteratorValueType<_ContiguousIterator_>);
    memset(ToAddress(_Destination), 0, _Size);
}

__BASE_MEMORY_NAMESPACE_END
