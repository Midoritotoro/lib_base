#pragma once 

#include <src/core/utility/simd/Find.h>
#include <src/core/utility/simd/Count.h>

#include <src/core/utility/simd/Replace.h>

__BASE_CONTAINER_NAMESPACE_BEGIN


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
    else if (ProcessorFeatures::AVX())
        return CountAVX(std::move(_First), std::move(_Last), _Value);
    else if (ProcessorFeatures::SSE42())
        return CountSSE42(std::move(_First), std::move(_Last), _Value);
    else if (ProcessorFeatures::SSE2())
        return CountSSE2(std::move(_First), std::move(_Last), _Value);

    return find;
}


__BASE_CONTAINER_NAMESPACE_END