#pragma once 

#include <base/core/arch/Platform.h>

__BASE_NAMESPACE_BEGIN

NODISCARD __m256i Avx2TailMask32(const std::size_t countInDwords) noexcept {
    // _Count_in_dwords must be within [0, 8].
    static constexpr unsigned int tailMasks[16] = {
        ~0u, ~0u, ~0u, ~0u, ~0u, ~0u, ~0u, ~0u, 0, 0, 0, 0, 0, 0, 0, 0 };
    return _mm256_loadu_si256(
        reinterpret_cast<const __m256i*>(
            tailMasks + (8 - countInDwords)));
}

NODISCARD __mmask16 Avx512TailMask64(const std::size_t countInQWords) noexcept {
    if (countInQWords > 8)
        return 0;

    return (1ULL << countInQWords) - 1;
}

__BASE_NAMESPACE_END