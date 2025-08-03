#pragma once 

#include <base/core/Types.h>
#include <base/core/utility/MathConstants.h>

#include <base/core/utility/Assert.h>
#include <base/core/utility/BitOps.h>

#include <base/core/arch/KeywordSupport.h>

#include <cmath>
#include <numeric>


__BASE_NAMESPACE_BEGIN

struct rational_t {
    unsigned num;
    unsigned den;
};

NODISCARD int64_t GCD(
	int64_t a,
	int64_t b);

NODISCARD int64_t LCM(
	int64_t a,
	int64_t b);

bool UnsignedReduce(
	unsigned* pi_dst_nom, unsigned* pi_dst_den,
	uint64_t i_nom, uint64_t i_den, uint64_t i_max);

NODISCARD double SafeRound(double value);

inline NODISCARD 
    double FastSin(double x)
{
    int si = int(x * (0.5 * SINE_TABLE_SIZE / M_PI));
    double d = x - si * (2.0 * M_PI / SINE_TABLE_SIZE);

    int ci = si + SINE_TABLE_SIZE / 4;

    si &= SINE_TABLE_SIZE - 1;
    ci &= SINE_TABLE_SIZE - 1;

    return sineTable[si] +
        (sineTable[ci] - 0.5 * sineTable[si] * d) * d;
}

inline NODISCARD
    double FastCos(double x)
{
    int ci = int(x * (0.5 * SINE_TABLE_SIZE / M_PI));

    double d = x - ci * (2.0 * M_PI / SINE_TABLE_SIZE);
    int si = ci + SINE_TABLE_SIZE / 4;

    si &= SINE_TABLE_SIZE - 1;
    ci &= SINE_TABLE_SIZE - 1;

    return sineTable[si] - 
        (sineTable[ci] + 0.5 * sineTable[si] * d) * d;
}

constexpr inline NODISCARD
    float DegreesToRadians(float degrees)
{
    return degrees * float(M_PI / 180);
}

constexpr inline NODISCARD
    double DegreesToRadians(double degrees)
{
    return degrees * (M_PI / 180);
}

constexpr inline NODISCARD 
    long double DegreesToRadians(long double degrees)
{
    return degrees * (M_PI / 180);
}

template <typename T, 
    std::enable_if_t<std::is_integral_v<T>, bool> = true>
constexpr inline NODISCARD 
    double DegreesToRadians(T degrees)
{
    return DegreesToRadians(static_cast<double>(degrees));
}

constexpr inline NODISCARD 
    float RadiansToDegrees(float radians)
{
    return radians * float(180 / M_PI);
}

constexpr inline NODISCARD
    double RadiansToDegrees(double radians)
{
    return radians * (180 / M_PI);
}

constexpr inline NODISCARD
    long double RadiansToDegrees(long double radians)
{
    return radians * (180 / M_PI);
}

constexpr inline NODISCARD 
    uint32 ConstexprNextPowerOfTwo(uint32 v)
{
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;

    v |= v >> 8;
    v |= v >> 16;

    ++v;

    return v;
}

constexpr inline NODISCARD 
    uint64 ConstexprNextPowerOfTwo(uint64 v)
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

constexpr inline NODISCARD 
    uint32 ConstexprNextPowerOfTwo(int32 v)
{
    return ConstexprNextPowerOfTwo(uint32(v));
}

constexpr inline NODISCARD 
    uint64 ConstexprNextPowerOfTwo(int64 v)
{
    return ConstexprNextPowerOfTwo(uint64(v));
}

constexpr inline NODISCARD 
    uint32 NextPowerOfTwo(uint32 v)
{
    Assert(static_cast<int32>(v) >= 0); // There is a next power of two
#if defined(__cpp_lib_int_pow2) && __cpp_lib_int_pow2 >= 202002L
    return std::bit_ceil(v + 1);
#elif defined(HAS_BUILTIN_CLZ)
    if (v == 0)
        return 1;
    return 2U << (31 ^ base_builtin_clz(v));
#else
    return ConstexprNextPowerOfTwo(v);
#endif
}

constexpr inline NODISCARD
    uint64 NextPowerOfTwo(uint64 v)
{
    Assert(static_cast<int64>(v) >= 0); // There is a next power of two
#if defined(__cpp_lib_int_pow2) && __cpp_lib_int_pow2 >= 202002L
    return std::bit_ceil(v + 1);
#elif defined(HAS_BUILTIN_CLZLL)
    if (v == 0)
        return 1;
    return UINT64_CAST(2) << (63 ^ base_builtin_clzll(v));
#else
    return ConstexprNextPowerOfTwo(v);
#endif
}

constexpr inline NODISCARD 
    uint32 NextPowerOfTwo(int32 v) 
{
    return NextPowerOfTwo(uint32(v));
}

constexpr inline NODISCARD
    uint64 NextPowerOfTwo(int64 v)
{
    return NextPowerOfTwo(uint64(v));
}

constexpr inline NODISCARD 
    ulong NextPowerOfTwo(ulong v)
{
    return NextPowerOfTwo(IntegerForSizeof<long>::Unsigned(v));
}

constexpr inline NODISCARD 
    ulong NextPowerOfTwo(long v)
{
    return NextPowerOfTwo(IntegerForSizeof<long>::Unsigned(v));
}

constexpr inline NODISCARD
    float FastInverseSqrt(float x) 
{
    int i = *(int*)&x;
    i = 0x5f3759df - (i >> 1);

    x = *(float*)&i;
    x = x * (1.5f - (0.5f * x * x * x));

    return x;
}

template<typename T, typename Cmp = std::less<>>
NODISCARD constexpr bool points_into_range(
    const T * p,
    const T * b, 
    const T * e,
    Cmp less = {}) noexcept
{
    return !less(p, b) && less(p, e);
}

template <typename C, typename T>
NODISCARD constexpr bool points_into_range(
    const T& p,
    const C& c) noexcept
{
    static_assert(std::is_same_v<decltype(std::data(c)), T>);
    return points_into_range(p, std::data(c),
        std::data(c) + std::distance(std::begin(c), std::end(c)));
}

always_inline NODISCARD uint64 BytesToDoubleWordsCount(const uint64 bytes) noexcept {
    return (bytes >> 2);
}

always_inline NODISCARD uint64 BytesToQuadWordsCount(const uint64 bytes) noexcept {
    return (bytes >> 3);
}

always_inline NODISCARD uint64 BytesToXmmWordsCount(const uint64 bytes) noexcept {
    return (bytes >> 4);
}

always_inline NODISCARD uint64 BytesToYmmWordsCount(const uint64 bytes) noexcept {
    return (bytes >> 5);
}

always_inline NODISCARD uint64 BytesToZmmWordsCount(const uint64 bytes) noexcept {
    return (bytes >> 6);
}


template <typename To, typename From>
constexpr auto saturate(From x)
{
    static_assert(std::is_integral_v<To>);
    static_assert(std::is_integral_v<From>);

    MAYBE_UNUSED_ATTRIBUTE
    constexpr auto Lo = (std::numeric_limits<To>::min)();
    constexpr auto Hi = (std::numeric_limits<To>::max)();

    if constexpr (std::is_signed_v<From> == std::is_signed_v<To>) {
        return x < Lo ? Lo :
            x > Hi ? Hi :
            To(x);
    }
    else {
        if constexpr (std::is_signed_v<From>) {
            if (x < From{ 0 })
                return To{ 0 };
        }

        using FromU = std::make_unsigned_t<From>;
        using ToU = std::make_unsigned_t<To>;

        return FromU(x) > ToU(Hi) ? Hi : To(x);
    }
}

__BASE_NAMESPACE_END
