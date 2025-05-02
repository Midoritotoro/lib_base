#pragma once 

#include <base/core/utility/TypeTraits.h>

__BASE_MEMORY_NAMESPACE_BEGIN

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
        IsFillMemsetSafe<_Ty*, _Ty> 
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
        IsFillZeroMemsetSafe<_Ty*, _Ty> 
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

    UninitializedBackout<_Allocator_> _Backout { _First, _Allocator };
    for (; 0 < _Count; --_Count) {
        _Backout.EmplaceBack(_Value);
    }

    return _Backout.Release();
}

template <class _Allocator_>
// copy _Count copies of _Val to raw _First, using _Al
CONSTEXPR_CXX20 inline NODISCARD AllocatorPointerType<_Allocator_> UninitializedFill(
    AllocatorPointerType<_Allocator_>       _First,
    AllocatorPointerType<_Allocator_>       _Last,
    const AllocatorValueType<_Allocator_>&  _Value,
    _Allocator_&                            _Allocator)
{
    const auto _Count = IteratorsDifference(_First, _Last);
    return UninitializedFillCount(_First, _Count, _Value, _Allocator);
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

    if constexpr (std::_Fill_memset_is_safe<
        std::remove_cvref_t<
            const _NoThrowForwardIterator_&>,
        _Type_>) 
    {
        return FillMemset(
            _UFirst, _Value,
            static_cast<size_t>(_ULast - _UFirst));
    }

    if constexpr (std::_Fill_zero_memset_is_safe<
            std::remove_cvref_t<
                const _NoThrowForwardIterator_&>,
        _Type_>)
    {
        if (IsAllBitsZero(_Value)) {
            return MemsetZero(_UFirst, static_cast<size_t>(_ULast - _UFirst));
        }
    }
            
    UninitializedBackout<std::remove_cvref_t<const _NoThrowForwardIterator_&>> _Backout{_UFirst};

    while (_Backout._Last != _ULast)
        _Backout.EmplaceBack(_Value);

    _Backout.Release();
}


__BASE_MEMORY_NAMESPACE_END
