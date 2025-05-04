#pragma once 

#include <base/core/arch/Platform.h>
#include <src/core/utility/simd/SimdAlgorithmSafety.h>

#include <src/core/utility/simd/SimdTailMask.h>
#include <base/core/arch/ProcessorFeatures.h>

#include <src/core/memory/MemoryUtility.h>


__BASE_NAMESPACE_BEGIN

template <
    class _InputIterator_,
    class _Predicate_>
// Find first satisfying _Pred
CONSTEXPR_CXX20 inline NODISCARD _InputIterator_ FindIf(
    _InputIterator_             firstIterator,
    const _InputIterator_       lastIterator, 
    _Predicate_                 predicate)
{
    for (; firstIterator != lastIterator; unused(++firstIterator))
        if (predicate(*firstIterator))
            break;

    return firstIterator;
}

template <class _Type_>
CONSTEXPR_CXX20 always_inline NODISCARD const void* FindScalar(
    const void* firstPointer,
    const void* lastPointer,
    _Type_      value)
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
    _Type_      value)
{ 
    const auto sizeInBytes = memory::ByteLength(firstPointer, lastPointer);
    const size_t sseSize = sizeInBytes & ~size_t{ 0xF };

    if (sseSize != 0) {
        const __m128i _Comparand = _Traits_::SetSse(value);
        const void* _Stop_at = firstPointer;

        memory::AdvanceBytes(_Stop_at, sseSize);

        do {
            const __m128i _Data = _mm_loadu_si128(static_cast<const __m128i*>(firstPointer));
            const int _Bingo    = _mm_movemask_epi8(_Traits_::CompareSse(_Data, _Comparand));

            if (_Bingo != 0) {
                unsigned long _Offset = CountTrailingZeroBits(_Bingo); 
                memory::AdvanceBytes(firstPointer, _Offset);

                return firstPointer;
            }

            memory::AdvanceBytes(firstPointer, 16);
        } while (firstPointer != _Stop_at);
    }
}

template <
    class _Traits_,
    class _Type_>
CONSTEXPR_CXX20 always_inline NODISCARD const void* FindAVX(
    const void* firstPointer,
    const void* lastPointer,
    _Type_      value)
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
    _Type_      value)
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
                const unsigned long offset = _tzcnt_u64(bingo);
                memory::AdvanceBytes(firstPointer, offset);

                return firstPointer;
            }

            memory::AdvanceBytes(firstPointer, 64);
        } while (firstPointer != stopAt);

        const size_t avx512TailSize = sizeInBytes & AVX512_BYTE_ALIGNED_TAIL_MASK_UINT64;

        //if (avx512TailSize != 0) {
        //    const __m512i tailMask = Avx512TailMask64(avx512TailSize);
        //    const __m512i data = _mm512_maskz_loadu_epi8(tailMask, static_cast<const unsigned char*>(firstPointer));

        //    const __mmask64 comparisonMask =
        //        _mm512_kand(_Traits_::CompareAVX512(data, comparand), tailMask); // Mask the comparison

        //    if (comparisonMask != 0) {
        //        const unsigned long offset = _tzcnt_u64(comparisonMask);
        //        memory::AdvanceBytes(firstPointer, offset);

        //        return firstPointer;
        //    }

        //    memory::AdvanceBytes(firstPointer, avx512TailSize);
        //}


        if constexpr (sizeof(_Type_) >= 4)
            return firstPointer;
    }
}

template <
    class _InputIterator_,
    class _Type_>
CONSTEXPR_CXX20 inline NODISCARD _InputIterator_ FindVectorized(
    _InputIterator_             firstIterator,
    const _InputIterator_       lastIterator,
    const _Type_&               value)
{
    if (ProcessorFeatures::AVX512F())
        return FindAVX512(
            std::move(firstIterator), 
            std::move(lastIterator), value);
    else if (ProcessorFeatures::AVX())
        return FindAVX(
            std::move(firstIterator),
            std::move(lastIterator), value);
    else if (ProcessorFeatures::SSE42())
        return FindSSE42(
            std::move(firstIterator), 
            std::move(lastIterator), value);
    else if (ProcessorFeatures::SSE2())
        return FindSSE2(
            std::move(firstIterator), 
            std::move(lastIterator), value);

    return find;
}

__BASE_NAMESPACE_END
