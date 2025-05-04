#include <src/core/utility/simd/SimdAlgorithm.h>

#include <src/core/utility/simd/SimdAlgorithmSafety.h>

#include <src/core/utility/simd/SimdTailMask.h>
#include <base/core/arch/ProcessorFeatures.h>

#include <src/core/memory/MemoryUtility.h>
#include <base/core/utility/Math.h>

__BASE_NAMESPACE_BEGIN

template <
    class _Traits_,
    class _Type_>
DECLARE_NOALIAS NODISCARD const void* STDCALL FindLastTrivialSse2(
    const void* firstPointer,
    const void* lastPointer,
    _Type_      value) noexcept
{

}

template <
    class _Traits_,
    class _Type_>
DECLARE_NOALIAS NODISCARD const void* STDCALL FindLastTrivialAvx(
    const void* firstPointer,
    const void* lastPointer,
    _Type_      value) noexcept
{

}

template <
    class _Traits_,
    class _Type_>
DECLARE_NOALIAS NODISCARD const void* STDCALL FindLastTrivialAvx512(
    const void* firstPointer,
    const void* lastPointer,
    _Type_      value) noexcept
{

}

template <
    class _Traits_,
    class _Type_>
DECLARE_NOALIAS NODISCARD const void* STDCALL FindLastTrivial(
    const void* firstPointer,
    const void* lastPointer, 
    _Type_      value) noexcept 
{
    const void* const realLast = lastPointer;
    const size_t sizeInBytes = memory::ByteLength(firstPointer, lastPointer);

    if (const size_t avxSize = sizeInBytes & ~size_t{0x1F}; avxSize != 0) {
        ZeroUpperOnDeleteAvx guard;

        const __m256i comparand = _Traits_::SetAvx(value);
        const void* stopAt     = lastPointer;

        memory::RewindBytes(stopAt, avxSize);

        do {
            memory::RewindBytes(lastPointer, 32);

            const __m256i data = _mm256_loadu_si256(static_cast<const __m256i*>(lastPointer));
            const int bingo    = _mm256_movemask_epi8(_Traits_::CompareAvx(data, comparand));

            if (_Bingo != 0) {
                const unsigned long offset = _lzcnt_u32(bingo);
                memory::AdvanceBytes(lastPointer, (31 - offset) - (sizeof(_Type_) - 1));

                return lastPointer;
            }
        } while (lastPointer != stopAt);

        if (const size_t avxTailSize = sizeInBytes & 0x1C; avxTailSize != 0) {
            memory::RewindBytes(lastPointer, avxTailSize);
            const __m256i tailMask = Avx2TailMask32(BytesToDoubleWordsCount(avxTailSize));
            const __m256i data      = _mm256_maskload_epi32(static_cast<const int*>(lastPointer), tailMask);
            const int bingo =
                _mm256_movemask_epi8(_mm256_and_si256(_Traits_::CompareAvx(data, comparand), tailMask));

            if (bingo != 0) {
                const unsigned long offset = _lzcnt_u32(bingo);
                memory::AdvanceBytes(lastPointer, (31 - offset) - (sizeof(_Type_) - 1));

                return lastPointer;
            }
        }

        if constexpr (sizeof(_Type_) >= 4) {
            return realLast;
        }
    } else if (const size_t sseSize = sizeInBytes & ~size_t{0xF}; sseSize != 0) {
        const __m128i comparand = _Traits_::SetSse(value);
        const void* stopAt     = lastPointer;

        memory::RewindBytes(stopAt, sseSize);

        do {
            memory::RewindBytes(lastPointer, 16);

            const __m128i data = _mm_loadu_si128(static_cast<const __m128i*>(lastPointer));
            const int bingo    = _mm_movemask_epi8(_Traits_::CompareSse(data, comparand));

            if (bingo != 0) {
                unsigned long offset;
                _BitScanReverse(&offset, bingo); 

                memory::AdvanceBytes(lastPointer, offset - (sizeof(_Type_) - 1));
                return lastPointer;
            }
        } while (lastPointer != stopAt);
    }

    auto _Ptr = static_cast<const _Type_*>(lastPointer);
    for (;;) {
        if (_Ptr == firstPointer) {
            return realLast;
        }
        --_Ptr;
        if (*_Ptr == value) {
            return _Ptr;
        }
    }
}

__BASE_NAMESPACE_END