#pragma once 


#include <base/core/memory/Memory.h>

__BASE_MEMORY_NAMESPACE_BEGIN


enum class PocmaValues {
    EqualAllocators, // usually allows contents to be stolen (e.g. with swap)
    PropagateAllocators, // usually allows the allocator to be propagated, and then contents stolen
    NoPropagateAllocators, // usually turns moves into copies
};


template <class _Allocator_>
constexpr bool ChoosePocca_v = 
    std::allocator_traits<_Allocator_>::propagate_on_container_copy_assignment::value
    && !std::allocator_traits<_Allocator_>::is_always_equal::value;

template <class _Allocator_>
constexpr PocmaValues ChoosePocma_v = 
    std::allocator_traits<_Allocator_>::is_always_equal::value
        ? PocmaValues::EqualAllocators
        : (std::allocator_traits<_Allocator_>::propagate_on_container_move_assignment::value
                ? PocmaValues::PropagateAllocators
                : PocmaValues::NoPropagateAllocators);


template <class _Allocator_>
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
    if constexpr (std::allocator_traits<_Allocator_>::propagate_on_container_swap::value)
        std::swap(_Left, _Right); // intentional ADL
    else
        AssertLog(_Left == _Right, "base::memory::POCS: Containers incompatible for swap");
}

__BASE_MEMORY_NAMESPACE_END