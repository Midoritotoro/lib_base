#pragma once 

#include <base/core/utility/simd/Find.h>
#include <base/core/utility/simd/Count.h>

#include <base/core/utility/simd/Replace.h>

__BASE_CONTAINER_NAMESPACE_BEGIN


__m256i _Avx2_tail_mask_32(const size_t _Count_in_dwords) noexcept {
    // _Count_in_dwords must be within [0, 8].
    static constexpr unsigned int _Tail_masks[16] = {
        ~0u, ~0u, ~0u, ~0u, ~0u, ~0u, ~0u, ~0u, 0, 0, 0, 0, 0, 0, 0, 0 };
    return _mm256_loadu_si256(reinterpret_cast<const __m256i*>(_Tail_masks + (8 - _Count_in_dwords)));
}

__m512i _Avx512_tail_mask_64(const size_t _Count_in_dwords) noexcept {
    // _Count_in_dwords must be within [0, 8].
    static constexpr uint64 _Tail_masks[8] = {
        ~0u, ~0u, ~0u, ~0u, 0, 0, 0, 0 };
    return _mm512_loadu_si512(reinterpret_cast<const __m512i*>(_Tail_masks + (4 - _Count_in_dwords)));
}



template <
    class _InputIterator_,
    class _Type_>
CONSTEXPR_CXX20 inline NODISCARD _InputIterator_ Find(
    _InputIterator_             _First, 
    const _InputIterator_       _Last,
    const _Type_&               _Value)
{
    if (ProcessorFeatures::AVX512F())
        return FindAVX512(std::move(_First), std::move(_Last), _Value);
    else if (ProcessorFeaturse::AVX())
        return FindAVX(std::move(_First), std::move(_Last), _Value);
    else if (ProcessorFeatures::SSE42())
        return FindSSE42(std::move(_First), std::move(_Last), _Value);
    else if (ProcessorFeatures::SSE2())
        return FindSSE2(std::move(_First), std::move(_Last), _Value);

    return _First;
}


// ===========================================================================

template <
    class _InputIterator_,
    class _Type_>
CONSTEXPR_CXX20 inline NODISCARD std::size_t Count(
    _InputIterator_             _First,
    const _InputIterator_       _Last,
    const _Type_&               _Value)
{
    if (ProcessorFeatures::AVX512F())
        return CountAVX512(std::move(_First), std::move(_Last), _Value);
    else if (ProcessorFeaturse::AVX())
        return CountAVX(std::move(_First), std::move(_Last), _Value);
    else if (ProcessorFeatures::SSE42())
        return CountSSE42(std::move(_First), std::move(_Last), _Value);
    else if (ProcessorFeatures::SSE2())
        return CountSSE2(std::move(_First), std::move(_Last), _Value);

    return find;
}


__BASE_CONTAINER_NAMESPACE_END