#pragma once 

#include <base/core/arch/Platform.h>
#include <base/core/arch/ProcessorFeatures.h>

#include <base/core/memory/MemoryRange.h>
#include <src/core/utility/simd/SimdAlgorithmSafety.h>

#include <src/core/utility/simd/SimdCountTraits.h>

__BASE_NAMESPACE_BEGIN

template <
    class _InputIterator_,
    class _Predicate_>
// Find first satisfying _Pred
CONSTEXPR_CXX20 inline NODISCARD std::size_t CountIf(
    _InputIterator_             _First,
    const _InputIterator_       _Last,
    _Predicate_                 _Pred) noexcept
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
    //size_t result = 0;

    //const auto sizeInBytes = memory::ByteLength(_First, _Last);
    //size_t avxSize = sizeInBytes & ~size_t{ 0x1F };

    //if (avxSize != 0) {
    //    const auto comparand = _Traits_::SetAvx(_Val);
    //    const void* stopAt = _First;

    //    for (;;) {
    //        if constexpr (sizeof(_Type_) >= sizeof(size_t)) {
    //            memory::AdvanceBytes(stopAt, avxSize);
    //        }
    //        else {
    //            constexpr size_t maxPortionSize = _Traits_::maxCount * 32;
    //            const size_t portionSize = avxSize < maxPortionSize
    //                ? avxSize 
    //                : maxPortionSize;

    //            memory::AdvanceBytes(stopAt, portionSize);
    //            avxSize -= portionSize;
    //        }

    //        auto countVector = _mm256_setzero_si256();

    //        do {
    //            const auto _Data = _mm256_loadu_si256(static_cast<const __m256i*>(_First));
    //            const auto _Mask = _Traits_::CompareAvx(_Data, _Comparand);

    //            _Count_vector = _Traits_::SubstractAvx(_Count_vector, _Mask);
    //            memory::AdvanceBytes(_First, 32);
    //        } while (_First != stopAt);

    //        _Result += _Traits_::ReduceAvx(_Count_vector);

    //        if constexpr (sizeof(_Type_) >= sizeof(size_t))
    //            break;
    //        else if (_Avx_size == 0)
    //            break;
    //    }

    //    const size_t avxTailSize = sizeInBytes & 0x1C;

    //    if (avxTailSize != 0) {
    //        const __m256i _Tail_mask = AvxTailMask32(avxTailSize >> 2);
    //        const __m256i _Data = _mm256_maskload_epi32(static_cast<const int*>(_First), _Tail_mask);
    //        const __m256i _Mask = _mm256_and_si256(_Traits::_Cmp_avx(_Data, _Comparand), _Tail_mask);
    //        const int _Bingo = _mm256_movemask_epi8(_Mask);
    //        const size_t _Tail_count = __popcnt(_Bingo); // Assume available with SSE4.2
    //        _Result += _Tail_count / sizeof(_Ty);
    //        _Advance_bytes(_First, _Avx_tail_size);
    //    }

    //    _mm256_zeroupper(); // TRANSITION, DevCom-10331414

    //    if constexpr (sizeof(_Ty) >= 4) {
    //        return _Result;
    //    }
    //}
}


template <
    class _Traits_,
    class _Type_>
CONSTEXPR_CXX20 inline NODISCARD std::size_t CountAVX512(
    void*               _First,
    const void* const   _Last,
    const _Type_&       _Value) 
{   

}

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
    class _InputIterator_,
    class _Type_>
CONSTEXPR_CXX20 inline NODISCARD std::size_t Count(
    _InputIterator_             _First,
    const _InputIterator_       _Last,
    const _Type_&               _Value)
{
    if (ProcessorFeatures::AVX512F())
        return CountAVX512(
            memory::UnFancy(std::move(_First)),
            memory::UnFancy(std::move(_Last)),
            _Value);
    else if (ProcessorFeatures::AVX())
        return CountAVX(
            memory::UnFancy(std::move(_First)),
            memory::UnFancy(std::move(_Last)), 
            _Value);
    else if (ProcessorFeatures::SSE42())
        return CountSSE42(
            memory::UnFancy(std::move(_First)), 
            memory::UnFancy(std::move(_Last)),
            _Value);

    return CountScalar(
        memory::UnFancy(std::move(_First)), 
        memory::UnFancy(std::move(_Last)), 
        _Value);
}

__BASE_NAMESPACE_END