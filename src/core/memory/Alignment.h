#pragma once 

#include <base/core/utility/TypeTraits.h>

__BASE_MEMORY_NAMESPACE_BEGIN

#if defined(CPP_GNU)
#  if CPP_GNU > 430
#    define ALLOC_SIZE(...) __attribute__((alloc_size(__VA_ARGS__)))
#  else
#    define ALLOC_SIZE(...)
#  endif
#  else 
#    define ALLOC_SIZE(...) 
#endif

#if defined(OS_WIN) && !defined(aligned_malloc)
#  define aligned_malloc		                _aligned_malloc
#elif !defined(aligned_malloc)
#  define aligned_malloc(size, alignment)		malloc(size)
#endif

#if defined(OS_WIN) && !defined(aligned_realloc)
#  define aligned_realloc                       _aligned_realloc
#elif !defined(aligned_realloc)
#  define aligned_realloc(block, size, align)   realloc(block, size)
#endif

#if defined (OS_WIN) && !defined(aligned_free)
#  define aligned_free(ptr)                     _aligned_free(ptr)
#elif !defined(aligned_free)
#  define aligned_free(ptr)                     free(ptr)
#endif

#ifndef MEMORY_DEFAULT_ALIGNMENT
#  define MEMORY_DEFAULT_ALIGNMENT MINIMUM_ACCEPTABLE_SIMD_ALIGNMENT
#endif

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

template <std::size_t N>
struct IsAlignmentConstant : 
    std::integral_constant<bool, (N > 0) && ((N & (N - 1)) == 0)> 
{};

__BASE_MEMORY_NAMESPACE_END
