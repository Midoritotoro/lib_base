#pragma once 

#include <base/system/Platform.h>

#if __has_include(<bit>) && __cplusplus > 201703L
#include <bit>
#endif

#ifdef CPP_MSVC
#include <intrin.h>
#endif

namespace base {
    template <typename ForwardIterator>
    inline void DeleteAll(ForwardIterator begin, ForwardIterator end)
    {
        while (begin != end) {
            delete* begin;
            ++begin;
        }
    }

    template <typename Container>
    inline void DeleteAll(const Container& c)
    {
        DeleteAll(c.begin(), c.end());
    }

#if defined(__cpp_lib_bitops) && __cpp_lib_bitops >= 201907L
    // Использование битовых операций из C++20, что обеспечивает их constexpr выполнение
#  define HAS_CONSTEXPR_BITOPS
#elif defined(CC_GNU)
#  define HAS_CONSTEXPR_BITOPS
#  define HAS_BUILTIN_CTZS
    constexpr always_inline uint base_builtin_ctzs(uint16 v) noexcept
    {
#  if __has_builtin(__builtin_ctzs)
        return __builtin_ctzs(v);
#  else
        return __builtin_ctz(v);
#  endif
    }
#define HAS_BUILTIN_CLZS
    constexpr always_inline uint base_builtin_clzs(uint16 v) noexcept
    {
#  if __has_builtin(__builtin_clzs)
        return __builtin_clzs(v);
#  else
        return __builtin_clz(v) - 16U;
#  endif
    }
#define HAS_BUILTIN_CTZ
    constexpr always_inline uint base_builtin_ctz(uint32 v) noexcept
    {
        return __builtin_ctz(v);
    }
#define HAS_BUILTIN_CLZ
    constexpr always_inline uint base_builtin_clz(uint32 v) noexcept
    {
        return __builtin_clz(v);
    }
#define HAS_BUILTIN_CTZLL
    constexpr always_inline uint base_builtin_ctzll(uint64 v) noexcept
    {
        return __builtin_ctzll(v);
    }
#define HAS_BUILTIN_CLZLL
    constexpr always_inline uint base_builtin_clzll(uint64 v) noexcept
    {
        return __builtin_clzll(v);
    }
#define ALGORITHMS_USE_BUILTIN_POPCOUNT
    constexpr always_inline uint base_builtin_popcount(uint32 v) noexcept
    {
        return __builtin_popcount(v);
    }
    constexpr always_inline uint base_builtin_popcount(uint8 v) noexcept
    {
        return __builtin_popcount(v);
    }
    constexpr always_inline uint base_builtin_popcount(uint16 v) noexcept
    {
        return __builtin_popcount(v);
    }
#define ALGORITHMS_USE_BUILTIN_POPCOUNTLL
    constexpr always_inline uint base_builtin_popcountll(uint64 v) noexcept
    {
        return __builtin_popcountll(v);
    }
#elif defined(CC_MSVC) && !defined(PROCESSOR_ARM)
#define HAS_BUILTIN_CTZ
    always_inline unsigned long base_builtin_ctz(uint32 val)
    {
        unsigned long result;
        _BitScanForward(&result, val);
        return result;
    }
#define HAS_BUILTIN_CLZ
    always_inline unsigned long base_builtin_clz(uint32 val)
    {
        unsigned long result;
        _BitScanReverse(&result, val);
        // Now Invert the result: clz will count *down* from the msb to the lsb, so the msb index is 31
        // and the lsb index is 0. The result for the index when counting up: msb index is 0 (because it
        // starts there), and the lsb index is 31.
        result ^= sizeof(quint32) * 8 - 1;
        return result;
    }
#if PROCESSOR_WORDSIZE == 8
    // These are only defined for 64bit builds.
#define HAS_BUILTIN_CTZLL
    always_inline unsigned long base_builtin_ctzll(uint64 val)
    {
        unsigned long result;
        _BitScanForward64(&result, val);
        return result;
    }
    // MSVC calls it _BitScanReverse and returns the carry flag, which we don't need
#define HAS_BUILTIN_CLZLL
    always_inline unsigned long base_builtin_clzll(uint64 val)
    {
        unsigned long result;
        _BitScanReverse64(&result, val);
        // see qt_builtin_clz
        result ^= sizeof(quint64) * 8 - 1;
        return result;
    }
#endif // MSVC 64bit
#  define HAS_BUILTIN_CTZS
    always_inline uint base_builtin_ctzs(uint16 v) noexcept
    {
        return qt_builtin_ctz(v);
    }
#define HAS_BUILTIN_CLZS
    always_inline uint base_builtin_clzs(uint16 v) noexcept
    {
        return base_builtin_clz(v) - 16U;
    }

    // Neither MSVC nor the Intel compiler define a macro for the POPCNT processor
    // feature, so we're using either the SSE4.2 or the AVX macro as a proxy (Clang
    // does define the macro). It's incorrect for two reasons:
    // 1. It's a separate bit in CPUID, so a processor could implement SSE4.2 and
    //    not POPCNT, but that's unlikely to happen.
    // 2. There are processors that support POPCNT but not AVX (Intel Nehalem
    //    architecture), but unlike the other compilers, MSVC has no option
    //    to generate code for those processors.
    // So it's an acceptable compromise.
#if defined(__AVX__) || defined(__SSE4_2__) || defined(__POPCNT__)
#define POPCOUNT_CONSTEXPR
#define POPCOUNT_RELAXED_CONSTEXPR
#define AGORITHMS_USE_BUILTIN_POPCOUNT
#define ALGORITHMS_USE_BUILTIN_POPCOUNTLL
    always_inline uint base_builtin_popcount(uint32 v) noexcept
    {
        return __popcnt(v);
    }
    always_inline uint base_builtin_popcount(uint8 v) noexcept
    {
        return __popcnt16(v);
    }
    always_inline uint base_builtin_popcount(uint16 v) noexcept
    {
        return __popcnt16(v);
    }
    always_inline uint base_builtin_popcountll(uint64 v) noexcept
    {
#if PROCESSOR_WORDSIZE == 8
        return __popcnt64(v);
#else
        return __popcnt(uint32(v)) + __popcnt(uint32(v >> 32));
#endif // MSVC 64bit
    }

#endif // __AVX__ || __SSE4_2__ || __POPCNT__

#endif // MSVC

#ifndef POPCOUNT_CONSTEXPR
#define POPCOUNT_CONSTEXPR constexpr
#define POPCOUNT_RELAXED_CONSTEXPR constexpr
#endif


    DECL_CONST_FUNCTION POPCOUNT_CONSTEXPR inline uint PopulationCount(uint32 v) noexcept
    {
#if defined(__cpp_lib_bitops) && __cpp_lib_bitops >= 201907L
        return std::popcount(v);
#elif defined ALGORITHMS_USE_BUILTIN_POPCOUNT
        return base_builtin_popcount(v);
#else
        // http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
        return
            (((v) & 0xfff) * UINT64_C(0x1001001001001) & UINT64_C(0x84210842108421)) % 0x1f +
            (((v >> 12) & 0xfff) * UINT64_C(0x1001001001001) & UINT64_C(0x84210842108421)) % 0x1f +
            (((v >> 24) & 0xfff) * UINT64_C(0x1001001001001) & UINT64_C(0x84210842108421)) % 0x1f;
#endif
    }

    DECL_CONST_FUNCTION POPCOUNT_CONSTEXPR inline uint PopulationCount(uint8 v) noexcept
    {
#if defined(__cpp_lib_bitops) && __cpp_lib_bitops >= 201907L
        return std::popcount(v);
#elif defined ALGORITHMS_USE_BUILTIN_POPCOUNT
        return base_builtin_popcount(v);
#else
        return
            (((v) & 0xfff) * UINT64_C(0x1001001001001) & UINT64_C(0x84210842108421)) % 0x1f;
#endif
    }

    DECL_CONST_FUNCTION POPCOUNT_CONSTEXPR inline uint PopulationCount(uint16 v) noexcept
    {
#if defined(__cpp_lib_bitops) && __cpp_lib_bitops >= 201907L
        return std::popcount(v);
#elif defined ALGORITHMS_USE_BUILTIN_POPCOUNT
        return base_builtin_popcount(v);
#else
        return
            (((v) & 0xfff) * UINT64_C(0x1001001001001) & UINT64_C(0x84210842108421)) % 0x1f +
            (((v >> 12) & 0xfff) * UINT64_C(0x1001001001001) & UINT64_C(0x84210842108421)) % 0x1f;
#endif
    }

    DECL_CONST_FUNCTION POPCOUNT_CONSTEXPR inline uint PopulationCount(uint64 v) noexcept
    {
#if defined(__cpp_lib_bitops) && __cpp_lib_bitops >= 201907L
        return std::popcount(v);
#elif defined ALGORITHMS_USE_BUILTIN_POPCOUNTLL
        return base_builtin_popcountll(v);
#else
        return
            (((v) & 0xfff) * UINT64_C(0x1001001001001) & UINT64_C(0x84210842108421)) % 0x1f +
            (((v >> 12) & 0xfff) * UINT64_C(0x1001001001001) & UINT64_C(0x84210842108421)) % 0x1f +
            (((v >> 24) & 0xfff) * UINT64_C(0x1001001001001) & UINT64_C(0x84210842108421)) % 0x1f +
            (((v >> 36) & 0xfff) * UINT64_C(0x1001001001001) & UINT64_C(0x84210842108421)) % 0x1f +
            (((v >> 48) & 0xfff) * UINT64_C(0x1001001001001) & UINT64_C(0x84210842108421)) % 0x1f +
            (((v >> 60) & 0xfff) * UINT64_C(0x1001001001001) & UINT64_C(0x84210842108421)) % 0x1f;
#endif
    }

    DECL_CONST_FUNCTION POPCOUNT_CONSTEXPR inline uint PopulationCount(long unsigned int v) noexcept
    {
        return PopulationCount(static_cast<uint64>(v));
    }

#if defined(ALGORITHMS_USE_BUILTIN_POPCOUNT)
#undef ALGORITHMS_USE_BUILTIN_POPCOUNT
#endif
#undef POPCOUNT_CONSTEXPR

    constexpr inline uint ConstexprCountTrailingZeroBits(uint32 v) noexcept
    {
        // see http://graphics.stanford.edu/~seander/bithacks.html#ZerosOnRightParallel
        unsigned int c = 32; // c will be the number of zero bits on the right
        v &= -signed(v);
        if (v) c--;
        if (v & 0x0000FFFF) c -= 16;
        if (v & 0x00FF00FF) c -= 8;
        if (v & 0x0F0F0F0F) c -= 4;
        if (v & 0x33333333) c -= 2;
        if (v & 0x55555555) c -= 1;
        return c;
    }

    constexpr inline uint ConstexprCountTrailingZeroBits(uint64 v) noexcept
    {
        uint32 x = static_cast<uint32>(v);
        return x ? ConstexprCountTrailingZeroBits(x)
            : 32 + ConstexprCountTrailingZeroBits(static_cast<uint32>(v >> 32));
    }

    constexpr inline uint ConstexprCountTrailingZeroBits(uint8 v) noexcept
    {
        unsigned int c = 8; // c will be the number of zero bits on the right
        v &= uint8(-signed(v));
        if (v) c--;
        if (v & 0x0000000F) c -= 4;
        if (v & 0x00000033) c -= 2;
        if (v & 0x00000055) c -= 1;
        return c;
    }

    constexpr inline uint ConstexprCountTrailingZeroBits(uint16 v) noexcept
    {
        unsigned int c = 16; // c will be the number of zero bits on the right
        v &= quint16(-signed(v));
        if (v) c--;
        if (v & 0x000000FF) c -= 8;
        if (v & 0x00000F0F) c -= 4;
        if (v & 0x00003333) c -= 2;
        if (v & 0x00005555) c -= 1;
        return c;
    }

    constexpr inline uint ConstexprCountTrailingZeroBits(unsigned long v) noexcept
    {
        return ConstexprCountTrailingZeroBits(IntegerForSizeof<long>::Unsigned(v));
    }

    constexpr inline uint CountTrailingZeroBits(uint32 v) noexcept
    {
#if defined(__cpp_lib_bitops) && __cpp_lib_bitops >= 201907L
        return std::countr_zero(v);
#elif defined(HAS_BUILTIN_CTZ)
        return v ? base_builtin_ctz(v) : 32U;
#else
        return ConstexprCountTrailingZeroBits(v);
#endif
    }

    constexpr inline uint CountTrailingZeroBits(uint8 v) noexcept
    {
#if defined(__cpp_lib_bitops) && __cpp_lib_bitops >= 201907L
        return std::countr_zero(v);
#elif defined(HAS_BUILTIN_CTZ)
        return v ? base_builtin_ctz(v) : 8U;
#else
        return ConstexprCountTrailingZeroBits(v);
#endif
    }

    constexpr inline uint CountTrailingZeroBits(uint16 v) noexcept
    {
#if defined(__cpp_lib_bitops) && __cpp_lib_bitops >= 201907L
        return std::countr_zero(v);
#elif defined(QT_HAS_BUILTIN_CTZS)
        return v ? base_builtin_ctzs(v) : 16U;
#else
        return ConstexprCountTrailingZeroBits(v);
#endif
    }

    constexpr inline uint CountTrailingZeroBits(uint64 v) noexcept
    {
#if defined(__cpp_lib_bitops) && __cpp_lib_bitops >= 201907L
        return std::countr_zero(v);
#elif defined(HAS_BUILTIN_CTZLL)
        return v ? base_builtin_ctzll(v) : 64;
#else
        return ConstexprCountTrailingZeroBits(v);
#endif
    }

    constexpr inline uint CountTrailingZeroBits(unsigned long v) noexcept
    {
        return CountTrailingZeroBits(IntegerForSizeof<long>::Unsigned(v));
    }

    POPCOUNT_RELAXED_CONSTEXPR inline uint CountLeadingZeroBits(uint32 v) noexcept
    {
#if defined(__cpp_lib_bitops) && __cpp_lib_bitops >= 201907L
        return std::countl_zero(v);
#elif defined(HAS_BUILTIN_CLZ)
        return v ? base_builtin_clz(v) : 32U;
#else

        v = v | (v >> 1);
        v = v | (v >> 2);
        v = v | (v >> 4);
        v = v | (v >> 8);
        v = v | (v >> 16);
        return PopulationCount(~v);
#endif
    }

    POPCOUNT_RELAXED_CONSTEXPR inline uint CountLeadingZeroBits(uint8 v) noexcept
    {
#if defined(__cpp_lib_bitops) && __cpp_lib_bitops >= 201907L
        return std::countl_zero(v);
#elif defined(HAS_BUILTIN_CLZ)
        return v ? base_builtin_clz(v) - 24U : 8U;
#else
        v = v | (v >> 1);
        v = v | (v >> 2);
        v = v | (v >> 4);
        return PopulationCount(static_cast<uint8>(~v));
#endif
    }

    POPCOUNT_RELAXED_CONSTEXPR inline uint CountLeadingZeroBits(uint16 v) noexcept
    {
#if defined(__cpp_lib_bitops) && __cpp_lib_bitops >= 201907L
        return std::countl_zero(v);
#elif defined(HAS_BUILTIN_CLZS)
        return v ? base_builtin_clzs(v) : 16U;
#else

        v = v | (v >> 1);
        v = v | (v >> 2);
        v = v | (v >> 4);
        v = v | (v >> 8);

        return PopulationCount(static_cast<uint16>(~v));
#endif
    }

    POPCOUNT_RELAXED_CONSTEXPR inline uint CountLeadingZeroBits(quint64 v) noexcept
    {
#if defined(__cpp_lib_bitops) && __cpp_lib_bitops >= 201907L
        return std::countl_zero(v);
#elif defined(HAS_BUILTIN_CLZLL)
        return v ? base_builtin_clzll(v) : 64U;
#else
        v = v | (v >> 1);
        v = v | (v >> 2);
        v = v | (v >> 4);
        v = v | (v >> 8);
        v = v | (v >> 16);
        v = v | (v >> 32);

        return PopulationCount(~v);
#endif
    }

    POPCOUNT_RELAXED_CONSTEXPR inline uint CountLeadingZeroBits(unsigned long v) noexcept
    {
        return CountLeadingZeroBits(IntegerForSizeof<long>::Unsigned(v));
    }

#undef POPCOUNT_RELAXED_CONSTEXPR

}