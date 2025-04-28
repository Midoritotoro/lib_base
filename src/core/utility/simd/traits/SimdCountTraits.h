#pragma once 

#include <base/core/arch/Platform.h>
#include <src/core/utility/simd/traits/SimdFindTraits.h>

__BASE_NAMESPACE_BEGIN

class CountTraits64Bit : public FindTraits64Bit {
public:
    static NODISCARD __m512i SubstractAvx512(
        const __m512i left,
        const __m512i right) noexcept;
    static NODISCARD __m256i SubstractAvx(
        const __m256i left,
        const __m256i right) noexcept;
    static NODISCARD __m128i SubstractSse(
        const __m128i left,
        const __m128i right) noexcept;

    static NODISCARD std::size_t ReduceAvx512(const __m512i value) noexcept;
    static NODISCARD std::size_t ReduceAvx(const __m256i value) noexcept;
    static NODISCARD std::size_t ReduceSse(const __m128i value) noexcept;
};

class CountTraits32Bit : public FindTraits32Bit {
public:
    inline static constexpr size_t maxCount = 0x1FFF'FFFF;

    static NODISCARD __m512i SubstractAvx512(
        const __m512i left,
        const __m512i right) noexcept;
    static NODISCARD __m256i SubstractAvx(
        const __m256i left,
        const __m256i right) noexcept;
    static NODISCARD __m128i SubstractSse(
        const __m128i left,
        const __m128i right) noexcept;

    static NODISCARD std::size_t ReduceAvx512(const __m512i value) noexcept;
    static NODISCARD std::size_t ReduceAvx(const __m256i value) noexcept;
    static NODISCARD std::size_t ReduceSse(const __m128i value) noexcept;
};

class CountTraits16Bit : public FindTraits16Bit {
public:
    // For both AVX2 and SSE4.2, we use hadd_epi16 once to combine pairs of 16-bit counters into 16-bit results.
    // Therefore, _Max_count is 0x7FFF, which is 0xFFFE when doubled; any more would overflow.

    static constexpr size_t maxCount = 0x7FFF;

    static NODISCARD __m512i SubstractAvx512(
        const __m512i left,
        const __m512i right) noexcept;
    static NODISCARD __m256i SubstractAvx(
        const __m256i left,
        const __m256i right) noexcept;
    static NODISCARD __m128i SubstractSse(
        const __m128i left,
        const __m128i right) noexcept;

    static NODISCARD std::size_t ReduceAvx512(const __m512i value) noexcept;
    static NODISCARD std::size_t ReduceAvx(const __m256i value) noexcept;
    static NODISCARD std::size_t ReduceSse(const __m128i value) noexcept;
};

class CountTraits8Bit : public FindTraits8Bit {
public:
    // For AVX2, _Max_portion_size below is _Max_count * 32 bytes, and we have 1-byte elements.
    // We're using packed 8-bit counters, and 32 of those fit in 256 bits.

    // For SSE4.2, _Max_portion_size below is _Max_count * 16 bytes, and we have 1-byte elements.
    // We're using packed 8-bit counters, and 16 of those fit in 128 bits.

    // For both codepaths, this is why _Max_count is the maximum unsigned 8-bit integer.
    // (The reduction steps aren't the limiting factor here.)

    static constexpr size_t maxCount = 0xFF;

    static NODISCARD __m512i SubstractAvx512(
        const __m512i left,
        const __m512i right) noexcept;
    static NODISCARD __m256i SubstractAvx(
        const __m256i left,
        const __m256i right) noexcept;
    static NODISCARD __m128i SubstractSse(
        const __m128i left,
        const __m128i right) noexcept;

    static NODISCARD std::size_t ReduceAvx512(const __m512i value) noexcept;
    static NODISCARD std::size_t ReduceAvx(const __m256i value) noexcept;
    static NODISCARD std::size_t ReduceSse(const __m128i value) noexcept;
};

__BASE_NAMESPACE_END
