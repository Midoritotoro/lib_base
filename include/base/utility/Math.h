#pragma once 

#include <base/core/Types.h>
#include <base/utility/MathConstants.h>

#include <base/utility/Assert.h>
#include <cmath>


namespace base {
	struct rational_t {
		unsigned num, den;
	};

	[[nodiscard]] int64_t GCD(
		int64_t a,
		int64_t b);

	[[nodiscard]] int64_t LCM(
		int64_t a,
		int64_t b);

	bool UnsignedReduce(
		unsigned* pi_dst_nom, unsigned* pi_dst_den,
		uint64_t i_nom, uint64_t i_den, uint64_t i_max);

	[[nodiscard]] double SafeRound(double value);

    inline [[nodiscard]] double FastSin(double x)
    {
        int si = int(x * (0.5 * SINE_TABLE_SIZE / M_PI));
        double d = x - si * (2.0 * M_PI / SINE_TABLE_SIZE);

        int ci = si + SINE_TABLE_SIZE / 4;

        si &= SINE_TABLE_SIZE - 1;
        ci &= SINE_TABLE_SIZE - 1;

        return sineTable[si] + (sineTable[ci] - 0.5 * sineTable[si] * d) * d;
    }

    inline [[nodiscard]] double FastCos(double x)
    {
        int ci = int(x * (0.5 * SINE_TABLE_SIZE / M_PI));

        double d = x - ci * (2.0 * M_PI / SINE_TABLE_SIZE);
        int si = ci + SINE_TABLE_SIZE / 4;

        si &= SINE_TABLE_SIZE - 1;
        ci &= SINE_TABLE_SIZE - 1;

        return sineTable[si] - (sineTable[ci] + 0.5 * sineTable[si] * d) * d;
    }

    constexpr inline [[nodiscard]] float DegreesToRadians(float degrees)
    {
        return degrees * float(M_PI / 180);
    }

    constexpr inline [[nodiscard]] double DegreesToRadians(double degrees)
    {
        return degrees * (M_PI / 180);
    }

    constexpr inline [[nodiscard]] long double DegreesToRadians(long double degrees)
    {
        return degrees * (M_PI / 180);
    }

    template <typename T, 
        std::enable_if_t<std::is_integral_v<T>, bool> = true>
    constexpr inline [[nodiscard]] double DegreesToRadians(T degrees)
    {
        return DegreesToRadians(static_cast<double>(degrees));
    }

    constexpr inline [[nodiscard]] float RadiansToDegrees(float radians)
    {
        return radians * float(180 / M_PI);
    }

    constexpr inline [[nodiscard]] double RadiansToDegrees(double radians)
    {
        return radians * (180 / M_PI);
    }

    constexpr inline [[nodiscard]] long double RadiansToDegrees(long double radians)
    {
        return radians * (180 / M_PI);
    }

    constexpr inline [[nodiscard]] uint32 qConstexprNextPowerOfTwo(uint32 v)
    {
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
        ++v;
        return v;
    }

    constexpr inline [[nodiscard]] uint64 qConstexprNextPowerOfTwo(uint64 v)
    {
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;

        v |= v >> 8;
        v |= v >> 16;
        v |= v >> 32;

        ++v;

        return v;
    }

    constexpr inline [[nodiscard]] uint32 ConstexprNextPowerOfTwo(int32 v)
    {
        return ConstexprNextPowerOfTwo(uint32(v));
    }

    constexpr inline [[nodiscard]] uint64 ConstexprNextPowerOfTwo(int64 v)
    {
        return ConstexprNextPowerOfTwo(uint64(v));
    }

    constexpr inline [[nodiscard]] uint32 NextPowerOfTwo(uint32 v)
    {
        Assert(static_cast<int32>(v) >= 0); // There is a next power of two
#if defined(__cpp_lib_int_pow2) && __cpp_lib_int_pow2 >= 202002L
        return std::bit_ceil(v + 1);
#elif defined(QT_HAS_BUILTIN_CLZ)
        if (v == 0)
            return 1;
        return 2U << (31 ^ base_builtin_clz(v));
#else
        return ConstexprNextPowerOfTwo(v);
#endif
    }

    constexpr inline [[nodiscard]] uint64 NextPowerOfTwo(uint64 v)
    {
        Assert(static_cast<int64>(v) >= 0); // There is a next power of two
#if defined(__cpp_lib_int_pow2) && __cpp_lib_int_pow2 >= 202002L
        return std::bit_ceil(v + 1);
#elif defined(QT_HAS_BUILTIN_CLZLL)
        if (v == 0)
            return 1;
        return UINT64_CAST(2) << (63 ^ base_builtin_clzll(v));
#else
        return ConstexprNextPowerOfTwo(v);
#endif
    }

    constexpr inline [[nodiscard]] uint32 NextPowerOfTwo(int32 v)
    {
        return NextPowerOfTwo(uint32(v));
    }

    constexpr inline [[nodiscard]] uint64 NextPowerOfTwo(int64 v)
    {
        return NextPowerOfTwo(uint64(v));
    }

    constexpr inline [[nodiscard]] ulong NextPowerOfTwo(ulong v)
    {
        return NextPowerOfTwo(IntegerForSizeof<long>::Unsigned(v));
    }

    constexpr inline [[nodiscard]] ulong NextPowerOfTwo(long v)
    {
        return NextPowerOfTwo(IntegerForSizeof<long>::Unsigned(v));
    }
} // namespace base