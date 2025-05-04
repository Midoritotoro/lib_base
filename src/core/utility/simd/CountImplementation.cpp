#pragma once 

#include <base/core/arch/Platform.h>
#include <base/core/arch/ProcessorFeatures.h>

#include <src/core/memory/MemoryUtility.h>
#include <src/core/utility/simd/SimdAlgorithmSafety.h>

#include <src/core/utility/simd/traits/SimdCountTraits.h>
#include <src/core/utility/simd/SimdTailMask.h>

__BASE_NAMESPACE_BEGIN


template <class _Type_>
CONSTEXPR_CXX20 inline NODISCARD std::size_t CountScalar(
    void*               _First,
    const void* const   _Last,
    const _Type_&       _Value)
{
    std::size_t count = 0;

    for (auto current = reinterpret_cast<_Type_*>(_First); current != _Last; ++current)
        if ((*current) == _Value)
            ++count;

    return count;
}



template <
    class _Traits_,
    class _Type_>
CONSTEXPR_CXX20 inline NODISCARD std::size_t CountSSE42(
    void*               _First,
    const void* const   _Last,
    const _Type_&       _Value) noexcept
{
    size_t result = 0;

    const auto sizeInBytes = memory::ByteLength(_First, _Last);
    size_t sseSize = sizeInBytes & ~size_t{ 0xF };

    if (sseSize == 0)
        return;
   
    const auto comparand = _Traits_::SetSse(_Value);
    const void* stopAt = _First;

    for (;;) {
        if constexpr (sizeof(_Type_) >= sizeof(size_t)) {
            memory::AdvanceBytes(stopAt, sseSize);
        }
        else {
            constexpr size_t maximumPortionSize = _Traits_::maxCount * 16;
            const size_t portionSize = sseSize < maximumPortionSize
                ? sseSize
                : maximumPortionSize;

            memory::AdvanceBytes(stopAt, portionSize);
            sseSize -= portionSize;
        }

        auto countVector = _mm_setzero_si128();

        do {
            const auto data = _mm_loadu_si128(static_cast<const __m128i*>(_First));
            const auto mask = _Traits_::CompareSse(data, comparand);

            countVector = _Traits_::SubstractSse(countVector, mask);
            memory::AdvanceBytes(_First, 16);
        } while (_First != stopAt);

        result += _Traits_::ReduceSse(countVector);

        if constexpr (sizeof(_Type_) >= sizeof(size_t))
            break;
        else if (sseSize == 0)
            break;
    }
}


template <
    class _Traits_,
    class _Type_>
CONSTEXPR_CXX20 inline NODISCARD std::size_t CountAVX(
    const void*         _First, 
    const void* const   _Last,
    const _Type_        _Val) noexcept
{
    size_t result = 0;

    const auto sizeInBytes = memory::ByteLength(_First, _Last);
    size_t avxSize = sizeInBytes & ~size_t{ 0x1F };

    if (avxSize != 0) {
        const auto comparand = _Traits_::SetAvx(_Val);
        const void* stopAt = _First;

        for (;;) {
            if constexpr (sizeof(_Type_) >= sizeof(size_t)) {
                memory::AdvanceBytes(stopAt, avxSize);
            }
            else {
                constexpr size_t maxPortionSize = _Traits_::maxCount * 32;
                const size_t portionSize = avxSize < maxPortionSize
                    ? avxSize 
                    : maxPortionSize;

                memory::AdvanceBytes(stopAt, portionSize);
                avxSize -= portionSize;
            }

            auto countVector = _mm256_setzero_si256();

            do {
                const auto data = _mm256_loadu_si256(static_cast<const __m256i*>(_First));
                const auto mask = _Traits_::CompareAvx(data, comparand);

                countVector = _Traits_::SubstractAvx(countVector, mask);
                memory::AdvanceBytes(_First, 32);
            } while (_First != stopAt);

            result += _Traits_::ReduceAvx(countVector);

            if constexpr (sizeof(_Type_) >= sizeof(size_t))
                break;
            else if (avxSize == 0)
                break;
        }

        const size_t avxTailSize = sizeInBytes & AVX_BYTE_ALIGNED_TAIL_MASK_UINT32;

        if (avxTailSize != 0) {
            const __m256i tailMask = Avx2TailMask32(BytesToDoubleWordsCount(avxTailSize));

            const __m256i data = _mm256_maskload_epi32(static_cast<const int*>(_First), tailMask);
            const __m256i mask = _mm256_and_si256(_Traits_::CompareAvx(data, comparand), tailMask);

            const int bingo = _mm256_movemask_epi8(mask);

            const size_t tailCount = __popcnt(bingo); // Assume available with SSE4.2

            result += tailCount / sizeof(_Type_);
            memory::AdvanceBytes(_First, avxTailSize);
        }

        _mm256_zeroupper();

        if constexpr (sizeof(_Type_) >= 4)
            return result;
    }
}


template <
    class _Traits_,
    class _Type_>
CONSTEXPR_CXX20 inline NODISCARD std::size_t CountAVX512(
    void*               _First,
    const void* const   _Last,
    const _Type_&       _Value) 
{   
    size_t result = 0;

    const auto sizeInBytes = memory::ByteLength(_First, _Last);
    size_t avx512Size = sizeInBytes & ~size_t{ 0x3F };

    if (avx512Size != 0) {
        const auto comparand = _Traits_::SetAvx512(_Value);
        const void* stopAt = _First;

        for (;;) {
            if constexpr (sizeof(_Type_) >= sizeof(size_t)) {
                memory::AdvanceBytes(stopAt, avx512Size);
            }
            else {
                constexpr size_t maxPortionSize = _Traits_::maxCount * 64;
                const size_t portionSize = avx512Size < maxPortionSize
                    ? avx512Size
                    : maxPortionSize;

                memory::AdvanceBytes(stopAt, portionSize);
                avx512Size -= portionSize;
            }

            auto countVector = _mm512_setzero_si512();

            do {
                const auto data = _mm512_loadu_si512(static_cast<const __m512i*>(_First));
                const auto mask = _Traits_::CompareAvx512(data, comparand);

                countVector = _Traits_::SubstractAvx512(countVector, mask);
                memory::AdvanceBytes(_First, 64);
            } while (_First != stopAt);

            result += _Traits_::ReduceAvx512(countVector);

            if constexpr (sizeof(_Type_) >= sizeof(size_t))
                break;
            else if (avx512Size == 0)
                break;
        }

        const size_t avxTailSize = sizeInBytes & AVX512_BYTE_ALIGNED_TAIL_MASK_UINT64;

        if (avxTailSize != 0) {
            const __mmask16 tailMask = Avx512TailMask64(BytesToDoubleWordsCount(avxTailSize));

            const __m512i data = _mm512_mask_load_epi32(static_cast<const int*>(_First), tailMask);
            const __m512i mask = _mm512_and_si512(_Traits_::CompareAvx(data, comparand), tailMask);

            const int bingo = _mm512_movepi8_mask(mask);
            const size_t tailCount = PopulationCount(bingo);

            result += tailCount / sizeof(_Type_);
            memory::AdvanceBytes(_First, avxTailSize);
        }

        if constexpr (sizeof(_Type_) >= 8)
            return result;
    }
}

template <class _Type_>
DECLARE_NOALIAS CONSTEXPR_CXX20 inline NODISCARD std::size_t CountVectorized(
    const void*     firstPointer,
    const void*     lastPointer,
    const _Type_&   value) noexcept
{
    if (ProcessorFeatures::AVX512F())
        return CountAVX512(firstPointer, lastPointer, value);
    else if (ProcessorFeatures::AVX())
        return CountAVX(firstPointer, lastPointer, value);
    else if (ProcessorFeatures::SSE42())
        return CountSSE42(firstPointer, lastPointer, value);

    return CountScalar(firstPointer, lastPointer, value);
}

__BASE_NAMESPACE_END