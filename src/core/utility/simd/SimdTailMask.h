#pragma once 

#include <base/core/arch/Platform.h>
#include <base/core/math/Math.h>

__BASE_NAMESPACE_BEGIN

// Masks for determining the maximum number of remaining bytes to process in the tail of a data buffer,
// where the remaining bytes are byte-aligned and a multiple of either 4 (dword/AVX) or 8 (qword/AVX512) bytes.

#define AVX_BYTE_ALIGNED_TAIL_MASK_UINT32       (0x1C)
#define AVX_BYTE_ALIGNED_TAIL_MASK_UINT64       (0x18)

#define AVX512_BYTE_ALIGNED_TAIL_MASK_UINT64    (0x38)
#define AVX512_BYTE_ALIGNED_TAIL_MASK_UINT32    (0x30)

base_nodiscard __m256i inline Avx2TailMask32(const std::size_t countInDwords) noexcept {
    // countInDwords must be within [0, 8].
    static constexpr unsigned int tailMasks[16] = {
        ~0u, ~0u, ~0u, ~0u, ~0u, ~0u, ~0u, ~0u, 0, 0, 0, 0, 0, 0, 0, 0 };
    return _mm256_loadu_si256(
        reinterpret_cast<const __m256i*>(
            tailMasks + (8 - countInDwords)));
}

constexpr base_nodiscard __mmask16 base_always_inline Avx512TailMask64(const std::size_t countInQWords) noexcept {
    // countInQWords must be within [0, 8].
    return (1ULL << countInQWords) - 1;
}

__BASE_NAMESPACE_END