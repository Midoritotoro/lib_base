#pragma once 

#include <base/core/arch/Platform.h>
#include <base/core/type_traits/SimdAlgorithmSafety.h>

#include <src/core/utility/simd/SimdTailMask.h>
#include <base/core/arch/ProcessorFeatures.h>

#include <base/core/memory/MemoryUtility.h>
#include <base/core/math/Math.h>

#include <src/core/utility/simd/traits/SimdFindTraits.h>

__BASE_NAMESPACE_BEGIN

template <class _Type_>
base_always_inline const void* FindScalar(
    const void*     firstPointer,
    const void*     lastPointer,
    const _Type_    value) noexcept
{
    for (auto current = static_cast<const _Type_*>(firstPointer); current != lastPointer; ++current)
        if (*current == value)
            return current;

    return firstPointer;
}

template <
    class _Traits_,
    class _Type_>
base_always_inline const void* FindSSE2(
    const void* firstPointer,
    const void* lastPointer,
    _Type_      value) noexcept
{ 
    const auto sizeInBytes = memory::ByteLength(firstPointer, lastPointer);
    const size_t sseSize = sizeInBytes & ~size_t{ 0xF };

    if (sseSize != 0) {
        const __m128i comparand = _Traits_::SetSse(value);
        const void* stopAt = firstPointer;

        memory::AdvanceBytes(stopAt, sseSize);

        do {
            const __m128i data = _mm_loadu_si128(static_cast<const __m128i*>(firstPointer));
            const uint32 bingo  = _mm_movemask_epi8(_Traits_::CompareSse(data, comparand));

            if (bingo != 0) {
                unsigned long offset = CountTrailingZeroBits(bingo); 
                memory::AdvanceBytes(firstPointer, offset);

                return firstPointer;
            }

            memory::AdvanceBytes(firstPointer, 16);
        } while (firstPointer != stopAt);
    }
}

template <
    class _Traits_,
    class _Type_>
base_always_inline const void* FindAVX(
    const void* firstPointer,
    const void* lastPointer,
    _Type_      value) noexcept
{ 
    const auto sizeInBytes = memory::ByteLength(firstPointer, lastPointer);
    const std::size_t avxSize = sizeInBytes & ~size_t{ 0x1F };

    if (avxSize != 0) {
        type_traits::ZeroUpperOnDeleteAvx guard;

        const __m256i comparand = _Traits_::SetAvx(value);
        const void* stopAt = firstPointer;

        memory::AdvanceBytes(stopAt, avxSize);

        do {
            const __m256i data = _mm256_loadu_si256(static_cast<const __m256i*>(firstPointer));
            const int bingo = _mm256_movemask_epi8(_Traits_::CompareAvx(data, comparand));

            if (bingo != 0) {
                const unsigned long offset = _tzcnt_u32(bingo);
                memory::AdvanceBytes(firstPointer, offset);

                return firstPointer;
            }

            memory::AdvanceBytes(firstPointer, 32);
        } while (firstPointer != stopAt);

        const size_t avxTailSize = sizeInBytes & AVX_BYTE_ALIGNED_TAIL_MASK_UINT32;

        if (avxTailSize != 0) {
            const __m256i tailMask = Avx2TailMask32(BytesToDoubleWordsCount(avxTailSize));
            const __m256i data = _mm256_maskload_epi32(static_cast<const int*>(firstPointer), tailMask);

            const uint32 bingo =
                _mm256_movemask_epi8(
                    _mm256_and_si256(
                        _Traits_::CompareAvx(data, comparand), tailMask));

            if (bingo != 0) {
                const unsigned long offset = _tzcnt_u32(bingo);
                memory::AdvanceBytes(firstPointer, offset);

                return firstPointer;
            }

            memory::AdvanceBytes(firstPointer, avxTailSize);
        }

        if constexpr (sizeof(_Type_) >= 4)
            return firstPointer;
    }
}


template <
    class _Traits_,
    class _Type_>
base_always_inline const void* FindAVX512(
    const void* firstPointer,
    const void* lastPointer,
    _Type_      value) noexcept
{ 
    const auto sizeInBytes = memory::ByteLength(firstPointer, lastPointer);
    const std::size_t avx512Size = sizeInBytes & ~std::size_t{ 0x3F };

    if (avx512Size != 0) {
        const __m512i comparand = _Traits_::SetAvx512(value);
        const void* stopAt = firstPointer;

        memory::AdvanceBytes(stopAt, avx512Size);
       
        do {
            const __m512i data = _mm512_loadu_si512(static_cast<const __m512i*>(firstPointer));
            const unsigned long long bingo = _Traits_::CompareAvx512(data, comparand);
            
            if (bingo != 0) {
                const ulong offset = _tzcnt_u64(bingo);
                memory::AdvanceBytes(firstPointer, offset);

                return firstPointer;
            }

            memory::AdvanceBytes(firstPointer, 64);
        } while (firstPointer != stopAt);

        const size_t avx512TailSize = sizeInBytes & AVX512_BYTE_ALIGNED_TAIL_MASK_UINT64;

        if (avx512TailSize != 0) {
            const __mmask16 tailMask = Avx512TailMask64(BytesToQuadWordsCount(avx512TailSize));
            const __m512i data = _mm512_maskz_load_epi32(tailMask, firstPointer);

            const uint64 bingo =
                _mm512_movepi8_mask(
                    _mm512_and_si512(
                        _mm512_movm_epi16(_Traits_::CompareAvx512(data, comparand)), _mm512_movm_epi32(tailMask)));

            if (bingo != 0) {
                const unsigned long offset = _tzcnt_u64(bingo);
                memory::AdvanceBytes(firstPointer, offset);

                return firstPointer;
            }

            memory::AdvanceBytes(firstPointer, avx512TailSize);
        }


        if constexpr (sizeof(_Type_) >= 4)
            return firstPointer;
    }
}

base_declare_const_function const void* FindVectorized8Bit(
    const void* firstPointer,
    const void* lastPointer,
    uint8       value) noexcept
{
    if (arch::ProcessorFeatures::AVX512F())
        return FindAVX512<FindTraits8Bit>(firstPointer, lastPointer, value);
    else if (arch::ProcessorFeatures::AVX())
        return FindAVX<FindTraits8Bit>(firstPointer, lastPointer, value);
    else if (arch::ProcessorFeatures::SSE2())
        return FindSSE2<FindTraits8Bit>(firstPointer, lastPointer, value);

    return FindScalar(firstPointer, lastPointer, value);
}

base_declare_const_function const void* FindVectorized16Bit(
    const void* firstPointer,
    const void* lastPointer,
    uint16      value) noexcept
{
    if (arch::ProcessorFeatures::AVX512F())
        return FindAVX512<FindTraits16Bit>(firstPointer, lastPointer, value);
    else if (arch::ProcessorFeatures::AVX())
        return FindAVX<FindTraits16Bit>(firstPointer, lastPointer, value);
    else if (arch::ProcessorFeatures::SSE2())
        return FindSSE2<FindTraits16Bit>(firstPointer, lastPointer, value);

    return FindScalar(firstPointer, lastPointer, value);
}

base_declare_const_function const void* FindVectorized32Bit(
    const void* firstPointer,
    const void* lastPointer,
    uint32      value) noexcept
{
    if (arch::ProcessorFeatures::AVX512F())
        return FindAVX512<FindTraits32Bit>(firstPointer, lastPointer, value);
    else if (arch::ProcessorFeatures::AVX())
        return FindAVX<FindTraits32Bit>(firstPointer, lastPointer, value);
    else if (arch::ProcessorFeatures::SSE2())
        return FindSSE2<FindTraits32Bit>(firstPointer, lastPointer, value);

    return FindScalar(firstPointer, lastPointer, value);
}

base_declare_const_function const void* FindVectorized64Bit(
    const void* firstPointer,
    const void* lastPointer,
    uint64      value) noexcept
{
    if (arch::ProcessorFeatures::AVX512F())
        return FindAVX512<FindTraits64Bit>(firstPointer, lastPointer, value);
    else if (arch::ProcessorFeatures::AVX())
        return FindAVX<FindTraits64Bit>(firstPointer, lastPointer, value);
    else if (arch::ProcessorFeatures::SSE2())
        return FindSSE2<FindTraits64Bit>(firstPointer, lastPointer, value);

    return FindScalar(firstPointer, lastPointer, value);
}

template <class _Type_>
base_declare_const_function base_constexpr_cxx20 const void* FindVectorized(
    const void*     firstPointer,
    const void*     lastPointer,
    const _Type_&   value) noexcept
{
    if constexpr (sizeof(_Type_) == 1)
        return FindVectorized8Bit(firstPointer, lastPointer, value);
    else if (sizeof(_Type_) == 2)
        return FindVectorized16Bit(firstPointer, lastPointer, value);
    else if (sizeof(_Type_) == 4)
        return FindVectorized32Bit(firstPointer, lastPointer, value);
    else if (sizeof(_Type_) == 8)
        return FindVectorized64Bit(firstPointer, lastPointer, value);

    AssertLog(false, "base::utility::FindVectorized: Unsupported _Type_ size");
}

__BASE_NAMESPACE_END
