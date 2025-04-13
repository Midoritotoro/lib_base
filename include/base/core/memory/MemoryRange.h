#pragma once 

#include <base/core/memory/MemoryUtility.h>

__BASE_MEMORY_NAMESPACE_BEGIN

#if BASE_HAS_CXX20

    template <typename _Allocator_>
    struct AllocatorPointerTraits {
        using type = typename std::allocator_traits<_Allocator_>::pointer;
    };

    template <typename _Allocator_>
        requires HasPointerType<_Allocator_>
    struct AllocatorPointerTraits<_Allocator_> {
        using type = typename _Allocator_::pointer;
    };

    template <typename _Allocator_>
    struct AllocatorConstPointerTraits {
        using type = typename std::allocator_traits<_Allocator_>::const_pointer;
    };

    template <typename _Allocator_>
        requires HasConstPointerType<_Allocator_>
    struct AllocatorConstPointerTraits<_Allocator_> {
        using type = typename _Allocator_::const_pointer;
    };

    template <typename _Allocator_>
    struct AllocatorValueTraits {
        using type = typename std::allocator_traits<_Allocator_>::value_type;
    };

    template <typename _Allocator_>
        requires HasValueType<_Allocator_>
    struct AllocatorValueTraits<_Allocator_> {
        using type = typename _Allocator_::value_type;
    };

    template <typename _Allocator_>
    struct AllocatorSizeTraits {
        using type = typename std::allocator_traits<_Allocator_>::size_type;
    };

    template <typename _Allocator_>
        requires HasSizeType<_Allocator_>
    struct AllocatorSizeTraits<_Allocator_> {
        using type = typename _Allocator_::size_type;
    };

    template <typename _Allocator_>
    using AllocatorPointerType      = typename AllocatorPointerTraits<_Allocator_>::type;

    template <class _Allocator_>
    using AllocatorConstPointerType = typename AllocatorConstPointerTraits<_Allocator_>::type;

    template <class _Allocator_>
    using AllocatorSizeType         = typename AllocatorSizeTraits<_Allocator_>::type;

    template <class _Allocator_>
    using AllocatorValueType        = typename AllocatorValueTraits<_Allocator_>::type;

#else 

    template <class _Allocator_>
    using AllocatorPointerType      = typename std::allocator_traits<_Allocator_>::pointer;

    template <class _Allocator_>
    using AllocatorConstPointerType = typename std::allocator_traits<_Allocator_>::const_pointer;

    template <class _Allocator_>
    using AllocatorSizeType         = typename std::allocator_traits<_Allocator_>::size_type;

    template <class _Allocator_>
    using AllocatorValueType        = typename std::allocator_traits<_Allocator_>::value_type;

#endif

#if defined(OS_WIN) && defined(CPP_MSVC)
    template <
        typename _Type_,
        class   _Allocator_>
    constexpr bool CanDestroyRange  = !std::conjunction_v<
        std::is_trivially_destructible<_Type_>,
        std::_Uses_default_destroy<_Allocator_, _Type_*>>;
#else 
    template <
        typename _Type_,
        class   _Allocator_>
    constexpr bool CanDestroyRange = !std::is_trivially_destructible_v<_Type_>;
#endif

template <class _Iterator_>
constexpr inline void VerifyRange(
    _Iterator_ _First,
    _Iterator_ _Last)
{

}


template <class _Allocator_>
class NODISCARD UninitializedBackout {
    // Class to undo partially constructed ranges in UninitializedXXX algorithms
private:
    using pointer = AllocatorPointerType<_Allocator_>;

public:
    CONSTEXPR_CXX20 inline UninitializedBackout(
        pointer         _Dest, 
        _Allocator_&    _Al_
    ) :
        _First(_Dest),
        _Last(_Dest), 
        _Al(_Al_) 
    {}

    UninitializedBackout(const UninitializedBackout&) = delete;
    UninitializedBackout& operator=(const UninitializedBackout&) = delete;

    CONSTEXPR_CXX20 ~UninitializedBackout() {
        DestroyRange(_First, _Last, _Al);
    }

    template <class... _Types>
    CONSTEXPR_CXX20 inline void EmplaceBack(_Types&&... _Vals) {
        // construct a new element at *_Last and increment
        std::allocator_traits<_Allocator_>::construct(
            _Al, UnFancy(_Last), 
            std::forward<_Types>(_Vals)...);

        ++_Last;
    }

    constexpr inline pointer Release() { 
        // suppress any exception handling backout and return _Last
        _First = _Last;
        return _Last;
    }
private:
    pointer         _First;
    pointer         _Last;
    _Allocator_&    _Al;
};

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
			_Ty*        _Location,
			_Types&&... _Args)
	{
		::new (static_cast<void*>(_Location))
			_Ty(std::forward<_Types>(_Args)...);
	}
	constexpr _Ty* ConstructAt(
		_Ty* const  _Location,
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
CONSTEXPR_CXX20 inline void ConstructInPlace(
    _Type_& _Obj, 
    _Types_&&... _Args) noexcept(
        std::is_nothrow_constructible_v<_Type_, _Types_...>) 
{
#if BASE_HAS_CXX20
    if (is_constant_evaluated())
        ConstructAt(AddressOf(_Obj), std::forward<_Types_>(_Args)...);
    else
#endif
        ::new (static_cast<void*>(AddressOf(_Obj)))
            _Type_(std::forward<_Types_>(_Args)...);
}

template <class _Type_>
CONSTEXPR_CXX20 inline void DestroyInPlace(_Type_& _Obj) noexcept;

template <class _Allocator_>
CONSTEXPR_CXX20 inline void DestroyRange(
    AllocatorPointerType<_Allocator_>       _First,
    AllocatorConstPointerType<_Allocator_>  _Last,
    _Allocator_&                            _Allocator) noexcept 
{
    using _Ty = AllocatorValueType<_Allocator_>;

    if constexpr (CanDestroyRange<_Ty, _Allocator_>)
        for (; _First != _Last; ++_First)
            _Allocator.destroy(UnFancy(_First));
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
// Deallocates the range [_Start, _End) and sets _Start and _End to nullptr
CONSTEXPR_CXX20 inline void FreeRange(
    AllocatorPointerType<_Allocator_>   _Start,
    AllocatorPointerType<_Allocator_>   _End,
    _Allocator_&                        _Allocator) noexcept
{
    if (UNLIKELY(!_Start) || UNLIKELY(!_End))
        return; 

    using _SizeType_    = AllocatorSizeType<_Allocator_>;
    const auto _Length  = static_cast<_SizeType_>(_End - _Start);

    memory::DestroyRange(_Start, _End, _Allocator);
    _Allocator.deallocate(_Start, _Length);

    _Start  = nullptr;
    _End    = nullptr;
}

template <class _Allocator_>
// Deallocates the range [_Start, _Start + _Offset) and sets _Start and calculated end to nullptr
CONSTEXPR_CXX20 inline void FreeRangeCount(
    AllocatorPointerType<_Allocator_>   _Start,
    AllocatorSizeType<_Allocator_>      _ElementsCount,
    _Allocator_&                        _Allocator) noexcept
{
    if (UNLIKELY(_ElementsCount <= 0))
        return;

    using _ValueType_       = AllocatorValueType<_Allocator_>;
    using _PointerType_     = AllocatorPointerType<_Allocator_>;

    const auto _BytesCount  = _ElementsCount * sizeof(_ValueType_);
    _PointerType_& _End     = _Start + _BytesCount;

    memory::DestroyRange(_Start, _End, _Allocator);
    _Allocator.deallocate(_Start, _BytesCount);

    _Start  = nullptr;
    _End    = nullptr;
}

template <class _Allocator_>
// Deallocates the range [_Start, _Start + _BytesCount) and sets _Start and calculated end to nullptr
CONSTEXPR_CXX20 inline void FreeRangeBytes(
    AllocatorPointerType<_Allocator_>   _Start,
    AllocatorSizeType<_Allocator_>      _BytesCount,
    _Allocator_&                        _Allocator) noexcept
{
    if (UNLIKELY(_BytesCount <= 0))
        return;

    using _ValueType_   = AllocatorValueType<_Allocator_>;
    using _PointerType_ = AllocatorPointerType<_Allocator_>;

    _PointerType_& _End = _Start + _BytesCount;

    memory::DestroyRange(_Start, _End, _Allocator);
    _Allocator.deallocate(_Start, _BytesCount);

    _Start  = nullptr;
    _End    = nullptr;
}


template <class _Allocator_>
// Deallocates the range [_Start, _End) without setting _Start and _End to nullptr
CONSTEXPR_CXX20 inline void DeallocateRange(
    AllocatorPointerType<_Allocator_>   _Start,
    AllocatorPointerType<_Allocator_>   _End,
    _Allocator_&                        _Allocator) noexcept
{
    if (UNLIKELY(!_Start) || UNLIKELY(!_End))
        return; 

    using _SizeType_    = AllocatorSizeType<_Allocator_>;
    const auto _Length  = static_cast<_SizeType_>(_End - _Start);

    memory::DestroyRange(_Start, _End, _Allocator);
    _Allocator.deallocate(_Start, _Length);
}

template <class _Allocator_>
// Deallocates the range [_Start, _Start + _Offset) without setting _Start and calculated end to nullptr
CONSTEXPR_CXX20 inline void DeallocateRangeCount(
    AllocatorPointerType<_Allocator_>   _Start,
    AllocatorSizeType<_Allocator_>      _ElementsCount,
    _Allocator_& _Allocator) noexcept
{
    if (UNLIKELY(_ElementsCount <= 0))
        return;

    using _ValueType_ = AllocatorValueType<_Allocator_>;
    using _PointerType_ = AllocatorPointerType<_Allocator_>;

    const auto _BytesCount = _ElementsCount * sizeof(_ValueType_);
    _PointerType_& _End = _Start + _BytesCount;

    memory::DestroyRange(_Start, _End, _Allocator);
    _Allocator.deallocate(_Start, _BytesCount);
}

template <class _Allocator_>
// Deallocates the range [_Start, _Start + _BytesCount) without setting _Start and calculated end to nullptr
CONSTEXPR_CXX20 inline void DeallocateRangeBytes(
    AllocatorPointerType<_Allocator_>   _Start,
    AllocatorSizeType<_Allocator_>      _BytesCount,
    _Allocator_& _Allocator) noexcept
{
    if (UNLIKELY(_BytesCount <= 0))
        return;

    using _ValueType_ = AllocatorValueType<_Allocator_>;
    using _PointerType_ = AllocatorPointerType<_Allocator_>;

    _PointerType_& _End = _Start + _BytesCount;

    memory::DestroyRange(_Start, _End, _Allocator);
    _Allocator.deallocate(_Start, _BytesCount);
}


// ------------------------------------------------------------------------------------------------

template <
    class _InputIterator_,
    class _Sentinel_, 
    class _Allocator_>
CONSTEXPR_CXX20 inline NODISCARD AllocatorPointerType<_Allocator_> UninitializedCopy(
    _InputIterator_                     _First,
    _Sentinel_                          _Last, 
    AllocatorPointerType<_Allocator_>   _Dest,
    _Allocator_&                        _Allocator)
{
    // copy [_First, _Last) to raw _Dest, using _Al
    // note: only called internally from elsewhere in the STL
    using _Ptrval = typename _Allocator_::value_type*;

#if BASE_HAS_CXX20
    auto _UFirst = _RANGES _Unwrap_iter<_Sentinel_>(_STD move(_First));
    auto _ULast = _RANGES _Unwrap_sent<_InputIterator_>(_STD move(_Last));
#else // ^^^ _HAS_CXX20 / !_HAS_CXX20 vvv
    // In pre-concepts world, UninitializedCopy should only ever be called with an iterator
    // and sentinel of the same type, so `_Get_unwrapped` is fine to call.
    auto _UFirst = _STD _Get_unwrapped(_STD move(_First));
    auto _ULast = _STD _Get_unwrapped(_STD move(_Last));
#endif // ^^^ !BASE_HAS_CXX20 ^^^

    constexpr bool _Can_memmove = _Sent_copy_cat<decltype(_UFirst), decltype(_ULast), _Ptrval>::_Bitcopy_constructible
        && _Uses_default_construct<_Alloc, _Ptrval, decltype(*_UFirst)>::value;

    if constexpr (_Can_memmove) {
#if BASE_HAS_CXX20
        if (!is_constant_evaluated())
#endif // BASE_HAS_CXX20
        {
            if constexpr (std::is_same_v<decltype(_UFirst), decltype(_ULast)>) {
                MemoryMove(ToAddress(_UFirst), ToAdress(_ULast), UnFancy(_Dest));
                _Dest += _ULast - _UFirst;
            }
            else {
                const auto _Count = static_cast<size_t>(_ULast - _UFirst);
                MemoryMove(ToAddress(_UFirst), _Count, UnFancy(_Dest));
                _Dest += _Count;
            }

            return _Dest;
        }
    }

    for (; _UFirst != _ULast; ++_UFirst) {
        ConstructInPlace(*_Last, _UFirst);
        ++_Last;

    }

    return _Backout._Release();
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
    using _Ptrval = AllocatorValueType<_Allocator_>*;

    auto _UFirst = _STD _Get_unwrapped(_STD move(_First));

    constexpr bool _Can_memmove =
        std::conjunction_v<std::bool_constant<std::_Iter_copy_cat<decltype(_UFirst), _Ptrval>::_Bitcopy_constructible>,
        std::_Uses_default_construct<_Allocator_, _Ptrval, decltype(*_UFirst)>>;

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

    UninitializedBackout<_Allocator_> _Backout { _Destination, _Allocator_ };
    for (; _Count != 0; ++_UFirst, (void) --_Count) {
        _Backout.EmplaceBack(*_UFirst);
    }

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
    if constexpr (std::_Iter_copy_cat<_InputIterator_, _NoThrowForwardIterator_>::_Bitcopy_constructible) {
#if _HAS_CXX20
        if (!is_constant_evaluated())
#endif // _HAS_CXX20
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
    _NoThrowForwardIterator_    _Dest)
{
    _STD _Adl_verify_range(_First, _Last);

    auto _UFirst = _STD _Get_unwrapped(_First);
    const auto _ULast = _STD _Get_unwrapped(_Last);

    auto _UDest = _STD _Get_unwrapped_n(_Dest, _STD _Idl_distance<_InputIterator_>(_UFirst, _ULast));

    _Dest = std::_Uninitialized_copy_unchecked(_UFirst, _ULast, _UDest));
    return _Dest;
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

    if constexpr (std::conjunction_v<std::bool_constant<std::_Iter_move_cat<decltype(_UFirst), _Ptrval>::_Bitcopy_constructible>,
        std::_Uses_default_construct<_Allocator_, _Ptrval, decltype(std::move(*_UFirst))>>) {
#if BASE_HAS_CXX20
        if (!is_constant_evaluated())
#endif // BASE_HAS_CXX20
        {
            MemoryCopy(_UFirst, _ULast, UnFancy(_Destination));
            return _Destination + (_ULast - _UFirst);
        }
    }

    UninitializedBackout<_Allocator_> _Backout { _Destination, _Allocator };

    for (; _UFirst != _ULast; ++_UFirst) {
        _Backout.EmplaceBack(std::move(*_UFirst));
    }

    return _Backout.Release();
}

template <class _Allocator_>
// copy _Count copies of _Val to raw _First, using _Al
CONSTEXPR_CXX20 inline NODISCARD AllocatorPointerType<_Allocator_> UninitializedFillCount(
    AllocatorPointerType<_Allocator_>       _First,
    AllocatorSizeType<_Allocator_>          _Count,
    const AllocatorValueType<_Allocator_>&  _Value,
    _Allocator_&                            _Allocator)
{
    using _Ty = AllocatorValueType<_Allocator_>;

    if constexpr (
        std::_Fill_memset_is_safe<_Ty*, _Ty> 
#if defined(OS_WIN) && defined(CPP_MSVC)
        && std::_Uses_default_construct<_Allocator_, _Ty*, _Ty>::value
#endif
    ) {
#if BASE_HAS_CXX20
        if (!is_constant_evaluated())
#endif // BASE_HAS_CXX20
        {
            FillMemset(UnFancy(_First), _Value, static_cast<size_t>(_Count));
            return _First + _Count;
        }
    }
    else if constexpr (
        std::_Fill_zero_memset_is_safe<_Ty*, _Ty> 
#if defined(OS_WIN) && defined(CPP_MSVC)
        && std::_Uses_default_construct<_Allocator_, _Ty*, _Ty>::value
#endif
        ) {
#if BASE_HAS_CXX20
        if (!is_constant_evaluated())
#endif // BASE_HAS_CXX20
        {
            if (IsAllBitsZero(_Value)) {
                MemsetZero(UnFancy(_First), static_cast<size_t>(_Count));
                return _First + _Count;
            }
        }
    }

    UninitializedBackout<_Allocator_> _Backout { _First, _Al };
    for (; 0 < _Count; --_Count) {
        _Backout.EmplaceBack(_Val);
    }

    return _Backout.Release();
}

template <
    class _NoThrowForwardIterator_,
    class _Type_>
// copy _Val throughout raw [_First, _Last)
void uninitialized_fill(
    const _NoThrowForwardIterator_  _First, 
    const _NoThrowForwardIterator_  _Last,
    const _Type_&                   _Value) 
{
#if defined(OS_WIN) && defined(CPP_MSVC)
    auto _UFirst        = std::_Get_unwrapped(_First);
    const auto _ULast   = std::_Get_unwrapped(_Last);
#else
    auto _UFirst        = UnFancy(_First);
    const auto _ULast   = UnFancy(_Last);
#endif

    if constexpr (std::_Fill_memset_is_safe<std::remove_cvref_t<const _NoThrowForwardIterator_&>, _Type_>) {
        FillMemset(_UFirst, _Value, static_cast<size_t>(_ULast - _UFirst));
    }
    else {
        if constexpr (std::_Fill_zero_memset_is_safe<std::remove_cvref_t<const _NoThrowForwardIterator_&>, _Type_>)
            if (IsAllBitsZero(_Value))
                return MemsetZero(_UFirst, static_cast<size_t>(_ULast - _UFirst));
            
        UninitializedBackout<std::remove_cvref_t<const _NoThrowForwardIterator_&>> _Backout{_UFirst};

        while (_Backout._Last != _ULast)
            _Backout.EmplaceBack(_Value);

        _Backout.Release();
    }
}


__BASE_MEMORY_NAMESPACE_END
