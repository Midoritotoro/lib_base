#pragma once 

#include <base/core/arch/Platform.h>
#include <base/core/BaseNamespace.h>

#if __has_include(<bit>) && __cplusplus > 201703L
    #include <bit>
#endif

#if defined(base_cpp_msvc)
    #include <intrin.h>
#endif

__BASE_NAMESPACE_BEGIN

#ifndef base_combine_u32ints_to_u64int
#  define base_combine_u32ints_to_u64int(high, low) ((uint64)high << 32) | (uint64)low
#endif

#ifndef base_combine_u16ints_to_u32int
#  define base_combine_u16ints_to_u32int(high, low) ((uint32)high << 16) | (uint32)low
#endif

#ifndef base_combine_u16ints_to_u64int
#  define base_combine_u16ints_to_u64int(high1, high2, low1, low2)	\
	base_combine_u32ints_to_u64int(									\
		base_combine_u16ints_to_u32int(high1, high2),				\
		base_combine_u16ints_to_u32int(low1, low2))
#endif

template <typename ForwardIterator>
inline void DeleteAll(
    ForwardIterator begin, 
    ForwardIterator end)
{
    while (begin != end) {
        delete* begin;
        ++begin;
    }
}

template <typename _Type_>
  _Type_ ClearLeftMostSet(const _Type_ value) {
    DebugAssert(value != _Type_{});
    return value & (value - 1);
}

template <typename Container>
inline void DeleteAll(const Container& c)
{
    DeleteAll(c.begin(), c.end());
}

constexpr uint PopulationCount(uint32 v) noexcept
{
    // http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
    return (((v) & 0xfff) * UINT64_CAST(0x1001001001001) & UINT64_CAST(0x84210842108421)) % 0x1f +
        (((v >> 12) & 0xfff) * UINT64_CAST(0x1001001001001) & UINT64_CAST(0x84210842108421)) % 0x1f +
        (((v >> 24) & 0xfff) * UINT64_CAST(0x1001001001001) & UINT64_CAST(0x84210842108421)) % 0x1f;
}

constexpr uint PopulationCount(uint8 v) noexcept
{
    return (((v) & 0xfff) * UINT64_CAST(0x1001001001001) & UINT64_CAST(0x84210842108421)) % 0x1f;
}

constexpr uint PopulationCount(uint16 v) noexcept
{
    return (((v) & 0xfff) * UINT64_CAST(0x1001001001001) & UINT64_CAST(0x84210842108421)) % 0x1f +
        (((v >> 12) & 0xfff) * UINT64_CAST(0x1001001001001) & UINT64_CAST(0x84210842108421)) % 0x1f;
}

constexpr uint32 PopulationCount(uint64 v) noexcept
{
    return (((v) & 0xfff) * UINT64_CAST(0x1001001001001) & UINT64_CAST(0x84210842108421)) % 0x1f +
        (((v >> 12) & 0xfff) * UINT64_CAST(0x1001001001001) & UINT64_CAST(0x84210842108421)) % 0x1f +
        (((v >> 24) & 0xfff) * UINT64_CAST(0x1001001001001) & UINT64_CAST(0x84210842108421)) % 0x1f +
        (((v >> 36) & 0xfff) * UINT64_CAST(0x1001001001001) & UINT64_CAST(0x84210842108421)) % 0x1f +
        (((v >> 48) & 0xfff) * UINT64_CAST(0x1001001001001) & UINT64_CAST(0x84210842108421)) % 0x1f +
        (((v >> 60) & 0xfff) * UINT64_CAST(0x1001001001001) & UINT64_CAST(0x84210842108421)) % 0x1f;
}

constexpr uint PopulationCount(long unsigned int v) noexcept
{
    return PopulationCount(static_cast<uint64>(v));
}

constexpr uint32 ConstexprCountTrailingZeroBits(uint32 v) noexcept
{
    // http://graphics.stanford.edu/~seander/bithacks.html#ZerosOnRightParallel
    unsigned int c = 32; // c - число нулевых битов справа

    v &= -signed(v);
    if (v) c--;

    if (v & 0x0000FFFF) c -= 16;
    if (v & 0x00FF00FF) c -= 8;
    if (v & 0x0F0F0F0F) c -= 4;

    if (v & 0x33333333) c -= 2;
    if (v & 0x55555555) c -= 1;

    return c;
}

constexpr uint32 ConstexprCountTrailingZeroBits(uint64 v) noexcept
{
    uint32 x = static_cast<uint32>(v);
    return x ? ConstexprCountTrailingZeroBits(x)
        : 32 + ConstexprCountTrailingZeroBits(static_cast<uint32>(v >> 32));
}

constexpr uint32 ConstexprCountTrailingZeroBits(uint8 v) noexcept
{
    unsigned int c = 8; // c - число нулевых битов справа

    v &= uint8(-signed(v));
    if (v) c--;

    if (v & 0x0000000F) c -= 4;
    if (v & 0x00000033) c -= 2;
    if (v & 0x00000055) c -= 1;

    return c;
}

constexpr uint32 ConstexprCountTrailingZeroBits(uint16 v) noexcept
{
    unsigned int c = 16; // c - число нулевых битов справа

    v &= uint16(-signed(v));
    if (v) c--;

    if (v & 0x000000FF) c -= 8;
    if (v & 0x00000F0F) c -= 4;

    if (v & 0x00003333) c -= 2;
    if (v & 0x00005555) c -= 1;

    return c;
}

constexpr inline uint32 ConstexprCountTrailingZeroBits(unsigned long v) noexcept
{
    return ConstexprCountTrailingZeroBits(IntegerForSizeof<long>::Unsigned(v));
}

constexpr inline uint32 CountTrailingZeroBits(uint32 v) noexcept
{
    return ConstexprCountTrailingZeroBits(v);
}

constexpr inline uint32 CountTrailingZeroBits(uint8 v) noexcept
{
    return ConstexprCountTrailingZeroBits(v);
}

constexpr inline uint32 CountTrailingZeroBits(uint16 v) noexcept
{
    return ConstexprCountTrailingZeroBits(v);
}

constexpr inline uint32 CountTrailingZeroBits(uint64 v) noexcept
{
    return ConstexprCountTrailingZeroBits(v);
}

constexpr inline uint32 CountTrailingZeroBits(unsigned long v) noexcept
{
    return CountTrailingZeroBits(IntegerForSizeof<long>::Unsigned(v));
}

constexpr uint32 CountLeadingZeroBits(uint32 v) noexcept
{
    v = v | (v >> 1);
    v = v | (v >> 2);
    v = v | (v >> 4);

    v = v | (v >> 8);
    v = v | (v >> 16);

    return PopulationCount(~v);
}

constexpr uint32 CountLeadingZeroBits(uint8 v) noexcept
{
    v = v | (v >> 1);
    v = v | (v >> 2);
    v = v | (v >> 4);

    return PopulationCount(static_cast<uint8>(~v));
}

constexpr uint32 CountLeadingZeroBits(uint16 v) noexcept
{
    v = v | (v >> 1);
    v = v | (v >> 2);

    v = v | (v >> 4);
    v = v | (v >> 8);

    return PopulationCount(static_cast<uint16>(~v));
}

constexpr uint32 CountLeadingZeroBits(uint64 v) noexcept
{
    v = v | (v >> 1);
    v = v | (v >> 2);
    v = v | (v >> 4);

    v = v | (v >> 8);
    v = v | (v >> 16);
    v = v | (v >> 32);

    return PopulationCount(~v);
}

constexpr uint32 CountLeadingZeroBits(unsigned long v) noexcept
{
    return CountLeadingZeroBits(IntegerForSizeof<long>::Unsigned(v));
}

static base_always_inline uint32 __BitScanReverse(unsigned v) noexcept
{
    uint32 result = CountLeadingZeroBits(v);

    result ^= sizeof(unsigned) * 8 - 1;
    return result;
}

__BASE_NAMESPACE_END