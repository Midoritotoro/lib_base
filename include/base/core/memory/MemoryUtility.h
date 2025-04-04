#pragma once 

#include <base/core/memory/Memory.h>
#include <base/core/utility/Algorithm.h>

#include <base/core/utility/TypeTraits.h>

__BASE_MEMORY_NAMESPACE_BEGIN


#if __cpp_lib_addressof_constexpr >= 201603L
#  define CONSTEXPR_ADRESSOF constexpr
#else
#  define CONSTEXPR_ADRESSOF 
#endif

#if __cpp_lib_addressof_constexpr >= 201603L
#  if __has_builtin(__builtin_addressof)
     template <class _Type_>
     NODISCARD constexpr _Type_* AdressOf(_Type_& value) noexcept {
         return __builtin_addressof(value);
     }
#  endif
#endif

template <typename _Type_> 
CONSTEXPR_ADRESSOF NODISCARD inline _Type_* AdressOf(_Type_* value) noexcept {
    // http://eel.is/c++draft/pointer.conversion#1:
    static_assert(!std::is_function_v<_Type_>, "AdressOf must not be used on function types");
    return value;
}

template <typename _Type_>
CONSTEXPR_ADRESSOF NODISCARD inline _Type_* AdressOf(const _Type_& value) noexcept {
    return AdressOf(&value);
}

#if BASE_HAS_CXX20

template <class _Type_>
concept HasToAdress = requires(const _Type_ & _Val) {
    typename std::pointer_traits<_Type_>;
    std::pointer_traits<_Type_>::to_address(_Val);
};

template <class _Type_>
NODISCARD constexpr _Type_* ToAdress(_Type_* const _Val) noexcept {
    static_assert(!std::is_function_v<_Type_>, "N4950 [pointer.conversion]/1: Mandates: T is not a function type.");
    return _Val;
}

template <class _Ptr>
NODISCARD constexpr auto ToAdress(const _Ptr& _Val) noexcept {
    if constexpr (HasToAdress<_Ptr>)
        return std::pointer_traits<_Ptr>::to_address(_Val);
    else
        return ToAdress(_Val.operator->());
}

#endif

// true, если у скал€рного типа все биты имеют значение 0
template <class _Type_>
CONSTEXPR_CXX20 NODISCARD inline bool IsAllBitsZero(const _Type_& value) {
    static_assert(std::is_scalar_v<_Type_> && !std::is_member_pointer_v<_Type_>);

    if constexpr (std::is_same_v<_Type_, std::nullptr_t>)
        return true;

    constexpr auto zero = _Type_{};
    return memcmp(&value, &zero, sizeof(_Type_)) == 0;
}

//  онвертирует "модный" указатель в стандартный. 
// ћодными указател€ми считаютс€ типы с переопределенным оператором -> и *
template <class _PointerLikeType_>
NODISCARD inline constexpr decltype(auto)
    UnFancy(_PointerLikeType_ pointerLike) noexcept 
{ 
    return AdressOf(*pointerLike);
}

template <class _PointerLikeType_>
NODISCARD inline constexpr _PointerLikeType_* 
    UnFancy(_PointerLikeType_* pointerLike) noexcept
{
    return pointerLike;
}

//  онвертаци€ из потенциально нулевого модного указател€ в стандартный
template <class _PossiblyNullPointerLikeType_>
NODISCARD inline constexpr decltype(auto) 
    UnfancyMaybeNull(_PossiblyNullPointerLikeType_ possiblyNullPointerLike) noexcept
{
    return possiblyNullPointerLike
        ? AdressOf(*possiblyNullPointerLike)
        : nullptr;
}

template <class _PossiblyNullPointerLikeType_>
NODISCARD inline constexpr _PossiblyNullPointerLikeType_* 
    UnfancyMaybeNull(_PossiblyNullPointerLikeType_* possiblyNullPointerLike) noexcept
{
    return possiblyNullPointerLike;
}

template <typename _Type_>
NODISCARD inline constexpr char* 
    UnCheckedToChar(_Type_ pointerLike) noexcept
{
    return const_cast<char*>(
        reinterpret_cast<const volatile char*>(pointerLike));
}

template <typename _Type_> 
NODISCARD inline constexpr 
    const char* UnCheckedToConstChar(_Type_ pointerLike) noexcept
{
    return const_cast<const char*>(
        reinterpret_cast<const volatile char*>(pointerLike));
}

template <typename _Type_>
NODISCARD inline constexpr
    char* CheckedToChar(_Type_ pointerLike) noexcept
{
    const auto pointerLikeAdress = ToAdress(pointerLike);
    return UnCheckedToChar(pointerLikeAdress);
}

template <typename _Type_>
NODISCARD inline constexpr
    const char* CheckedToConstChar(_Type_ pointerLike) noexcept
{
    const auto pointerLikeAdress = ToAdress(pointerLike);
    return UnCheckedToConstChar(pointerLikeAdress);
}

template <typename _Type_>
NODISCARD inline constexpr
unsigned char*
    UnCheckedToUnsignedChar(
        _Type_ pointerLike) noexcept
{
    return const_cast<unsigned char*>(
        reinterpret_cast<
        const volatile unsigned char*>(pointerLike));
}

template <typename _Type_>
NODISCARD inline constexpr
const unsigned char* 
    UnCheckedToConstUnsignedChar(
        _Type_ pointerLike) noexcept
{
    return const_cast<const unsigned char*>(
        reinterpret_cast<
        const volatile unsigned char*>(pointerLike));
}

template <typename _Type_>
NODISCARD inline constexpr
    unsigned char* CheckedToUnsignedChar(
        _Type_ pointerLike) noexcept
{
    const auto pointerLikeAdress = ToAdress(pointerLike);
    return UnCheckedToUnsignedChar(pointerLikeAdress);
}

template <typename _Type_>
NODISCARD inline constexpr
const unsigned char* 
    CheckedToConstUnsignedChar(
        _Type_ pointerLike) noexcept
{
    const auto pointerLikeAdress = ToAdress(pointerLike);
    return UnCheckedToConstUnsignedChar(pointerLikeAdress);
}


template <class _BidIt>
inline constexpr void ReverseTail(
    _BidIt _First, 
    _BidIt _Last) noexcept 
{
    for (; _First != _Last && _First != --_Last; ++_First) {
        const auto _Temp = *_First;

        *_First = *_Last;
        *_Last = _Temp;
    }
}

template <
    class _BidIt, 
    class _OutIt>
inline constexpr void ReverseCopyTail(
    _BidIt _First,
    _BidIt _Last,
    _OutIt _Dest) noexcept 
{
    while (_First != _Last)
        *_Dest++ = *--_Last;
}

NODISCARD constexpr inline sizetype ByteLength(
    const void* _First,
    const void* _Last) noexcept 
{
    const auto _FirstUChar  = UnCheckedToConstUnsignedChar(_First);
    const auto _LastUChar   = UnCheckedToConstUnsignedChar(_Last);
    
    const auto _Length      = static_cast<sizetype>(_LastUChar - _FirstUChar);
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

template <class _Integral>
constexpr inline void AdvanceBytes(
    void*&      _Target, 
    _Integral   _Offset) noexcept 
{
    _Target = UnCheckedToUnsignedChar(_Target) + _Offset;
}

template <class _Integral>
constexpr inline void AdvanceBytes(
    const void*&    _Target, 
    _Integral       _Offset) noexcept 
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

template <class _Iterator_>
inline NODISCARD CONSTEXPR_CXX20 sizetype IteratorsDifference(
    _Iterator_ _First,
    _Iterator_ _Last) noexcept
{
    const auto _FirstAdress = CheckedToConstChar(_First);
    const auto _LastAdress  = CheckedToConstChar(_Last);

    const auto _Size        = static_cast<sizetype>(_LastAdress - _FirstAdress);
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
    const auto _DestinationAdress   = CheckedToChar(_Destination);
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
    const auto _DestinationAdress   = CheckedToChar(_Destination);
        
    const auto _FirstAdress         = CheckedToConstChar(_First);
    const auto _LastAdress          = CheckedToConstChar(_Last);

    const auto _Size                = static_cast<sizetype>(_LastAdress - _FirstAdress);
    const auto _Dest                = memmove(_DestinationAdress, _FirstAdress, _Size);

    const auto _Success             = (_Dest = _DestinationAdress);
    return _Success;
}

__BASE_MEMORY_NAMESPACE_END
