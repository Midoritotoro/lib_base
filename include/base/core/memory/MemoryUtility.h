#pragma once 

#include <base/core/memory/Memory.h>
#include <base/core/utility/Algorithm.h>

#include <base/core/utility/TypeTraits.h>

__BASE_MEMORY_NAMESPACE_BEGIN


template <class _Allocator_>
using AllocatorPointerType      = typename _Allocator_::pointer;

template <class _Allocator_>
using AllocatorConstPointerType = typename _Allocator_::const_pointer;

template <class _Allocator_>
using AllocatorSizeType         = typename _Allocator_::size_type;

template <class _Allocator_>
using AllocatorValueType        = typename _Allocator_::value_type;

template <
    typename _Type_,
    class   _Allocator_>
constexpr bool CanDestroyRange = !std::conjunction_v<
    std::is_trivially_destructible<_Type_>,
    std::_Uses_default_destroy<_Allocator_, _Type_*>>;

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

template <
    class _FirstIterator_,
    class _SecondIterator_>
struct CopyResult {
    CopyResult() noexcept = default;
    CopyResult(
        _FirstIterator_ _First, 
        _SecondIterator_ _Second) noexcept:
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
    using OutCopy = CopyResult<_InputIterator_, _OutIterator_>;

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
        inputFirst += static_cast<std::iter_difference_t<_InputIterator_>>(
            countBytes / sizeof(std::iter_value_t<_InputIterator_>));

    if constexpr (std::is_pointer_v<_OutIterator_>)
        outFirst = reinterpret_cast<_OutIterator_>(outFirstChar + countBytes);
    else
        outFirst += static_cast<std::iter_difference_t<_OutIterator_>>(
            countBytes / sizeof(std::iter_value_t<_OutIterator_>));

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

#if BASE_HAS_CXX20
	template <
		class _Ty,
		class... _Types>
			requires requires(
			_Ty* _Location,
			_Types&&... _Args)
	{
		::new (static_cast<void*>(_Location))
			_Ty(std::forward<_Types>(_Args)...);
	}
	constexpr _Ty* ConstructAt(
		_Ty* const _Location,
		_Types&&... _Args) noexcept(
			noexcept(::new(static_cast<void*>(_Location))
				_Ty(std::forward<_Types>(_Args)...)))
	{
		return ::new (static_cast<void*>(_Location))
			_Ty(std::forward<_Types>(_Args)...);
	}
#endif

template <
    class _Type_, 
    class... _Types_>
CONSTEXPR_CXX20 void ConstructInPlace(
    _Type_& _Obj, 
    _Types_&&... _Args) noexcept(
        std::is_nothrow_constructible_v<_Type_, _Types_...>) 
{
#if BASE_HAS_CXX20
    if (std::is_constant_evaluated())
        ConstructAt(AdressOf(_Obj), std::forward<_Types_>(_Args)...);
    else
#endif
        ::new (static_cast<void*>(AdressOf(_Obj))) 
            _Type_(std::forward<_Types_>(_Args)...);
}

template <class _Type_>
CONSTEXPR_CXX20 inline void DestroyInPlace(_Type_& _Obj) noexcept;

template <class _Allocator_>
CONSTEXPR_CXX20 inline void DestroyRange(
    AllocatorPointerType<_Allocator_>       _First,
    AllocatorConstPointerType<_Allocator_>  _Last,
    _Allocator_&                            _Al) noexcept 
{
    using _Ty = AllocatorValueType<_Allocator_>;

    if constexpr (CanDestroyRange<_Ty, _Allocator_>)
        for (; _First != _Last; ++_First)
            _Al.destroy(UnFancy(_First));
}

template <
    class _NoThrowFwdIt_,
    class _NoThrowSentinel_>
CONSTEXPR_CXX20 inline void DestroyRange(
    _NoThrowFwdIt_          _First,
    const _NoThrowSentinel_ _Last) noexcept
{
    if constexpr (!std::is_trivially_destructible_v<std::iter_value_t<_NoThrowFwdIt_>>)
        for (; _First != _Last; ++_First)
            DestroyInPlace(*_First);
}

template <class _Type_>
CONSTEXPR_CXX20 inline void DestroyInPlace(_Type_& _Obj) noexcept {
    if constexpr (std::is_array_v<_Type_>)
        DestroyRange(_Obj, _Obj + std::extent_v<_Type_>);
    else
        _Obj.~_Type_();
}


template <class _Allocator_>
CONSTEXPR_CXX20 inline void FreeRange(
    AllocatorPointerType<_Allocator_>   _Start,
    AllocatorPointerType<_Allocator_>   _End,
    _Allocator_&                        _Allocator) noexcept
{
    if (UNLIKELY(!_Start) || UNLIKELY(!_End))
        return; 

    using _SizeType_ = AllocatorSizeType<_Allocator_>;
    const auto _Length = static_cast<_SizeType_>(_End - _Start);

    memory::DestroyRange(_Start, _End, _Allocator);
    _Allocator.deallocate(_Start, _Length);

    _Start = nullptr;
    _End = nullptr;
}

template <class _Allocator_>
CONSTEXPR_CXX20 inline void FreeRangeCount(
    AllocatorPointerType<_Allocator_>   _Start,
    AllocatorSizeType<_Allocator_>      _ElementsCount,
    _Allocator_&                        _Allocator) noexcept
{
    if (UNLIKELY(_ElementsCount <= 0))
        return;

    using _ValueType_   = AllocatorValueType<_Allocator_>;
    using _PointerType_ = AllocatorPointerType<_Allocator_>;

    const auto _BytesCount  = _ElementsCount * sizeof(_ValueType_);
    _PointerType_& _End     = _Start + _BytesCount;

    memory::DestroyRange(_Start, _End, _Allocator);
    allocator.deallocate(_Start, _BytesCount);

    _Start = nullptr;
    _End = nullptr;
}

template <class _Allocator_>
CONSTEXPR_CXX20 inline void FreeRangeBytes(
    AllocatorPointerType<_Allocator_>   _Start,
    AllocatorSizeType<_Allocator_>      _BytesCount,
    _Allocator_&                        _Allocator) noexcept
{
    if (UNLIKELY(_ElementsCount <= 0))
        return;

    using _ValueType_   = AllocatorValueType<_Allocator_>;
    using _PointerType_ = AllocatorPointerType<_Allocator_>;

    _PointerType_& _End     = _Start + _BytesCount;

    memory::DestroyRange(_Start, _End, _Allocator);
    allocator.deallocate(_Start, _BytesCount);

    _Start = nullptr;
    _End = nullptr;
}


__BASE_MEMORY_NAMESPACE_END
