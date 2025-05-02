#pragma once 

#include <base/core/utility/TypeTraits.h>

__BASE_MEMORY_NAMESPACE_BEGIN

NODISCARD bool IsAlignment(std::size_t value) noexcept {
    return (value > 0) && ((value & (value - 1)) == 0);
}

NODISCARD bool isAligned(
    const void* pointer,
    std::size_t alignment) noexcept
{
    Assert(IsAlignment(alignment));
    return (reinterpret_cast<std::size_t>(pointer) & (alignment - 1)) == 0;
}

NODISCARD bool isAligned(
    std::size_t alignment,
    const void* pointer) noexcept
{
    Assert(IsAlignment(alignment));
    return (reinterpret_cast<std::size_t>(pointer) & (alignment - 1)) == 0;
}

inline void* AlignDown(
    void*       pointer, 
    std::size_t alignment) noexcept
{
    Assert(IsAlignment(alignment));
    return reinterpret_cast<void*>(~(alignment - 1) &
        reinterpret_cast<std::size_t>(pointer));

}

inline void* AlignUp(
    void*       pointer,
    std::size_t alignment) noexcept
{
    Assert(IsAlignment(alignment));
    return reinterpret_cast<void*>(~(alignment - 1) &
        (reinterpret_cast<std::size_t>(pointer) + alignment - 1));
}

template <
    class _Allocator_,
    typename = void>
struct HasMemberAllocateAligned :
    std::false_type
{};

template <class _Allocator_>
struct HasMemberAllocateAligned<_Allocator_, std::void_t<decltype(
    std::declval<_Allocator_>().allocateAligned())>>
{};

template <typename _Allocator_>
inline constexpr bool HasMemberAllocateAligned = HasMemberAllocateAligned<_Allocator_>::value;

template <std::size_t N>
struct IsAlignmentConstant : 
    std::integral_constant<bool, (N > 0) && ((N & (N - 1)) == 0)> 
{};

__BASE_MEMORY_NAMESPACE_END
