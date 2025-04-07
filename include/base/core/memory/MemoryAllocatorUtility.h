#pragma once 


#include <base/core/memory/Memory.h>

__BASE_MEMORY_NAMESPACE_BEGIN

template <class _Allocator_>
// 
_CONSTEXPR20 void POCCA(
    _Allocator_& _Left, 
    const _Allocator_& _Right) noexcept 
{
    if constexpr (std::allocator_traits<_Allocator_>::propagate_on_container_copy_assignment::value)
        _Left = _Right;
}

template <class _Allocator_>
_CONSTEXPR20 void POCMA(
    _Allocator_& _Left, 
    _Allocator_& _Right) noexcept 
{ 
    // (maybe) propagate on container move assignment
    if constexpr (std::allocator_traits<_Allocator_>::propagate_on_container_move_assignment::value)
        _Left = std::move(_Right);
}

template <class _Allocator_>
_CONSTEXPR20 void POCS(
    _Allocator_& _Left,
    _Allocator_& _Right) noexcept
{
    if constexpr (allocator_traits<_Allocator_>::propagate_on_container_swap::value)
        std::swap(_Left, _Right); // intentional ADL
    
    else {
        _STL_ASSERT(_Left == _Right, "containers incompatible for swap");
    }
}

__BASE_MEMORY_NAMESPACE_END