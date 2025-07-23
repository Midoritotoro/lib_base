#include <src/core/utility/simd/SimdAlgorithm.h>

#include <src/core/utility/simd/SimdAlgorithmSafety.h>

#include <src/core/utility/simd/SimdTailMask.h>
#include <base/core/arch/ProcessorFeatures.h>

#include <src/core/memory/MemoryUtility.h>
#include <base/core/utility/Math.h>

#include <src/core/utility/simd/traits/SimdFindTraits.h>

__BASE_NAMESPACE_BEGIN


template <class _Type_>
DECLARE_NOALIAS NODISCARD const void* STDCALL FindLastScalar(
    const void* firstPointer,
    const void* lastPointer,
    _Type_      value) noexcept
{
    return nullptr;
}

template <
    class _Traits_,
    class _Type_>
DECLARE_NOALIAS NODISCARD const void* STDCALL FindLastTrivialSse2(
    const void* firstPointer,
    const void* lastPointer,
    _Type_      value) noexcept
{
    const void* const realLast = lastPointer;
    const auto sizeInBytes = memory::ByteLength(firstPointer, lastPointer);

    const size_t sseSize = sizeInBytes & ~size_t{ 0xF };

    if (sseSize != 0) {
        const __m128i comparand = _Traits_::SetSse(value);
        const void* stopAt = lastPointer;

        memory::RewindBytes(stopAt, sseSize);

        do {
            memory::RewindBytes(lastPointer, 16);

            const __m128i data = _mm_loadu_si128(static_cast<const __m128i*>(lastPointer));
            const uint bingo = _mm_movemask_epi8(_Traits_::CompareSse(data, comparand));

            if (bingo != 0) {
                unsigned long offset = CountLeadingZeroBits(bingo);

                memory::AdvanceBytes(lastPointer, offset - (sizeof(_Type_) - 1));
                return lastPointer;
            }
        } while (lastPointer != stopAt);
    }

    auto current = static_cast<const _Type_*>(lastPointer);

    for (;;) {
        if (current == firstPointer)
            return realLast;
        
        --current;

        if (*current == value)
            return current;

    }
}

template <
    class _Traits_,
    class _Type_>
DECLARE_NOALIAS NODISCARD const void* STDCALL FindLastTrivialAvx(
    const void* firstPointer,
    const void* lastPointer,
    _Type_      value) noexcept
{
    const void* const realLast = lastPointer;
    const auto sizeInBytes = memory::ByteLength(firstPointer, lastPointer);

    const auto avxSize = sizeInBytes & ~size_t{ 0x1F };

    if (avxSize != 0) {
        ZeroUpperOnDeleteAvx guard;

        const __m256i comparand = _Traits_::SetAvx(value);
        const void* stopAt     = lastPointer;

        memory::RewindBytes(stopAt, avxSize);

        do {
            memory::RewindBytes(lastPointer, 32);

            const __m256i data = _mm256_loadu_si256(static_cast<const __m256i*>(lastPointer));
            const uint bingo    = _mm256_movemask_epi8(_Traits_::CompareAvx(data, comparand));

            if (bingo != 0) {
                const unsigned long offset = CountLeadingZeroBits(bingo);
                memory::AdvanceBytes(lastPointer, (31 - offset) - (sizeof(_Type_) - 1));

                return lastPointer;
            }
        } while (lastPointer != stopAt);

        const size_t avxTailSize = sizeInBytes & AVX_BYTE_ALIGNED_TAIL_MASK_UINT32;

        if (avxTailSize != 0) {
            memory::RewindBytes(lastPointer, avxTailSize);

            const __m256i tailMask  = Avx2TailMask32(BytesToDoubleWordsCount(avxTailSize));
            const __m256i data      = _mm256_maskload_epi32(static_cast<const int*>(lastPointer), tailMask);
            
            const uint bingo =
                _mm256_movemask_epi8(
                    _mm256_and_si256(_Traits_::CompareAvx(data, comparand), tailMask));

            if (bingo != 0) {
                const ulong offset = CountLeadingZeroBits(bingo);
                memory::AdvanceBytes(lastPointer, (31 - offset) - (sizeof(_Type_) - 1));

                return lastPointer;
            }
        }

        if constexpr (sizeof(_Type_) >= 4) {
            return realLast;
        }
    }

    return FindLastTrivialSse2<_Traits_>(firstPointer, lastPointer, value);
}

template <
    class _Traits_,
    class _Type_>
DECLARE_NOALIAS NODISCARD const void* STDCALL FindLastTrivialAvx512(
    const void* firstPointer,
    const void* lastPointer,
    _Type_      value) noexcept
{
    const void* const realLast = lastPointer;
    const auto sizeInBytes = memory::ByteLength(firstPointer, lastPointer);

    const auto avx512Size = sizeInBytes & ~size_t{ 0x3F };

    if (avx512Size != 0) {
        const __m512i comparand = _Traits_::SetAvx512(value);
        const void* stopAt     = lastPointer;

        memory::RewindBytes(stopAt, avx512Size);

        do {
            memory::RewindBytes(lastPointer, 64);

            const __m512i data = _mm512_loadu_si512(lastPointer);
            const uint64 bingo = _Traits_::CompareAvx512(data, comparand);

            if (bingo != 0) {
                const unsigned long offset = CountLeadingZeroBits(bingo);
                memory::AdvanceBytes(lastPointer, (63 - offset) - (sizeof(_Type_) - 1));

                return lastPointer;
            }
        } while (lastPointer != stopAt);

        const size_t avx512TailSize = sizeInBytes & AVX512_BYTE_ALIGNED_TAIL_MASK_UINT64;

        if (avx512TailSize != 0) {
            memory::RewindBytes(lastPointer, avx512TailSize);

            const __mmask16 tailMask  = Avx512TailMask64(BytesToQuadWordsCount(avx512TailSize));
            const __m512i data      = _mm512_maskz_load_epi32(tailMask, lastPointer);
            
            const uint64 bingo =
                _mm512_movepi8_mask(
                    _mm512_and_epi32(
                        _mm512_movm_epi16(_Traits_::CompareAvx512(data, comparand)), _mm512_movm_epi32(tailMask))); 

            if (bingo != 0) {
                const unsigned long offset = CountLeadingZeroBits(bingo);
                memory::AdvanceBytes(lastPointer, (63 - offset) - (sizeof(_Type_) - 1));

                return lastPointer;
            }
        }

        if constexpr (sizeof(_Type_) >= 4) {
            return realLast;
        }
    }

    return FindLastTrivialAvx<_Traits_>(firstPointer, lastPointer, value);
}

DECLARE_NOALIAS NODISCARD const void* STDCALL FindLastTrivial8Bit(
    const void* firstPointer,
    const void* lastPointer,
    uint8       value) noexcept
{
    if (arch::ProcessorFeatures::AVX512F())
        return FindLastTrivialAvx512<FindTraits8Bit>(
            firstPointer, lastPointer, value);
    else if (arch::ProcessorFeatures::AVX())
        return FindLastTrivialAvx<FindTraits8Bit>(
            firstPointer, lastPointer, value);
    else if (arch::ProcessorFeatures::SSE2())
        return FindLastTrivialSse2<FindTraits8Bit>(
            firstPointer, lastPointer, value);

    return FindLastScalar(firstPointer, lastPointer, value);
}

DECLARE_NOALIAS NODISCARD const void* STDCALL FindLastTrivial16Bit(
    const void* firstPointer,
    const void* lastPointer,
    uint16      value) noexcept
{
    if (arch::ProcessorFeatures::AVX512F())
        return FindLastTrivialAvx512<FindTraits16Bit>(
            firstPointer, lastPointer, value);
    else if (arch::ProcessorFeatures::AVX())
        return FindLastTrivialAvx<FindTraits16Bit>(
            firstPointer, lastPointer, value);
    else if (arch::ProcessorFeatures::SSE2())
        return FindLastTrivialSse2<FindTraits16Bit>(
            firstPointer, lastPointer, value);

    return FindLastScalar(firstPointer, lastPointer, value);
}

DECLARE_NOALIAS NODISCARD const void* STDCALL FindLastTrivial32Bit(
    const void* firstPointer,
    const void* lastPointer,
    uint32      value) noexcept
{
    if (arch::ProcessorFeatures::AVX512F())
        return FindLastTrivialAvx512<FindTraits32Bit>(
            firstPointer, lastPointer, value);
    else if (arch::ProcessorFeatures::AVX())
        return FindLastTrivialAvx<FindTraits32Bit>(
            firstPointer, lastPointer, value);
    else if (arch::ProcessorFeatures::SSE2())
        return FindLastTrivialSse2<FindTraits32Bit>(
            firstPointer, lastPointer, value);

    return FindLastScalar(firstPointer, lastPointer, value);
}

DECLARE_NOALIAS NODISCARD const void* STDCALL FindLastTrivial64Bit(
    const void* firstPointer,
    const void* lastPointer,
    uint64      value) noexcept
{
    if (arch::ProcessorFeatures::AVX512F())
        return FindLastTrivialAvx512<FindTraits64Bit>(
            firstPointer, lastPointer, value);
    else if (arch::ProcessorFeatures::AVX())
        return FindLastTrivialAvx<FindTraits64Bit>(
            firstPointer, lastPointer, value);
    else if (arch::ProcessorFeatures::SSE2())
        return FindLastTrivialSse2<FindTraits64Bit>(
            firstPointer, lastPointer, value);

    return FindLastScalar(firstPointer, lastPointer, value);
}

template <class _Type_>
CONSTEXPR_CXX20 DECLARE_NOALIAS NODISCARD const void* STDCALL FindLastVectorized(
    const void*     firstPointer,
    const void*     lastPointer, 
    const _Type_&   value) noexcept 
{
    if constexpr (sizeof(_Type_) == 1)
        return FindLastTrivial8Bit(firstPointer, lastPointer, value);
    else if (sizeof(_Type_) == 2)
        return FindLastTrivial16Bit(firstPointer, lastPointer, value);
    else if (sizeof(_Type_) == 4)
        return FindLastTrivial32Bit(firstPointer, lastPointer, value);
    else if (sizeof(_Type_) == 8)
        return FindLastTrivial64Bit(firstPointer, lastPointer, value);

    return FindLastScalar(firstPointer, lastPointer, value);
}

__BASE_NAMESPACE_END