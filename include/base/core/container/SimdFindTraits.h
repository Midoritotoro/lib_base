#pragma once 

#include <base/core/arch/Platform.h>

__BASE_CONTAINER_NAMESPACE_BEGIN

struct FindTraits8Bit {
    static __m256i _Set_avx(const uint8_t _Val) noexcept {
        return _mm256_set1_epi8(_Val);
    }

    static __m128i _Set_sse(const uint8_t _Val) noexcept {
        return _mm_shuffle_epi8(_mm_cvtsi32_si128(_Val), _mm_setzero_si128());
    }

    static __m256i _Cmp_avx(const __m256i _Lhs, const __m256i _Rhs) noexcept {
        return _mm256_cmpeq_epi8(_Lhs, _Rhs);
    }

    static __m128i _Cmp_sse(const __m128i _Lhs, const __m128i _Rhs) noexcept {
        return _mm_cmpeq_epi8(_Lhs, _Rhs);
    }
};

struct FindTraits16Bit {
    static __m256i _Set_avx(const uint16_t _Val) noexcept {
        return _mm256_set1_epi16(_Val);
    }

    static __m128i _Set_sse(const uint16_t _Val) noexcept {
        return _mm_set1_epi16(_Val);
    }

    static __m256i _Cmp_avx(const __m256i _Lhs, const __m256i _Rhs) noexcept {
        return _mm256_cmpeq_epi16(_Lhs, _Rhs);
    }

    static __m128i _Cmp_sse(const __m128i _Lhs, const __m128i _Rhs) noexcept {
        return _mm_cmpeq_epi16(_Lhs, _Rhs);
    }
};

struct FindTraits32Bit {
    static __m256i _Set_avx(const uint32_t _Val) noexcept {
        return _mm256_set1_epi32(_Val);
    }

    static __m128i _Set_sse(const uint32_t _Val) noexcept {
        return _mm_set1_epi32(_Val);
    }

    static __m256i _Cmp_avx(const __m256i _Lhs, const __m256i _Rhs) noexcept {
        return _mm256_cmpeq_epi32(_Lhs, _Rhs);
    }

    static __m128i _Cmp_sse(const __m128i _Lhs, const __m128i _Rhs) noexcept {
        return _mm_cmpeq_epi32(_Lhs, _Rhs);
    }
};

struct FindTraits64Bit {
    static __m256i _Set_avx(const uint64_t _Val) noexcept {
        return _mm256_set1_epi64x(_Val);
    }

    static __m128i _Set_sse(const uint64_t _Val) noexcept {
        return _mm_set1_epi64x(_Val);
    }

    static __m256i _Cmp_avx(const __m256i _Lhs, const __m256i _Rhs) noexcept {
        return _mm256_cmpeq_epi64(_Lhs, _Rhs);
    }

    static __m128i _Cmp_sse(const __m128i _Lhs, const __m128i _Rhs) noexcept {
        return _mm_cmpeq_epi64(_Lhs, _Rhs);
    }
};


__BASE_CONTAINER_NAMESPACE_END