#pragma once 

#include <base/core/arch/Platform.h>
#include <src/core/utility/simd/traits/SimdFindTraits.h>

__BASE_NAMESPACE_BEGIN


BASE_DECLARE_CPU_FEATURE_GUARDED_CLASS(
    template <arch::CpuFeature feature> class CountTraits,
    feature,
    "base::algorithm",
    arch::CpuFeature::SSE2, arch::CpuFeature::AVX2, arch::CpuFeature::AVX512F
);


template <>
class CountTraits<arch::CpuFeature::SSE2>: public FindTraits<arch::CpuFeature::SSE2> {
public:
    template <size_t typeSize>
    base_declare_const_function base_always_inline static auto Substract(
        const __m128i left,
        const __m128i right) noexcept
    {
        static_assert(
            typeSize == 1 || typeSize == 2 || typeSize == 4 || typeSize == 8,
            "base::algorithm::CountTraits<arch::CpuFeature::SSE2>::Substract: Unsupported typeSize size");

        if      constexpr (typeSize == 1) return _mm_sub_epi8(left, right);
        else if constexpr (typeSize == 2) return _mm_sub_epi16(left, right);
        else if constexpr (typeSize == 4) return _mm_sub_epi32(left, right);
        else if constexpr (typeSize == 8) return _mm_sub_epi64(left, right);

        return {};
    }

    template <size_t typeSize>
    base_declare_const_function base_always_inline static size_t Reduce(const __m128i value) noexcept {
        static_assert(
            typeSize == 1 || typeSize == 2 || typeSize == 4 || typeSize == 8,
            "base::algorithm::CountTraits<arch::CpuFeature::SSE2>::Reduce: Unsupported typeSize size");

        if constexpr (typeSize == 8) {
#ifdef base_processor_x86_32
            return static_cast<uint32>(_mm_cvtsi128_si32(value)) + static_cast<uint32>(_mm_extract_epi32(value, 2));
#else
            return _mm_cvtsi128_si64(value) + _mm_extract_epi64(value, 1);
#endif // base_processor_x86_32
        }
        else if constexpr (typeSize == 4) {
            const __m128i reduceXmm4 = _mm_hadd_epi32(value, _mm_setzero_si128()); // (0+1),(2+3),0,0
            const __m128i reduceXmm5 = _mm_hadd_epi32(reduceXmm4, _mm_setzero_si128()); // (0+...+3),0,0,0

            return static_cast<uint32>(_mm_cvtsi128_si32(reduceXmm5));
        }
        else if constexpr (typeSize == 2) {
            const __m128i reduceXmm2 = _mm_hadd_epi16(value, _mm_setzero_si128());
            const __m128i reduceXmm3 = _mm_unpacklo_epi16(reduceXmm2, _mm_setzero_si128());

            const __m128i reduceXmm4 = _mm_hadd_epi32(reduceXmm3, _mm_setzero_si128()); // (0+1),(2+3),0,0
            const __m128i reduceXmm5 = _mm_hadd_epi32(reduceXmm4, _mm_setzero_si128()); // (0+...+3),0,0,0

            return static_cast<uint32>(_mm_cvtsi128_si32(reduceXmm5));
        }
        else if constexpr (typeSize == 1) {
            const __m128i reduceXmm1 = _mm_sad_epu8(value, _mm_setzero_si128());
#ifdef base_processor_x86_32
            return static_cast<uint32>(_mm_cvtsi128_si32(reduceXmm1)) + static_cast<uint32>(_mm_extract_epi32(reduceXmm1, 2));
#else
            return _mm_cvtsi128_si64(reduceXmm1) + _mm_extract_epi64(reduceXmm1, 1);
#endif // base_processor_x86_32
        }

        return -1;
    }
};

template <>
class CountTraits<arch::CpuFeature::AVX2>: public FindTraits<arch::CpuFeature::AVX2> {
public:
    template <size_t typeSize>
    base_declare_const_function base_always_inline static auto Substract(
        const __m256i left,
        const __m256i right) noexcept
    {
        static_assert(
            typeSize == 1 || typeSize == 2 || typeSize == 4 || typeSize == 8,
            "base::algorithm::CountTraits<arch::CpuFeature::AVX2>::Substract: Unsupported typeSize size");

        if      constexpr (typeSize == 1) return _mm256_sub_epi8(left, right);
        else if constexpr (typeSize == 2) return _mm256_sub_epi16(left, right);
        else if constexpr (typeSize == 4) return _mm256_sub_epi32(left, right);
        else if constexpr (typeSize == 8) return _mm256_sub_epi64(left, right);

        return {};
    }

    template <size_t typeSize>
    base_declare_const_function base_always_inline static size_t Reduce(const __m256i value) noexcept {
        static_assert(
            typeSize == 1 || typeSize == 2 || typeSize == 4 || typeSize == 8,
            "base::algorithm::CountTraits<arch::CpuFeature::AVX2>::Reduce: Unsupported typeSize size");

        constexpr auto shuffleMask = _MM_SHUFFLE(3, 1, 2, 0); // Cross lane, to reduce further on low lane

        if constexpr (typeSize == 8) {
            const __m128i low64     = _mm256_extracti128_si256(value, 0);
            const __m128i high64    = _mm256_extracti128_si256(value, 1);

            const __m128i reduceXmm = _mm_add_epi64(low64, high64);
            return CountTraits<arch::CpuFeature::SSE2>::Reduce(reduceXmm);
        }
        else if constexpr (typeSize == 4) {
            const __m256i reduceXmm4 = _mm256_hadd_epi32(value, _mm256_setzero_si256()); // (0+1),(2+3),0,0 per lane
            const __m256i reduceXmm5 = _mm256_permute4x64_epi64(reduceXmm4, shuffleMask); // low lane  (0+1),(2+3),(4+5),(6+7)

            const __m256i reduceXmm6 = _mm256_hadd_epi32(reduceXmm5, _mm256_setzero_si256()); // (0+...+3),(4+...+7),0,0
            const __m256i reduceXmm7 = _mm256_hadd_epi32(reduceXmm6, _mm256_setzero_si256()); // (0+...+7),0,0,0

            return static_cast<uint32>(_mm_cvtsi128_si32(_mm256_castsi256_si128(reduceXmm7)));
        }
        else if constexpr (typeSize == 2) {
            const __m256i reduceXmm2 = _mm256_hadd_epi16(value, _mm256_setzero_si256());
            const __m256i reduceXmm3 = _mm256_unpacklo_epi16(reduceXmm2, _mm256_setzero_si256());

            const __m256i reduceXmm4 = _mm256_hadd_epi32(reduceXmm3, _mm256_setzero_si256()); // (0+1),(2+3),0,0 per lane
            const __m256i reduceXmm5 = _mm256_permute4x64_epi64(reduceXmm4, shuffleMask); // low lane  (0+1),(2+3),(4+5),(6+7)

            const __m256i reduceXmm6 = _mm256_hadd_epi32(reduceXmm5, _mm256_setzero_si256()); // (0+...+3),(4+...+7),0,0
            const __m256i reduceXmm7 = _mm256_hadd_epi32(reduceXmm6, _mm256_setzero_si256()); // (0+...+7),0,0,0

            return static_cast<uint32>(_mm_cvtsi128_si32(_mm256_castsi256_si128(reduceXmm7)));
        }
        else if constexpr (typeSize == 1) {
            const __m256i reduceXmm1    = _mm256_sad_epu8(value, _mm256_setzero_si256());

            const __m128i low64         = _mm256_extracti128_si256(reduceXmm1, 0);
            const __m128i high64        = _mm256_extracti128_si256(reduceXmm1, 1);

            const __m128i reduceXmm8    = _mm_add_epi64(low64, high64);
            return CountTraits<arch::CpuFeature::SSE2>::Reduce(reduceXmm8);
        }

        return -1;
    }
};

template <>
class CountTraits<arch::CpuFeature::AVX512F>: public FindTraits<arch::CpuFeature::AVX512F> {
public:
    template <size_t typeSize>
    base_declare_const_function base_always_inline static auto Substract(
        const __m512i left,
        const __m512i right) noexcept
    {
        static_assert(
            typeSize == 1 || typeSize == 2 || typeSize == 4 || typeSize == 8,
            "base::algorithm::CountTraits<arch::CpuFeature::AVX2>::Substract: Unsupported typeSize size");

        if      constexpr (typeSize == 1) return _mm512_sub_epi8(left, right);
        else if constexpr (typeSize == 2) return _mm512_sub_epi16(left, right);
        else if constexpr (typeSize == 4) return _mm512_sub_epi32(left, right);
        else if constexpr (typeSize == 8) return _mm512_sub_epi64(left, right);

        return {};
    }

    template <size_t typeSize>
    base_declare_const_function base_always_inline static size_t Reduce(const __m512i value) noexcept {
        static_assert(
            typeSize == 1 || typeSize == 2 || typeSize == 4 || typeSize == 8,
            "base::algorithm::CountTraits<arch::CpuFeature::AVX2>::Reduce: Unsupported typeSize size");

        if constexpr (typeSize == 8) {
           
        }
        else if constexpr (typeSize == 4) {
         
        }
        else if constexpr (typeSize == 2) {
           
        }
        else if constexpr (typeSize == 1) {
        }

        return -1;
    }
};



__BASE_NAMESPACE_END
