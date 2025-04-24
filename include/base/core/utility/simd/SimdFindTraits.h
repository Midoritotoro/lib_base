#pragma once 

#include <base/core/arch/Platform.h>

__BASE_CONTAINER_NAMESPACE_BEGIN

struct FindTraits8Bit {
    static NODISCARD __m512i SetAvx512(const uint8 _Val) noexcept {
        return _mm512_set1_epi8(_Val);
    }

    static NODISCARD __m256i SetAvx(const uint8 _Val) noexcept {
        return _mm256_set1_epi8(_Val);
    }

    static NODISCARD __m128i SetSse(const uint8 _Val) noexcept {
        return _mm_shuffle_epi8(_mm_cvtsi32_si128(_Val), _mm_setzero_si128());
    }

    static NODISCARD __mmask64 CompareAvx512(
        const __m512i _Lhs,
        const __m512i _Rhs) noexcept 
    {
        return _mm512_cmpeq_epi8_mask(_Lhs, _Rhs);
    }

    static NODISCARD __m256i CompareAvx(
        const __m256i _Lhs,
        const __m256i _Rhs) noexcept
    {
        return _mm256_cmpeq_epi8(_Lhs, _Rhs);
    }

    static __m128i CompareSse(
        const __m128i _Lhs,
        const __m128i _Rhs) noexcept 
    {
        return _mm_cmpeq_epi8(_Lhs, _Rhs);
    }
};

struct FindTraits16Bit {
    static NODISCARD __m512i SetAvx512(const uint16 _Val) noexcept {
        return _mm512_set1_epi16(_Val);
    }

    static NODISCARD __m256i SetAvx(const uint16 _Val) noexcept {
        return _mm256_set1_epi16(_Val);
    }

    static NODISCARD __m128i SetSse(const uint16 _Val) noexcept {
        return _mm_set1_epi16(_Val);
    }

    static NODISCARD __mmask64 CompareAvx512(
        const __m512i _Lhs,
        const __m512i _Rhs) noexcept
    {
        return _mm512_cmpeq_epi16_mask(_Lhs, _Rhs);
    }

    static NODISCARD __m256i CompareAvx(
        const __m256i _Lhs,
        const __m256i _Rhs) noexcept
    {
        return _mm256_cmpeq_epi16(_Lhs, _Rhs);
    }

    static NODISCARD __m128i CompareSse(
        const __m128i _Lhs, 
        const __m128i _Rhs) noexcept 
    {
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