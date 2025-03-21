#pragma once 

#include <base/core/memory/Memory.h>

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
    return _value;
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
        return ToAdress(_Val.operator->()); // plain pointer overload must come first
}

#endif

// true, если у скал€рного типа все биты имеют значение 0
template <class _Type_>
NODISCARD inline bool IsAllBitsZero(const _Type_& value) {
    static_assert(std::is_scalar_v<_Type_> && !std::is_member_pointer_v<_Type_>);

    if constexpr (std::is_same_v<_Type_, std::nullptr_t>)
        return true;

    constexpr auto zero = _Type_{};
    return memcmp(&value, &zero, sizeof(_Type_)) == 0;
}

//  онвертирует "модный" указатель в стандартный. 
// ћодными указател€ми считаютс€ типы с переопределенным оператором -> и * (Ќапример, итераторы).
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
    UnCheckedToChar(_Type_* pointerLike) noexcept
{
    return const_cast<char*>(
        reinterpret_cast<const volatile char*>(pointerLike));
}

template <typename _Type_> 
NODISCARD inline constexpr 
    const char* UnCheckedToConstChar(_Type_* pointerLike) noexcept
{
    return const_cast<const char*>(
        reinterpret_cast<const volatile char*>(pointerLike));
}

template <typename _Type_>
NODISCARD inline constexpr
    char* CheckedToChar(_Type_* pointerLike) noexcept
{
    const auto pointerLikeAdress = ToAdress(pointerLike);
    return UnCheckedToChar(pointerLikeAdress);
}

template <typename _Type_>
NODISCARD inline constexpr
    const char* CheckedToConstChar(_Type_* pointerLike) noexcept
{
    const auto pointerLikeAdress = ToAdress(pointerLike);
    return UnCheckedToConstChar(pointerLikeAdress);
}

template <typename _Type_>
inline NODISCARD CONSTEXPR_CXX20 bool MemoryCopy(
    _Type_* _Destination,
    const _Type_* _Source,
    const size_t _SourceLength)
{
    const auto _Dest    = memcpy(_Destination,
        _Source, _SourceLength);
    const auto _Success = (_Dest == _Destination);

    return _Success;
}

template <typename _Type_>
inline NODISCARD CONSTEXPR_CXX20 bool MemoryFill(
    _Type_* _Destination,
    const int _Value,
    const size_t _Size)
{
    const auto _Dest    = memset(_Destination, 
        _Value, _Size);
    const auto _Success = (_Dest == _Destination);
    
    return _Success;
}

template <class _Iterator_>
inline NODISCARD CONSTEXPR_CXX20 sizetype IteratorsDifference(
    _Iterator_ first,
    _Iterator_ last)
{
    const auto firstAdress  = CheckedToConstChar(first);
    const auto lastAdress   = CheckedToConstChar(last);

    const auto _Size        = static_cast<sizetype>(lastAdress - firstAdress);
    return _Size;
}


template <
    class _InputIterator_,
    class _OutIterator_>
inline NODISCARD CONSTEXPR_CXX20 bool MemoryMove(
    _InputIterator_ first,
    sizetype        size,
    _OutIterator_   destination)
{
    const auto destinationAdress = CheckedToChar(destination);
    const auto firstAdress       = CheckedToConstChar(first);

    const auto _Dest             = memmove(destinationAdress, 
        firstAdress, size);
    const auto success           = (_Dest == destinationAdress);

    return success;
}

template <
    class _InputIterator_,
    class _OutIterator_> 
inline NODISCARD CONSTEXPR_CXX20 bool MemoryMove(
    _InputIterator_ first,
    _InputIterator_ last,
    _OutIterator_   destination)
{
    const auto destinationAdress = CheckedToChar(destination);
        
    const auto firstAdress       = CheckedToConstChar(first);
    const auto lastAdress        = CheckedToConstChar(last);

    const auto _Size             = static_cast<sizetype>(last - first);
    const auto _Dest             = memmove(destAdress, firstAdress, _Size);

    const auto success           = (_Dest = destinationAdress);
    return success;
}

template <
    class _FirstIterator_,
    class _SecondIterator_>
struct CopyResult {
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
    const auto inputFirstChar   = CheckedToChar(inputFirst);
    const auto inputLastChar    = CheckedToConstChar(inputLast);

    const auto outFirstChar     = CheckedToChar(outFirst);
    const auto outLastChar      = CheckedToConstChar(outLast);

    const auto countBytes       = static_cast<sizetype>((std::min)
        (inputLastChar - inputFirstChar, outLastChar - outFirstChar));

    if (MemoryCopy(outFirstChar, inputFirstChar, countBytes) == false)
        return {};

    if constexpr (std::is_pointer_v<_InputIterator_>)
        inputFirst = reinterpret_cast<_InputIterator_>(inputFirstChar + countBytes);
    else
        inputFirst += static_cast<std::iter_difference_t<_InputIterator_>>(
            countBytes / sizeof(std::iter_value_t<_InputIterator_>));

    if constexpr (std::is_pointer_v<_OutIterator_>)
        outFirst = reinterpret_cast<_OutIterator_>(outFirstChar + countBytes);
    else
        outFirst += static_cast<std::iter_difference_t<_OutIterator_>>(
            countBytes / sizeof(std::iter_value_t<_OutIterator_>));

    return {
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
        * sizeof(std::iter_value_t<_InputIterator_>);

    if (MemoryCopy(outFirstChar, inputFirstChar, countBytes) == false)
        return {};

    if constexpr (std::is_pointer_v<_InputIterator_>)
        inputFirst = reinterpret_cast<_InputIterator_>(inputFirstChar + countBytes);
    else
        inputFirst += static_cast<std::iter_difference_t<_InputIterator_>>(countObjects);

    if constexpr (std::is_pointer_v<_OutIterator_>)
        outFirst = reinterpret_cast<_OutIterator_>(outFirstChar + countBytes);
    else
        outFirst += static_cast<std::iter_difference_t<_OutIterator_>>(countObjects);

    return {
        std::move(inputFirst),
        std::move(outFirst)
    };
}


__BASE_MEMORY_NAMESPACE_END
