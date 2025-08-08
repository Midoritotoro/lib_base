#pragma once

#include <base/core/arch/Platform.h>
#include <src/core/utility/simd/SimdTailMask.h>

#include <base/core/memory/MemoryUtility.h>

__BASE_NAMESPACE_BEGIN


void base_stdcall Replace(
    void*           _First,
    void* const     _Last,
    const uint32    _OldValue,
    const uint32    _NewValue) noexcept
{
    for (auto current = reinterpret_cast<uint32*>(_First); current != _Last; ++current)
        if (*current == _OldValue)
            *current = _NewValue;
}

base_declare_const_function void base_stdcall ReplaceAvx32Bit(
    void*           _First, 
    void* const     _Last, 
    const uint32    _OldValue, 
    const uint32    _NewValue) noexcept
{   
    const __m256i comparand = _mm256_broadcastd_epi32(
        _mm_cvtsi32_si128(_OldValue));
    const __m256i replacement = _mm256_broadcastd_epi32(
        _mm_cvtsi32_si128(_NewValue));

    const size_t fullLength = memory::ByteLength(_First, _Last);

    void* stopAt = _First;
    memory::AdvanceBytes(stopAt, fullLength & ~size_t{ 0x1F });

    while (_First != stopAt) {
        const __m256i data = _mm256_loadu_si256(
            reinterpret_cast<const __m256i*>(_First));
        const __m256i mask = _mm256_cmpeq_epi32(comparand, data);

        _mm256_maskstore_epi32(reinterpret_cast<int*>(_First), mask, replacement);
        memory::AdvanceBytes(_First, 32);
    }

    const size_t tailLength = fullLength & AVX_BYTE_ALIGNED_TAIL_MASK_UINT32;

    if (tailLength != 0) {
        const __m256i tailMask = Avx2TailMask32(BytesToDoubleWordsCount(tailLength));

        const __m256i data = _mm256_maskload_epi32(
            reinterpret_cast<const int*>(_First), tailMask);
        const __m256i mask = _mm256_and_si256(
            _mm256_cmpeq_epi32(comparand, data), tailMask);

        _mm256_maskstore_epi32(
            reinterpret_cast<int*>(_First), mask, replacement);
    }

    _mm256_zeroupper();
}

base_declare_const_function void base_stdcall ReplaceAvx64Bit(
    void*           _First, 
    void* const     _Last, 
    const uint64    _OldValue, 
    const uint64    _NewValue) noexcept 
{
#if defined(base_os_win64) || !defined(base_cpp_msvc)
    const __m256i comparand = _mm256_broadcastq_epi64(
        _mm_cvtsi64_si128(_OldValue));
    const __m256i replacement = _mm256_broadcastq_epi64(
        _mm_cvtsi64_si128(_NewValue));
#else // workaround, _mm_cvtsi64_si128 does not compile on x86 Windows using MSVC
    const __m256i comparand = _mm256_set1_epi64x(_OldValue);
    const __m256i replacement = _mm256_set1_epi64x(_NewValue);
#endif

    const auto fullLength = memory::ByteLength(_First, _Last);

    void* stopAt = _First;
    memory::AdvanceBytes(stopAt, fullLength & ~std::size_t{ 0x1F });

    while (_First != stopAt) {
        const auto data = _mm256_loadu_si256(
            reinterpret_cast<const __m256i*>(_First));
        const auto mask = _mm256_cmpeq_epi64(comparand, data);

        _mm256_maskstore_epi64(
            reinterpret_cast<long long*>(_First),
            mask, replacement);

        memory::AdvanceBytes(_First, 32);
    }
    
    const std::size_t tailLength = fullLength & AVX_BYTE_ALIGNED_TAIL_MASK_UINT64;

    if (tailLength != 0) {
        const auto tailMask = Avx2TailMask32(BytesToDoubleWordsCount(tailLength));
        const auto data = _mm256_maskload_epi64(
            reinterpret_cast<const long long*>(_First), tailMask);

        const __m256i mask = _mm256_and_si256(
            _mm256_cmpeq_epi64(comparand, data), tailMask);

        _mm256_maskstore_epi64(
            reinterpret_cast<long long*>(_First),
            mask, replacement);
    }

    _mm256_zeroupper();
}

base_declare_const_function void base_stdcall ReplaceAvx512_32Bit(
    void*           _First,
    void* const     _Last,
    const uint32    _OldValue,
    const uint32    _NewValue) noexcept
{
    const auto comparand    = _mm512_broadcastd_epi32(
        _mm_cvtsi32_si128(_OldValue));
    const auto replacement  = _mm512_broadcastd_epi32(
        _mm_cvtsi32_si128(_NewValue));

    const auto fullLength     = memory::ByteLength(_First, _Last);

    void* stopAt = _First;
    memory::AdvanceBytes(stopAt, fullLength & ~size_t{ 0x3F });

    while (_First != stopAt) {
        const auto data = _mm512_loadu_si512(
            reinterpret_cast<const __m512i*>(_First));
        const auto mask = _mm512_cmpeq_epi32_mask(comparand, data);

        _mm512_mask_store_epi32(_First, mask, replacement);

        memory::AdvanceBytes(_First, 64);
    }
    
    const size_t tailLength = fullLength & AVX512_BYTE_ALIGNED_TAIL_MASK_UINT32;

    if (tailLength != 0) {
        const auto tailMask = Avx512TailMask64(
            BytesToQuadWordsCount(tailLength));

        const auto data = _mm512_mask_load_epi32(
            _mm512_setzero_si512(), tailMask, _First);
        
        const auto mask = _mm512_cmpeq_epi32_mask(comparand, data) & tailMask;
        _mm512_mask_store_epi32(reinterpret_cast<int*>(_First), mask, replacement);
    }
}

base_declare_const_function void base_stdcall ReplaceAvx512_64Bit(
    void*           _First,
    void* const     _Last,
    const uint64    _OldValue,
    const uint64    _NewValue) noexcept
{
#if defined(base_os_win64) || !defined(base_cpp_msvc)
    const auto comparand = _mm512_broadcastd_epi32(
        _mm_cvtsi64_si128(_OldValue));
    const auto replacement = _mm512_broadcastd_epi32(
        _mm_cvtsi64_si128(_NewValue));
#else // workaround, _mm_cvtsi64_si128 does not compile on x86 Windows using MSVC
    const auto comparand = _mm512_set1_epi64(_OldValue);
    const auto replacement = _mm512_set1_epi64(_NewValue);
#endif

    const auto fullLength = memory::ByteLength(_First, _Last);

    void* stopAt = _First;
    memory::AdvanceBytes(stopAt, fullLength & ~size_t{ 0x3F });

    while (_First != stopAt) {
        const auto data = _mm512_loadu_epi64(
            reinterpret_cast<const __m512i*>(_First));
        const auto mask = _mm512_cmpeq_epi64_mask(comparand, data);

        _mm512_mask_store_epi64(_First, mask, replacement);
        memory::AdvanceBytes(_First, 64);
    }

    const size_t tailLength = fullLength & AVX512_BYTE_ALIGNED_TAIL_MASK_UINT64;

    if (tailLength != 0) {
        const auto tailMask = Avx512TailMask64(
            BytesToQuadWordsCount(tailLength));

        const auto data = _mm512_mask_load_epi64(
            _mm512_setzero_si512(), tailMask, _First);

        const auto mask = _mm512_cmpeq_epi64_mask(comparand, data) & tailMask;
        _mm512_mask_store_epi64(reinterpret_cast<int*>(_First), mask, replacement);
    }
}

__BASE_NAMESPACE_END
