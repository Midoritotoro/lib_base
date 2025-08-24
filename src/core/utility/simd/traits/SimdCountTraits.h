#pragma once 

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
            const __m128i reduce4 = _mm_hadd_epi32(value, _mm_setzero_si128()); // (0+1),(2+3),0,0
            const __m128i reduce5 = _mm_hadd_epi32(reduce4, _mm_setzero_si128()); // (0+...+3),0,0,0

            return static_cast<uint32>(_mm_cvtsi128_si32(reduce5));
        }
        else if constexpr (typeSize == 2) {
            const __m128i reduce2 = _mm_hadd_epi16(value, _mm_setzero_si128());
            const __m128i reduce3 = _mm_unpacklo_epi16(reduce2, _mm_setzero_si128());

            const __m128i reduce4 = _mm_hadd_epi32(reduce3, _mm_setzero_si128()); // (0+1),(2+3),0,0
            const __m128i reduce5 = _mm_hadd_epi32(reduce4, _mm_setzero_si128()); // (0+...+3),0,0,0

            return static_cast<uint32>(_mm_cvtsi128_si32(reduce5));
        }
        else if constexpr (typeSize == 1) {
            const __m128i reduce1 = _mm_sad_epu8(value, _mm_setzero_si128());
#ifdef base_processor_x86_32
            return static_cast<uint32>(_mm_cvtsi128_si32(reduce1)) + static_cast<uint32>(_mm_extract_epi32(reduce1, 2));
#else
            return _mm_cvtsi128_si64(reduce1) + _mm_extract_epi64(reduce1, 1);
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
    base_declare_const_function base_always_inline static size_t Reduce(__m256i value) noexcept {
        static_assert(
            typeSize == 1 || typeSize == 2 || typeSize == 4 || typeSize == 8,
            "base::algorithm::CountTraits<arch::CpuFeature::AVX2>::Reduce: Unsupported typeSize size");

        constexpr auto shuffleMask = _MM_SHUFFLE(3, 1, 2, 0); // Cross lane, to reduce further on low lane

        if constexpr (typeSize == 8) {
            const __m128i low64     = _mm256_castsi256_si128(value);
            const __m128i high64    = _mm256_extracti128_si256(value, 1);

            const __m128i reduce    = _mm_add_epi64(low64, high64);
            return CountTraits<arch::CpuFeature::SSE2>::Reduce<8>(reduce);
        }
        else if constexpr (typeSize == 4) {
            const __m256i reduce4 = _mm256_hadd_epi32(value, _mm256_setzero_si256()); // (0+1),(2+3),0,0 per lane
            const __m256i reduce5 = _mm256_permute4x64_epi64(reduce4, shuffleMask); // low lane  (0+1),(2+3),(4+5),(6+7)

            const __m256i reduce6 = _mm256_hadd_epi32(reduce5, _mm256_setzero_si256()); // (0+...+3),(4+...+7),0,0
            const __m256i reduce7 = _mm256_hadd_epi32(reduce6, _mm256_setzero_si256()); // (0+...+7),0,0,0

            return static_cast<uint32>(_mm_cvtsi128_si32(_mm256_castsi256_si128(reduce7)));
        }
        else if constexpr (typeSize == 2) {
            const __m256i reduce2 = _mm256_hadd_epi16(value, _mm256_setzero_si256());
            const __m256i reduce3 = _mm256_unpacklo_epi16(reduce2, _mm256_setzero_si256());

            const __m256i reduce4 = _mm256_hadd_epi32(reduce3, _mm256_setzero_si256()); // (0+1),(2+3),0,0 per lane
            const __m256i reduce5 = _mm256_permute4x64_epi64(reduce4, shuffleMask); // low lane  (0+1),(2+3),(4+5),(6+7)

            const __m256i reduce6 = _mm256_hadd_epi32(reduce5, _mm256_setzero_si256()); // (0+...+3),(4+...+7),0,0
            const __m256i reduce7 = _mm256_hadd_epi32(reduce6, _mm256_setzero_si256()); // (0+...+7),0,0,0

            return static_cast<uint32>(_mm_cvtsi128_si32(_mm256_castsi256_si128(reduce7)));
        }
        else if constexpr (typeSize == 1) {
            const __m256i reduce1   = _mm256_sad_epu8(value, _mm256_setzero_si256());

            const __m128i low64     = _mm256_castsi256_si128(reduce1);
            const __m128i high64    = _mm256_extracti128_si256(reduce1, 1);

            const __m128i reduce8   = _mm_add_epi64(low64, high64);
            return CountTraits<arch::CpuFeature::SSE2>::Reduce<1>(reduce8);
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
            /*  
                vextracti64x4   ymm0, zmm1, 1
                vpaddq          ymm2, ymm0, ymm1
                vextracti64x2   xmm0, ymm2, 1
                vpaddq          xmm1, xmm0, xmm2
                vpsrldq         xmm0, xmm1, 8
                vpaddq          xmm1, xmm0, xmm1
                vmovq           rax, xmm1
            */
            return _mm512_reduce_add_epi64(value);
        }
        else if constexpr (typeSize == 4) {
#if 0
            /*
                vextracti32x8   ymm0, zmm1, 1
                vpaddd          ymm1, ymm0, ymm1
                vextracti128    xmm0, ymm1, 1
                vpaddd          xmm2, xmm0, xmm1
                vpsrldq         xmm0, xmm2, 8
                vpaddd          xmm1, xmm0, xmm2
                vpsrldq         xmm0, xmm1, 4
                vpaddd          xmm1, xmm0, xmm1
                vmovd           eax, xmm1
            */
            return _mm512_reduce_add_epi32(value);
#else
            /*
                vextracti64x4   ymm0, zmm1, 1
                vpaddd          ymm2, ymm0, ymm1
                vextracti64x2   xmm3, ymm2, 1
                vpaddd          xmm3, xmm3, xmm2
                vpextrd         ecx, xmm3, 2
                vpextrd         eax, xmm3, 3
                add             eax, ecx
                vpextrd         ecx, xmm3, 1
                add             eax, ecx
                vmovd           ecx, xmm3
                add             eax, ecx
            */

            auto b = _mm256_add_epi32(_mm512_castsi512_si256(value), _mm512_extracti64x4_epi64(value, 1));
            auto d = _mm_add_epi32(_mm256_castsi256_si128(b), _mm256_extracti64x2_epi64(b, 1));
            return  static_cast<uint32>(_mm_cvtsi128_si32(d))    + static_cast<uint32>(_mm_extract_epi32(d, 1))
                +   static_cast<uint32>(_mm_extract_epi32(d, 2)) + static_cast<uint32>(_mm_extract_epi32(d, 3));
#endif
        }
        else if constexpr (typeSize == 2) {
            auto b = _mm256_add_epi16(_mm512_castsi512_si256(value), _mm512_extracti64x4_epi64(value, 1));
            auto d = _mm_add_epi16(_mm256_castsi256_si128(b), _mm256_extracti64x2_epi64(b, 1));

            auto high = _mm_cvtsi128_si64(d);
            auto low =  _mm_extract_epi64(d, 1);

            size_t sum = 0;

            sum += (uint16)(high & 0xFFFF);
            sum += (uint16)((high >> 16) & 0xFFFF);
            sum += (uint16)((high >> 32) & 0xFFFF);
            sum += (uint16)((high >> 48) & 0xFFFF);

            sum += (uint16)(low & 0xFFFF);
            sum += (uint16)((low >> 16) & 0xFFFF);
            sum += (uint16)((low >> 32) & 0xFFFF);
            sum += (uint16)((low >> 48) & 0xFFFF);

            return sum;
        }
        else if constexpr (typeSize == 1) {
            const auto reduce = _mm512_sad_epu8(value, _mm512_setzero_si512());

            const auto high = _mm512_castsi512_si256(reduce);
            const auto low  = _mm512_extracti64x4_epi64(reduce, 1);

            return  _mm256_extract_epi64(high, 0)   + _mm256_extract_epi64(high, 1) + 
                    _mm256_extract_epi64(high, 2)   + _mm256_extract_epi64(high, 3) +
                    _mm256_extract_epi64(low, 0)    + _mm256_extract_epi64(low, 1)  + 
                    _mm256_extract_epi64(low, 2)    + _mm256_extract_epi64(low, 3);

            //return ((int64*)&reduce)[0] + ((int64*)&reduce)[1] + ((int64*)&reduce)[2]
            //    + ((int64*)&reduce)[3] + ((int64*)&reduce)[4] + ((int64*)&reduce)[5] + ((int64*)&reduce)[6]
            //    + ((int64*)&reduce)[7];
        }

        return -1;
    }
};



__BASE_NAMESPACE_END
