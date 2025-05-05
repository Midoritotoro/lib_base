#pragma once 

#include <base/core/arch/Platform.h>
#include <src/core/utility/simd/SimdAlgorithmSafety.h>

#include <src/core/utility/simd/SimdTailMask.h>
#include <base/core/arch/ProcessorFeatures.h>

#include <src/core/memory/MemoryUtility.h>
#include <base/core/utility/Math.h>

__BASE_NAMESPACE_BEGIN

template <class _Type_>
CONSTEXPR_CXX20 always_inline NODISCARD const void* FindScalar(
    const void* firstPointer,
    const void* lastPointer,
    _Type_      value) noexcept
{
    for (_Type_* current = static_cast<_Type_*>(firstPointer); current != lastPointer; ++current)
        if (*current == value)
            return current;

    return firstPointer;
}

template <
    class _Traits_,
    class _Type_>
CONSTEXPR_CXX20 always_inline NODISCARD const void* FindSSE2(
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
            const int bingo    = _mm_movemask_epi8(_Traits_::CompareSse(data, comparand));

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
CONSTEXPR_CXX20 always_inline NODISCARD const void* FindAVX(
    const void* firstPointer,
    const void* lastPointer,
    _Type_      value) noexcept
{ 
    const auto sizeInBytes = memory::ByteLength(firstPointer, lastPointer);
    const std::size_t avxSize = sizeInBytes & ~size_t{ 0x1F };

    if (avxSize != 0) {
        ZeroUpperOnDeleteAvx guard;

        const __m256i comparand = _Traits_::SetAVX(value);
        const void* stopAt = firstPointer;

        memory::AdvanceBytes(stopAt, avxSize);

        do {
            const __m256i data = _mm256_loadu_si256(static_cast<const __m256i*>(firstPointer));
            const int bingo = _mm256_movemask_epi8(_Traits_::CompareAVX(data, comparand));

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

            const int bingo =
                _mm256_movemask_epi8(
                    _mm256_and_si256(
                        _Traits_::CompareAVX(data, comparand), tailMask));

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
CONSTEXPR_CXX20 always_inline NODISCARD const void* FindAVX512(
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

            const __mmask64 bingo =
                _mm512_movepi8_mask(
                    _mm512_and_si512(
                        _Traits_::CompareAVX512(data, comparand), tailMask));

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

template <class _Type_>
DECLARE_NOALIAS CONSTEXPR_CXX20 NODISCARD const void* FindVectorized(
    const void*     firstPointer,
    const void*     lastPointer,
    const _Type_&   value) noexcept
{
    if (ProcessorFeatures::AVX512F())
        return FindAVX512(firstPointer, lastPointer, value);
    else if (ProcessorFeatures::AVX())
        return FindAVX(firstPointer, lastPointer, value);
    else if (ProcessorFeatures::SSE2())
        return FindSSE2(firstPointer, lastPointer, value);

    return FindScalar(firstPointer, lastPointer, value);
}

__BASE_NAMESPACE_END
