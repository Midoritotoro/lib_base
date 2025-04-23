#pragma once 

#include <base/core/arch/Platform.h>
#include <base/core/container/SimdFindTraits.h>

__BASE_CONTAINER_NAMESPACE_BEGIN

struct CountTraits64Bit : FindTraits64Bit {
    static __m256i _Sub_avx(const __m256i _Lhs, const __m256i _Rhs) noexcept {
        return _mm256_sub_epi64(_Lhs, _Rhs);
    }

    static __m128i _Sub_sse(const __m128i _Lhs, const __m128i _Rhs) noexcept {
        return _mm_sub_epi64(_Lhs, _Rhs);
    }

    static size_t _Reduce_avx(const __m256i _Val) noexcept {
        const __m128i _Lo64 = _mm256_extracti128_si256(_Val, 0);
        const __m128i _Hi64 = _mm256_extracti128_si256(_Val, 1);
        const __m128i _Rx8 = _mm_add_epi64(_Lo64, _Hi64);
        return _Reduce_sse(_Rx8);
    }

    static size_t _Reduce_sse(const __m128i _Val) noexcept {
#ifdef _M_IX86
        return static_cast<uint32_t>(_mm_cvtsi128_si32(_Val)) + static_cast<uint32_t>(_mm_extract_epi32(_Val, 2));
#else // ^^^ defined(_M_IX86) / defined(_M_X64) vvv
        return _mm_cvtsi128_si64(_Val) + _mm_extract_epi64(_Val, 1);
#endif // ^^^ defined(_M_X64) ^^^
    }
};

struct CountTraits32Bit : FindTraits32Bit {
    static constexpr size_t _Max_count = 0x1FFF'FFFF;

    static __m256i _Sub_avx(const __m256i _Lhs, const __m256i _Rhs) noexcept {
        return _mm256_sub_epi32(_Lhs, _Rhs);
    }

    static __m128i _Sub_sse(const __m128i _Lhs, const __m128i _Rhs) noexcept {
        return _mm_sub_epi32(_Lhs, _Rhs);
    }

    static size_t _Reduce_avx(const __m256i _Val) noexcept {
        constexpr auto _Shuf = _MM_SHUFFLE(3, 1, 2, 0); // Cross lane, to reduce further on low lane
        const __m256i _Rx4 = _mm256_hadd_epi32(_Val, _mm256_setzero_si256()); // (0+1),(2+3),0,0 per lane
        const __m256i _Rx5 = _mm256_permute4x64_epi64(_Rx4, _Shuf); // low lane  (0+1),(2+3),(4+5),(6+7)
        const __m256i _Rx6 = _mm256_hadd_epi32(_Rx5, _mm256_setzero_si256()); // (0+...+3),(4+...+7),0,0
        const __m256i _Rx7 = _mm256_hadd_epi32(_Rx6, _mm256_setzero_si256()); // (0+...+7),0,0,0
        return static_cast<uint32_t>(_mm_cvtsi128_si32(_mm256_castsi256_si128(_Rx7)));
    }

    static size_t _Reduce_sse(const __m128i _Val) noexcept {
        const __m128i _Rx4 = _mm_hadd_epi32(_Val, _mm_setzero_si128()); // (0+1),(2+3),0,0
        const __m128i _Rx5 = _mm_hadd_epi32(_Rx4, _mm_setzero_si128()); // (0+...+3),0,0,0
        return static_cast<uint32_t>(_mm_cvtsi128_si32(_Rx5));
    }
};

struct CountTraits16Bit : FindTraits16Bit {
    // For both AVX2 and SSE4.2, we use hadd_epi16 once to combine pairs of 16-bit counters into 16-bit results.
    // Therefore, _Max_count is 0x7FFF, which is 0xFFFE when doubled; any more would overflow.

    static constexpr size_t _Max_count = 0x7FFF;

    static __m256i _Sub_avx(const __m256i _Lhs, const __m256i _Rhs) noexcept {
        return _mm256_sub_epi16(_Lhs, _Rhs);
    }

    static __m128i _Sub_sse(const __m128i _Lhs, const __m128i _Rhs) noexcept {
        return _mm_sub_epi16(_Lhs, _Rhs);
    }

    static size_t _Reduce_avx(const __m256i _Val) noexcept {
        const __m256i _Rx2 = _mm256_hadd_epi16(_Val, _mm256_setzero_si256());
        const __m256i _Rx3 = _mm256_unpacklo_epi16(_Rx2, _mm256_setzero_si256());
        return CountTraits32Bit::_Reduce_avx(_Rx3);
    }

    static size_t _Reduce_sse(const __m128i _Val) noexcept {
        const __m128i _Rx2 = _mm_hadd_epi16(_Val, _mm_setzero_si128());
        const __m128i _Rx3 = _mm_unpacklo_epi16(_Rx2, _mm_setzero_si128());
        return CountTraits32Bit::_Reduce_sse(_Rx3);
    }
};

struct CountTraits8Bit : FindTraits8Bit {
    // For AVX2, _Max_portion_size below is _Max_count * 32 bytes, and we have 1-byte elements.
    // We're using packed 8-bit counters, and 32 of those fit in 256 bits.

    // For SSE4.2, _Max_portion_size below is _Max_count * 16 bytes, and we have 1-byte elements.
    // We're using packed 8-bit counters, and 16 of those fit in 128 bits.

    // For both codepaths, this is why _Max_count is the maximum unsigned 8-bit integer.
    // (The reduction steps aren't the limiting factor here.)

    static constexpr size_t _Max_count = 0xFF;

    static __m256i _Sub_avx(const __m256i _Lhs, const __m256i _Rhs) noexcept {
        return _mm256_sub_epi8(_Lhs, _Rhs);
    }

    static __m128i _Sub_sse(const __m128i _Lhs, const __m128i _Rhs) noexcept {
        return _mm_sub_epi8(_Lhs, _Rhs);
    }

    static size_t _Reduce_avx(const __m256i _Val) noexcept {
        const __m256i _Rx1 = _mm256_sad_epu8(_Val, _mm256_setzero_si256());
        return CountTraits64Bit::_Reduce_avx(_Rx1);
    }

    static size_t _Reduce_sse(const __m128i _Val) noexcept {
        const __m128i _Rx1 = _mm_sad_epu8(_Val, _mm_setzero_si128());
        return CountTraits64Bit::_Reduce_sse(_Rx1);
    }
};


__BASE_CONTAINER_NAMESPACE_END