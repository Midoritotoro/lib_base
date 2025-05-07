#pragma once 

#include <base/core/arch/Platform.h>
#include <src/core/utility/simd/SimdAlgorithmSafety.h>

#include <src/core/utility/simd/SimdTailMask.h>
#include <base/core/arch/ProcessorFeatures.h>

#include <src/core/memory/MemoryUtility.h>
#include <base/core/utility/Math.h>

#include <src/core/utility/simd/traits/SimdFindTraits.h>

__BASE_NAMESPACE_BEGIN

template <class _Type_>
 always_inline NODISCARD bool ContainsScalar(
    const void*     firstPointer,
    const void*     lastPointer,
    const _Type_    value) noexcept
{
    for (auto current = static_cast<const _Type_*>(firstPointer); current != lastPointer; ++current)
        if (*current == value)
            return true;

    return false;
}

template <
    class _Traits_,
    class _Type_>
 always_inline NODISCARD bool ContainsSSE2(
    const void* firstPointer,
    const void* lastPointer,
    _Type_      value) noexcept
{
    const auto sizeInBytes = memory::ByteLength(firstPointer, lastPointer);
    const size_t sseSize = sizeInBytes & ~size_t{ 0xF };

    if (sseSize != 0) {
        const auto comparand = _Traits_::SetSse(value);
        const void* stopAt = firstPointer;

        memory::AdvanceBytes(stopAt, sseSize);

        do {
            const auto data = _mm_loadu_si128(static_cast<const __m128i*>(firstPointer));
            const int bingo = _mm_movemask_epi8(_Traits_::CompareSse(data, comparand));

            if (bingo != 0)
                return true;

            memory::AdvanceBytes(firstPointer, 16);
        } while (firstPointer != stopAt);
    }

    if (firstPointer != lastPointer)
        return ContainsScalar(firstPointer, lastPointer, value);

    return false;
}

template <
    class _Traits_,
    class _Type_>
 always_inline NODISCARD bool ContainsAVX(
    const void* firstPointer,
    const void* lastPointer,
    _Type_      value) noexcept
{
    const auto sizeInBytes = memory::ByteLength(firstPointer, lastPointer);
    const std::size_t avxSize = sizeInBytes & ~size_t{ 0x1F };

    if (avxSize != 0) {
        ZeroUpperOnDeleteAvx guard;

        const auto comparand = _Traits_::SetAvx(value);
        const void* stopAt = firstPointer;

        memory::AdvanceBytes(stopAt, avxSize);

        do {
            const auto data = _mm256_loadu_si256(static_cast<const __m256i*>(firstPointer));
            const int bingo = _mm256_movemask_epi8(_Traits_::CompareAvx(data, comparand));

            if (bingo != 0)
                return true;

            memory::AdvanceBytes(firstPointer, 32);
        } while (firstPointer != stopAt);

        const size_t avxTailSize = sizeInBytes & AVX_BYTE_ALIGNED_TAIL_MASK_UINT32;

        if (avxTailSize != 0) {
            const __m256i tailMask = Avx2TailMask32(BytesToDoubleWordsCount(avxTailSize));
            const __m256i data = _mm256_maskload_epi32(static_cast<const int*>(firstPointer), tailMask);

            const int bingo =
                _mm256_movemask_epi8(
                    _mm256_and_si256(
                        _Traits_::CompareAvx(data, comparand), tailMask));

            if (bingo != 0)
                return true;

            memory::AdvanceBytes(firstPointer, avxTailSize);
        }

        if constexpr (sizeof(_Type_) >= 4)
            return false;
    }

    return false;
}


template <
    class _Traits_,
    class _Type_>
 always_inline NODISCARD bool ContainsAVX512(
    const void* firstPointer,
    const void* lastPointer,
    _Type_      value) noexcept
{
    const auto sizeInBytes = memory::ByteLength(firstPointer, lastPointer);
    const std::size_t avx512Size = sizeInBytes & ~std::size_t{ 0x3F };

    if (avx512Size != 0) {
        const auto comparand = _Traits_::SetAvx512(value);
        const void* stopAt = firstPointer;

        memory::AdvanceBytes(stopAt, avx512Size);

        do {
            const auto data = _mm512_loadu_si512(static_cast<const __m512i*>(firstPointer));
            const unsigned long long bingo = _Traits_::CompareAvx512(data, comparand);

            if (bingo != 0)
                return true;

            memory::AdvanceBytes(firstPointer, 64);
        } while (firstPointer != stopAt);

        const size_t avx512TailSize = sizeInBytes & AVX512_BYTE_ALIGNED_TAIL_MASK_UINT64;

        if (avx512TailSize != 0) {
            const __mmask16 tailMask = Avx512TailMask64(BytesToQuadWordsCount(avx512TailSize));
            const __m512i data = _mm512_maskz_load_epi32(tailMask, firstPointer);

            const __mmask64 bingo =
                _mm512_movepi8_mask(
                    _mm512_and_si512(
                        _mm512_movm_epi16(_Traits_::CompareAvx512(data, comparand)), _mm512_movm_epi32(tailMask)));

            if (bingo != 0)
                return true;

            memory::AdvanceBytes(firstPointer, avx512TailSize);
        }


        if constexpr (sizeof(_Type_) >= 4)
            return false;
    }

    return false;
}

DECLARE_NOALIAS  NODISCARD bool ContainsVectorized8Bit(
    const void* firstPointer,
    const void* lastPointer,
    uint8       value) noexcept
{
    if (ProcessorFeatures::AVX512F())
        return ContainsAVX512<FindTraits8Bit>(firstPointer, lastPointer, value);
    else if (ProcessorFeatures::AVX())
        return ContainsAVX<FindTraits8Bit>(firstPointer, lastPointer, value);
    else if (ProcessorFeatures::SSE2())
        return ContainsSSE2<FindTraits8Bit>(firstPointer, lastPointer, value);

    return ContainsScalar(firstPointer, lastPointer, value);
}

DECLARE_NOALIAS  NODISCARD bool ContainsVectorized16Bit(
    const void* firstPointer,
    const void* lastPointer,
    uint16      value) noexcept
{
    if (ProcessorFeatures::AVX512F())
        return ContainsAVX512<FindTraits16Bit>(firstPointer, lastPointer, value);
    else if (ProcessorFeatures::AVX())
        return ContainsAVX<FindTraits16Bit>(firstPointer, lastPointer, value);
    else if (ProcessorFeatures::SSE2())
        return ContainsSSE2<FindTraits16Bit>(firstPointer, lastPointer, value);

    return ContainsScalar(firstPointer, lastPointer, value);
}

DECLARE_NOALIAS  NODISCARD bool ContainsVectorized32Bit(
    const void* firstPointer,
    const void* lastPointer,
    uint32      value) noexcept
{
    if (ProcessorFeatures::AVX512F())
        return ContainsAVX512<FindTraits32Bit>(firstPointer, lastPointer, value);
    else if (ProcessorFeatures::AVX())
        return ContainsAVX<FindTraits32Bit>(firstPointer, lastPointer, value);
    else if (ProcessorFeatures::SSE2())
        return ContainsSSE2<FindTraits32Bit>(firstPointer, lastPointer, value);

    return ContainsScalar(firstPointer, lastPointer, value);
}

DECLARE_NOALIAS  NODISCARD bool ContainsVectorized64Bit(
    const void* firstPointer,
    const void* lastPointer,
    uint64      value) noexcept
{
    if (ProcessorFeatures::AVX512F())
        return ContainsAVX512<FindTraits64Bit>(firstPointer, lastPointer, value);
    else if (ProcessorFeatures::AVX())
        return ContainsAVX<FindTraits64Bit>(firstPointer, lastPointer, value);
    else if (ProcessorFeatures::SSE2())
        return ContainsSSE2<FindTraits64Bit>(firstPointer, lastPointer, value);

    return ContainsScalar(firstPointer, lastPointer, value);
}

template <class _Type_>
DECLARE_NOALIAS  NODISCARD bool ContainsVectorized(
    const void* firstPointer,
    const void* lastPointer,
    const _Type_& value) noexcept
{
    if constexpr (sizeof(_Type_) == 1)
        return ContainsVectorized8Bit(firstPointer, lastPointer, value);
    else if (sizeof(_Type_) == 2)
        return ContainsVectorized16Bit(firstPointer, lastPointer, value);
    else if (sizeof(_Type_) == 4)
        return ContainsVectorized32Bit(firstPointer, lastPointer, value);
    else if (sizeof(_Type_) == 8)
        return ContainsVectorized64Bit(firstPointer, lastPointer, value);

    AssertLog(false, "base::utility::ContainsVectorized: Unsupported _Type_ size");
}

__BASE_NAMESPACE_END
