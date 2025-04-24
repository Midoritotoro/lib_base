#pragma once 

#include <base/core/arch/Platform.h>

#include <base/core/memory/MemoryRange.h>
#include <base/core/utility/simd/SimdAlgorithmSafety.h>

#include <base/core/utility/simd/SimdCountTraits.h>

__BASE_NAMESPACE_BEGIN

template <
    class _InputIterator_,
    class _Predicate_>
// Find first satisfying _Pred
CONSTEXPR_CXX20 inline NODISCARD std::size_t CountIf(
    _InputIterator_             _First,
    const _InputIterator_       _Last,
    _Predicate_                 _Pred)
{
    std::size_t _Count = 0;

    auto _UFirst = memory::UnFancy(_First);
    const auto _ULast = memory::UnFancy(_Last);

    for (; _UFirst != _ULast; ++_UFirst)
        if (_Pred(*_UFirst))
            ++_Count;

    return _Count;
}

template <
    class _InputIterator_,
    class _Type_>
CONSTEXPR_CXX20 inline NODISCARD std::size_t CountSSE2(
    _InputIterator_             _First,
    const _InputIterator_       _Last,
    const _Type_&               _Value)
{

}


template <
    class _InputIterator_,
    class _Type_>
CONSTEXPR_CXX20 inline NODISCARD std::size_t CountSSE42(
    _InputIterator_             _First,
    const _InputIterator_       _Last,
    const _Type_&               _Value)
{

}


template <
    class _Traits_,
    class _Type_>
CONSTEXPR_CXX20 inline NODISCARD std::size_t CountAVX(
    const void*         _First, 
    const void* const   _Last,
    const _Type_        _Val) noexcept
{
    size_t _Result = 0;

    const size_t _Size_bytes = memory::ByteLength(_First, _Last);
    size_t _Avx_size = _Size_bytes & ~size_t{ 0x1F };

    if (_Avx_size != 0) {
        const __m256i _Comparand = _Traits_::SetAvx(_Val);
        const void* _Stop_at = _First;

        for (;;) {
            if constexpr (sizeof(_Type_) >= sizeof(size_t)) {
                memory::AdvanceBytes(_Stop_at, _Avx_size);
            }
            else {
                constexpr size_t _Max_portion_size = _Traits_::maxCount * 32;
                const size_t _Portion_size = _Avx_size < _Max_portion_size 
                    ? _Avx_size 
                    : _Max_portion_size;

                memory::AdvanceBytes(_Stop_at, _Portion_size);
                _Avx_size -= _Portion_size;
            }

            __m256i _Count_vector = _mm256_setzero_si256();

            do {
                const __m256i _Data = _mm256_loadu_si256(static_cast<const __m256i*>(_First));
                const __m256i _Mask = _Traits_::CompareAvx(_Data, _Comparand);

                _Count_vector = _Traits_::SubstractAvx(_Count_vector, _Mask);
                memory::AdvanceBytes(_First, 32);
            } while (_First != _Stop_at);

            _Result += _Traits_::ReduceAvx(_Count_vector);

            if constexpr (sizeof(_Type_) >= sizeof(size_t))
                break;
            else if (_Avx_size == 0)
                break;
        }

        if (const size_t _Avx_tail_size = _Size_bytes & 0x1C; _Avx_tail_size != 0) {
            const __m256i _Tail_mask = _Avx2_tail_mask_32(_Avx_tail_size >> 2);
            const __m256i _Data = _mm256_maskload_epi32(static_cast<const int*>(_First), _Tail_mask);
            const __m256i _Mask = _mm256_and_si256(_Traits::_Cmp_avx(_Data, _Comparand), _Tail_mask);
            const int _Bingo = _mm256_movemask_epi8(_Mask);
            const size_t _Tail_count = __popcnt(_Bingo); // Assume available with SSE4.2
            _Result += _Tail_count / sizeof(_Ty);
            _Advance_bytes(_First, _Avx_tail_size);
        }

        _mm256_zeroupper(); // TRANSITION, DevCom-10331414

        if constexpr (sizeof(_Ty) >= 4) {
            return _Result;
        }
    }
    else if (size_t _Sse_size = _Size_bytes & ~size_t{ 0xF }; _Sse_size != 0 && _Use_sse42()) {
        const __m128i _Comparand = _Traits::_Set_sse(_Val);
        const void* _Stop_at = _First;

        for (;;) {
            if constexpr (sizeof(_Ty) >= sizeof(size_t)) {
                _Advance_bytes(_Stop_at, _Sse_size);
            }
            else {
                constexpr size_t _Max_portion_size = _Traits::_Max_count * 16;
                const size_t _Portion_size = _Sse_size < _Max_portion_size ? _Sse_size : _Max_portion_size;
                _Advance_bytes(_Stop_at, _Portion_size);
                _Sse_size -= _Portion_size;
            }

            __m128i _Count_vector = _mm_setzero_si128();

            do {
                const __m128i _Data = _mm_loadu_si128(static_cast<const __m128i*>(_First));
                const __m128i _Mask = _Traits::_Cmp_sse(_Data, _Comparand);
                _Count_vector = _Traits::_Sub_sse(_Count_vector, _Mask);
                _Advance_bytes(_First, 16);
            } while (_First != _Stop_at);

            _Result += _Traits::_Reduce_sse(_Count_vector);

            if constexpr (sizeof(_Ty) >= sizeof(size_t)) {
                break;
            }
            else {
                if (_Sse_size == 0) {
                    break;
                }
            }
        }
    }
}


template <
    class _InputIterator_,
    class _Type_>
CONSTEXPR_CXX20 inline NODISCARD std::size_t CountAVX512(
    _InputIterator_             _First,
    const _InputIterator_       _Last,
    const _Type_&               _Value)
{   

}

__BASE_NAMESPACE_END