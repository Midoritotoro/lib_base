#pragma once

#include <base/core/arch/Platform.h>
#include <base/core/memory/MemoryRange.h>

#include <base/core/utility/simd/SimdTailMask.h>

__BASE_NAMESPACE_BEGIN


void STDCALL Replace(
    void*           _First,
    void* const     _Last,
    const uint32    _OldValue,
    const uint32    _NewValue) noexcept
{
    for (auto current = reinterpret_cast<uint32*>(_First); current != _Last; ++current)
        if (*current == _OldValue)
            *current = _NewValue;
}

DECLARE_NOALIAS void STDCALL ReplaceAvx32Bit(
    void*           _First, 
    void* const     _Last, 
    const uint32    _OldValue, 
    const uint32    _NewValue) noexcept
{   
    const __m256i comparand = _mm256_broadcastd_epi32(_mm_cvtsi32_si128(_OldValue));
    const __m256i replacement = _mm256_broadcastd_epi32(_mm_cvtsi32_si128(_NewValue));

    const size_t fullLength = memory::ByteLength(_First, _Last);

    void* stopAt = _First;
    memory::AdvanceBytes(stopAt, fullLength & ~size_t{ 0x1F });

    while (_First != stopAt) {
        const __m256i data = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(_First));
        const __m256i mask = _mm256_cmpeq_epi32(comparand, data);

        _mm256_maskstore_epi32(reinterpret_cast<int*>(_First), mask, replacement);
        memory::AdvanceBytes(_First, 32);
    }

    const size_t tailLength = fullLength & 0x1C;

    if (tailLength != 0) {
        const __m256i tailMask = Avx2TailMask32(tailLength >> 2);

        const __m256i data = _mm256_maskload_epi32(reinterpret_cast<const int*>(_First), tailMask);
        const __m256i mask = _mm256_and_si256(_mm256_cmpeq_epi32(comparand, data), tailMask);

        _mm256_maskstore_epi32(reinterpret_cast<int*>(_First), mask, replacement);
    }

    _mm256_zeroupper();
}

DECLARE_NOALIAS void STDCALL ReplaceAvx64Bit(
    void*           _First, 
    void* const     _Last, 
    const uint64    _OldValue, 
    const uint64    _NewValue) noexcept 
{
#if defined(OS_WIN64) || !defined(CPP_MSVC)
    const __m256i comparand = _mm256_broadcastq_epi64(_mm_cvtsi64_si128(_OldValue));
    const __m256i replacement = _mm256_broadcastq_epi64(_mm_cvtsi64_si128(_NewValue));
#else // workaround, _mm_cvtsi64_si128 does not compile on x86 Windows using MSVC
    const __m256i comparand = _mm256_set1_epi64x(_OldValue);
    const __m256i replacement = _mm256_set1_epi64x(_NewValue);
#endif
    const size_t fullLength = memory::ByteLength(_First, _Last);

    void* stopAt = _First;
    memory::AdvanceBytes(stopAt, fullLength & ~size_t{ 0x1F });

    while (_First != stopAt) {
        const __m256i data = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(_First));
        const __m256i mask = _mm256_cmpeq_epi64(comparand, data);

        _mm256_maskstore_epi64(reinterpret_cast<long long*>(_First), mask, replacement);

        memory::AdvanceBytes(_First, 32);
    }

    const size_t tailLength = fullLength & 0x18;

    if (tailLength != 0) {
        const __m256i tailMask = Avx2TailMask32(tailLength >> 2);
        const __m256i data = _mm256_maskload_epi64(reinterpret_cast<const long long*>(_First), tailMask);

        const __m256i mask = _mm256_and_si256(_mm256_cmpeq_epi64(comparand, data), tailMask);
        _mm256_maskstore_epi64(reinterpret_cast<long long*>(_First), mask, replacement);
    }

    _mm256_zeroupper();
}

DECLARE_NOALIAS void STDCALL ReplaceAvx512_32Bit(
    void*           _First,
    void* const     _Last,
    const uint32    _OldValue,
    const uint32    _NewValue) noexcept
{
    const __m512i comparand     = _mm512_broadcastd_epi32(_mm_cvtsi32_si128(_OldValue));
    const __m512i replacement   = _mm512_broadcastd_epi32(_mm_cvtsi32_si128(_NewValue));
        
    const size_t fullLength     = memory::ByteLength(_First, _Last);

    void* stopAt = _First;
    memory::AdvanceBytes(stopAt, fullLength & ~size_t{ 0x3F });

    while (_First != stopAt) {
        const __m512i data      = _mm512_loadu_si512(reinterpret_cast<const __m512i*>(_First));
        const __mmask64 mask    = _mm512_cmpeq_epi32_mask(comparand, data);

        _mm512_mask_store_epi32(reinterpret_cast<int*>(_First), mask, replacement);
        memory::AdvanceBytes(_First, 64);
    }

    const size_t tailLength = fullLength & 0x1C;

    if (tailLength != 0) {
        const __m512i tailMask = Avx512TailMask64(tailLength >> 2);
        
        __m512i data = _mm512_set1_epi32(0);
        _mm512_mask_load_epi32(reinterpret_cast<__m512i*>(_First), _mm512_mask_tailMask, data);
        const __m512i mask = _mm512_and_si512(_mm512_cmpeq_epi32(comparand, data), tailMask);

        _mm256_maskstore_epi32(reinterpret_cast<int*>(_First), mask, replacement);
    }

    _mm256_zeroupper();
}
__BASE_NAMESPACE_END
