#include <src/core/utility/simd/traits/SimdFindTraits.h>

__BASE_NAMESPACE_BEGIN

  __m512i FindTraits8Bit::SetAvx512(const uint8 value) noexcept {
    return _mm512_set1_epi8(value);
}

  __m256i FindTraits8Bit::SetAvx(const uint8 value) noexcept {
    return _mm256_set1_epi8(value);
}

  __m128i FindTraits8Bit::SetSse(const uint8 value) noexcept {
    return _mm_shuffle_epi8(
        _mm_cvtsi32_si128(value),
        _mm_setzero_si128());
}

  __mmask64 FindTraits8Bit::CompareAvx512(
    const __m512i left,
    const __m512i right) noexcept
{
    return _mm512_cmpeq_epi8_mask(left, right);
}

  __m256i FindTraits8Bit::CompareAvx(
    const __m256i left,
    const __m256i right) noexcept
{
    return _mm256_cmpeq_epi8(left, right);
}

  __m128i FindTraits8Bit::CompareSse(
    const __m128i left,
    const __m128i right) noexcept
{
    return _mm_cmpeq_epi8(left, right);
}


// ========================================================================
// ========================================================================
// ========================================================================


  __m512i FindTraits16Bit::SetAvx512(const uint16 value) noexcept {
    return _mm512_set1_epi16(value);
}

  __m256i FindTraits16Bit::SetAvx(const uint16 value) noexcept {
    return _mm256_set1_epi16(value);
}

  __m128i FindTraits16Bit::SetSse(const uint16 value) noexcept {
    return _mm_set1_epi16(value);
}

  __mmask32 FindTraits16Bit::CompareAvx512(
    const __m512i left,
    const __m512i right) noexcept
{
    return _mm512_cmpeq_epi16_mask(left, right);
}

  __m256i FindTraits16Bit::CompareAvx(
    const __m256i left,
    const __m256i right) noexcept
{
    return _mm256_cmpeq_epi16(left, right);
}

  __m128i FindTraits16Bit::CompareSse(
    const __m128i left,
    const __m128i right) noexcept
{
    return _mm_cmpeq_epi16(left, right);
}


// ========================================================================
// ========================================================================
// ========================================================================


  __m512i FindTraits32Bit::SetAvx512(const uint32 value) noexcept {
    return _mm512_set1_epi32(value);
}

  __m256i FindTraits32Bit::SetAvx(const uint32 value) noexcept {
    return _mm256_set1_epi32(value);
}

  __m128i FindTraits32Bit::SetSse(const uint32 value) noexcept {
    return _mm_set1_epi32(value);
}

  __mmask16 FindTraits32Bit::CompareAvx512(
    const __m512i left,
    const __m512i right) noexcept
{
    return _mm512_cmpeq_epi32_mask(left, right);
}

  __m256i FindTraits32Bit::CompareAvx(
    const __m256i left,
    const __m256i right) noexcept
{
    return _mm256_cmpeq_epi32(left, right);
}

  __m128i FindTraits32Bit::CompareSse(
    const __m128i left,
    const __m128i right) noexcept
{
    return _mm_cmpeq_epi32(left, right);
}


// ========================================================================
// ========================================================================
// ========================================================================


  __m512i FindTraits64Bit::SetAvx512(const uint64 value) noexcept {
    return _mm512_set1_epi64(value);
}

  __m256i FindTraits64Bit::SetAvx(const uint64 value) noexcept {
    return _mm256_set1_epi64x(value);
}

  __m128i FindTraits64Bit::SetSse(const uint64 value) noexcept {
    return _mm_set1_epi64x(value);
}

  __mmask8 FindTraits64Bit::CompareAvx512(
    const __m512i left,
    const __m512i right) noexcept
{
    return _mm512_cmpeq_epi64_mask(left, right);
}

  __m256i FindTraits64Bit::CompareAvx(
    const __m256i left,
    const __m256i right) noexcept
{
    return _mm256_cmpeq_epi64(left, right);
}

  __m128i FindTraits64Bit::CompareSse(
    const __m128i left,
    const __m128i right) noexcept
{
    return _mm_cmpeq_epi64(left, right);
}

__BASE_NAMESPACE_END
