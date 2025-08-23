#pragma once 

#include <base/core/arch/Platform.h>

__BASE_NAMESPACE_BEGIN

BASE_DECLARE_CPU_FEATURE_GUARDED_CLASS(
    template <arch::CpuFeature feature> class FindTraits,
    feature,
    "base::algorithm",
    arch::CpuFeature::SSE2, arch::CpuFeature::AVX2, arch::CpuFeature::AVX512F
);


template <>
class FindTraits<arch::CpuFeature::SSE2> {
public:
    base_declare_const_function base_always_inline static __m128i Set(const uint8 value) noexcept {
        return _mm_shuffle_epi8(_mm_cvtsi32_si128(value), _mm_setzero_si128());
    }

    base_declare_const_function base_always_inline static __m128i Set(const uint16 value) noexcept {
        return _mm_set1_epi16(value);
    }

    base_declare_const_function base_always_inline static __m128i Set(const uint32 value) noexcept {
        return _mm_set1_epi32(value);
    }

    base_declare_const_function base_always_inline static __m128i Set(const uint64 value) noexcept {
        return _mm_set1_epi64x(value);
    }

    template <size_t singleElementSize>
    base_declare_const_function base_always_inline static __m128i Compare(
        const __m128i left,
        const __m128i right) noexcept
    {
        static_assert(
            singleElementSize == 1 || singleElementSize == 2 || singleElementSize == 4 || singleElementSize == 8,
            "base::algorithm::FindTraits<arch::CpuFeature::AVX2>::Compare: Unsupported element size");

        if      constexpr (singleElementSize == 1) return _mm_cmpeq_epi8(left, right);
        else if constexpr (singleElementSize == 2) return _mm_cmpeq_epi16(left, right);
        else if constexpr (singleElementSize == 4) return _mm_cmpeq_epi32(left, right);
        else if constexpr (singleElementSize == 8) return _mm_cmpeq_epi64(left, right);

        return {};
    }
};

template <>
class FindTraits<arch::CpuFeature::AVX2> {
public:
    base_declare_const_function base_always_inline static __m256i Set(const uint8 value) noexcept {
        return _mm256_set1_epi8(value);
    }

    base_declare_const_function base_always_inline static __m256i Set(const uint16 value) noexcept {
        return _mm256_set1_epi16(value);
    }

    base_declare_const_function base_always_inline static __m256i Set(const uint32 value) noexcept {
        return _mm256_set1_epi32(value);
    }

    base_declare_const_function base_always_inline static __m256i Set(const uint64 value) noexcept {
        return _mm256_set1_epi64x(value);
    }

    template <size_t singleElementSize>
    base_declare_const_function base_always_inline static __m256i Compare(
        const __m256i left,
        const __m256i right) noexcept
    {
        static_assert(
            singleElementSize == 1 || singleElementSize == 2 || singleElementSize == 4 || singleElementSize == 8,
            "base::algorithm::FindTraits<arch::CpuFeature::AVX2>::Compare: Unsupported element size");

        if      constexpr (singleElementSize == 1) return _mm256_cmpeq_epi8(left, right);
        else if constexpr (singleElementSize == 2) return _mm256_cmpeq_epi16(left, right);
        else if constexpr (singleElementSize == 4) return _mm256_cmpeq_epi32(left, right);
        else if constexpr (singleElementSize == 8) return _mm256_cmpeq_epi64(left, right);

        return {};
    }
};

template <>
class FindTraits<arch::CpuFeature::AVX512F> {
public:
    base_declare_const_function base_always_inline static __m512i Set(const uint8 value) noexcept {
        return _mm512_set1_epi8(value);
    }

    base_declare_const_function base_always_inline static __m512i Set(const uint16 value) noexcept {
        return _mm512_set1_epi16(value);
    }

    base_declare_const_function base_always_inline static __m512i Set(const uint32 value) noexcept {
        return _mm512_set1_epi32(value);
    }

    base_declare_const_function base_always_inline static __m512i Set(const uint64 value) noexcept {
        return _mm512_set1_epi64(value);
    }

    template <size_t singleElementSize>
    base_declare_const_function base_always_inline static auto Compare(
        const __m512i left,
        const __m512i right) noexcept
    {
        static_assert(
            singleElementSize == 1 || singleElementSize == 2 || singleElementSize == 4 || singleElementSize == 8,
            "base::algorithm::FindTraits<arch::CpuFeature::AVX512F>::Compare: Unsupported element size");

        if      constexpr (singleElementSize == 1) return _mm512_cmpeq_epi8_mask(left, right);
        else if constexpr (singleElementSize == 2) return _mm512_cmpeq_epi16_mask(left, right);
        else if constexpr (singleElementSize == 4) return _mm512_cmpeq_epi32_mask(left, right);
        else if constexpr (singleElementSize == 8) return _mm512_cmpeq_epi64_mask(left, right);

        return {};
    }
};

__BASE_NAMESPACE_END