#pragma once 

#include <base/core/utility/TypeTraits.h>
#include <memory>

__BASE_MEMORY_NAMESPACE_BEGIN

template <class _Allocator_>
using AllocatorPointerType      = typename std::allocator_traits<_Allocator_>::pointer;

template <class _Allocator_>
using AllocatorConstPointerType = typename std::allocator_traits<_Allocator_>::const_pointer;

template <class _Allocator_>
using AllocatorSizeType         = typename std::allocator_traits<_Allocator_>::size_type;

template <class _Allocator_>
using AllocatorValueType        = typename std::allocator_traits<_Allocator_>::value_type;

template <
    class _Allocator_,
    class _ValueType_>
using RebindAllocator = 
    typename std::allocator_traits<_Allocator_>
        ::template rebind_alloc<_ValueType_>;

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
base_constexpr_cxx20 inline void POCCA(
    _Allocator_&        left, 
    const _Allocator_&  right) noexcept 
{
    if constexpr (std::allocator_traits<_Allocator_>::propagate_on_container_copy_assignment::value)
        left = right;
}

template <class _Allocator_>
base_constexpr_cxx20 inline void POCMA(
    _Allocator_& left,
    _Allocator_& right) noexcept
{ 
    // (maybe) propagate on container move assignment
    if constexpr (std::allocator_traits<_Allocator_>::propagate_on_container_move_assignment::value)
        left = std::move(right);
}

template <class _Allocator_>
base_constexpr_cxx20 inline void POCS(
    _Allocator_& left,
    _Allocator_& right) noexcept
{
    if constexpr (std::allocator_traits<_Allocator_>::propagate_on_container_swap::value)
        std::swap(left, right); // intentional ADL
    else
        AssertLog(left == right, "base::memory::POCS: Containers incompatible for swap");
}

__BASE_MEMORY_NAMESPACE_END