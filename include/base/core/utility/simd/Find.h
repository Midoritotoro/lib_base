#pragma once 

#include <base/core/arch/Platform.h>

#include <base/core/memory/MemoryRange.h>
#include <base/core/utility/simd/SimdAlgorithmSafety.h>

__BASE_NAMESPACE_BEGIN

template <
    class _InputIterator_,
    class _Predicate_>
// Find first satisfying _Pred
CONSTEXPR_CXX20 inline NODISCARD _InputIterator_ FindIf(
    _InputIterator_             _First, 
    const _InputIterator_       _Last, 
    _Predicate_                 _Pred)
{
    auto _UFirst      = memory::UnFancy(_First);
    const auto _ULast = memory::UnFancy(_Last);

    for (; _UFirst != _ULast; ++_UFirst)
        if (_Pred(*_UFirst))
            break;

    _First = std::forward<_InputIterator_>(_UFirst);
    return _First;
}

template <
    class _Traits_,
    class _Type_>
CONSTEXPR_CXX20 inline NODISCARD const void* FindSSE2(
    const void* _First,
    const void* _Last,
    _Type_      _Val)
{ 
    const auto sizeInBytes = memory::ByteLength(_First, _Last);
    const size_t sseSize = sizeInBytes & ~size_t{ 0xF };

    if (sseSize != 0) {
        const __m128i _Comparand = _Traits_::SetSse(_Val);
        const void* _Stop_at = _First;

        memory::AdvanceBytes(_Stop_at, sseSize);

        do {
            const __m128i _Data = _mm_loadu_si128(static_cast<const __m128i*>(_First));
            const int _Bingo = _mm_movemask_epi8(_Traits_::CompareSse(_Data, _Comparand));

            if (_Bingo != 0) {
                unsigned long _Offset;

                _BitScanForward(&_Offset, _Bingo);
                memory::AdvanceBytes(_First, _Offset);

                return _First;
            }

            memory::AdvanceBytes(_First, 16);
        } while (_First != _Stop_at);
    }
}


template <
    class _Traits_,
    class _Type_>
CONSTEXPR_CXX20 inline NODISCARD const void* FindSSE42(
    const void* _First,
    const void* _Last,
    _Type_      _Val)
{ 
    return FindSSE2(_First, _Last, _Val);
}


template <
    class _Traits_,
    class _Type_>
CONSTEXPR_CXX20 inline NODISCARD const void* FindAVX(
    const void* _First,
    const void* _Last,
    _Type_      _Val)
{ 
    const auto sizeInBytes = memory::ByteLength(_First, _Last);
    const std::size_t avxSize = sizeInBytes & ~size_t{ 0x1F };

    if (avxSize != 0) {
        ZeroUpperOnDelete _Guard;

        const __m256i _Comparand = _Traits_::SetAVX(_Val);
        const void* stopAt = _First;

        memory::AdvanceBytes(stopAt, avxSize);

        do {
            const __m256i data = _mm256_loadu_si256(static_cast<const __m256i*>(_First));
            const int bingo = _mm256_movemask_epi8(_Traits_::CompareAVX(data, _Comparand));

            if (bingo != 0) {
                const unsigned long _Offset = _tzcnt_u32(bingo);
                memory::AdvanceBytes(_First, _Offset);
                return _First;
            }

            memory::AdvanceBytes(_First, 32);
        } while (_First != stopAt);

        const size_t avxTailSize = sizeInBytes & 0x1C;

        if (avxTailSize != 0) {
            const __m256i tailMask = _Avx2_tail_mask_32(avxTailSize >> 2);
            const __m256i data = _mm256_maskload_epi32(static_cast<const int*>(_First), tailMask);
            const int bingo =
                _mm256_movemask_epi8(
                    _mm256_and_si256(
                        _Traits_::CompareAVX(data, _Comparand), tailMask));

            if (bingo != 0) {
                const unsigned long _Offset = _tzcnt_u32(bingo);
                memory::AdvanceBytes(_First, _Offset);
                return _First;
            }

            memory::AdvanceBytes(_First, avxTailSize);
        }

        if constexpr (sizeof(_Type_) >= 4)
            return _First;
    }
}


template <
    class _Traits_,
    class _Type_>
CONSTEXPR_CXX20 inline NODISCARD const void* FindAVX512(
    const void* _First,
    const void* _Last,
    _Type_      _Val)
{ 
    const auto sizeInBytes = memory::ByteLength(_First, _Last);
    const std::size_t avx512Size = sizeInBytes & ~std::size_t{0x3F};

    if (avx512Size != 0) {
      //  ZeroUpperOnDelete _Guard;

        const __m512i _Comparand = _Traits_::SetAvx512(_Val);
        const void* stopAt = _First;

        memory::AdvanceBytes(stopAt, avx512Size);
       
        do {
            const __m512i data = _mm512_loadu_si512(static_cast<const __m512i*>(_First));
            const int bingo = _Traits_::CompareAvx512(data, _Comparand);

            if (bingo != 0) {
                const unsigned long _Offset = _tzcnt_u32(bingo);
                memory::AdvanceBytes(_First, _Offset);
                return _First;
            }

            memory::AdvanceBytes(_First, 64);
        } while (_First != stopAt);

        const size_t avx512TailSize = sizeInBytes & 0x3F;

        if (avx512TailSize != 0) {
            const __m512i tailMask = _Avx512_tail_mask_64(avx512TailSize);
            const __m512i data = _mm512_maskz_loadu_epi8(tailMask, static_cast<const unsigned char*>(_First));

            const __mmask64 comparisonMask =
                _mm512_kand(_Traits_::CompareAVX512(data, _Comparand), tailMask); // Mask the comparison

            if (comparisonMask != 0) {
                const unsigned long _Offset = _tzcnt_u64(comparisonMask);
                memory::AdvanceBytes(_First, _Offset);
                return _First;
            }

            memory::AdvanceBytes(_First, avx512TailSize);
        }


        if constexpr (sizeof(_Type_) >= 4)
            return _First;
    }
}

__BASE_NAMESPACE_END
