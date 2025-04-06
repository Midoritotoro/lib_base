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

template <
    typename _Type_,
    class   _Allocator_>
constexpr bool CanDestroyRange  = !std::conjunction_v<
    std::is_trivially_destructible<_Type_>,
    std::_Uses_default_destroy<_Allocator_, _Type_*>>;

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

__BASE_MEMORY_NAMESPACE_END
