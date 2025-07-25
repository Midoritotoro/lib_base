#include <base/core/arch/SimdHelpers.h>


__BASE_NAMESPACE_BEGIN

DECLARE_NOALIAS always_inline NODISCARD __mmask16 ZeroByteMask(const __m512i vector) noexcept {
    const auto vector01     = _mm512_set1_epi8(0x01);
    const auto vector80     = _mm512_set1_epi8(int8(0x80));

    const auto vector1      = _mm512_sub_epi32(vector, vector01);
    // tempVector1 = (vector - 0x01010101) & ~vector & 0x80808080
    const auto tempVector1  = _mm512_ternarylogic_epi32(vector1, vector, vector80, 0x20);

    return _mm512_test_epi32_mask(tempVector1, tempVector1);
}

DECLARE_NOALIAS always_inline NODISCARD bool IsXmmZero(__m128i xmmRegister) noexcept {
	return _mm_movemask_epi8(_mm_cmpeq_epi64(xmmRegister, _mm_setzero_si128())) == 0xFFFFFFFFFFFFFFFF;
}

DECLARE_NOALIAS always_inline NODISCARD bool IsYmmZero(__m256i ymmRegister) noexcept {
	return _mm256_cmpeq_epi64_mask(ymmRegister, _mm256_setzero_si256()) == 0xFFFFFFFFFFFFFFFF;
}

DECLARE_NOALIAS always_inline NODISCARD bool IsZmmZero(__m512i zmmRegister) noexcept {
	return _mm512_cmpeq_epi64_mask(zmmRegister, _mm512_setzero_si512()) == 0xFFFFFFFFFFFFFFFF;
}

__BASE_NAMESPACE_END
