#pragma once

#include <base/core/memory/MemoryTypeTraits.h>
#include <base/core/memory/MemoryPointerConversion.h>

__BASE_MEMORY_NAMESPACE_BEGIN

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
    _Type_&         _Obj, 
    _Types_&&...    _Args) noexcept(
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

__BASE_MEMORY_NAMESPACE_END