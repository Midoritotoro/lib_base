#include <src/core/utility/simd/traits/SimdCountTraits.h>

__BASE_NAMESPACE_BEGIN

  __m512i CountTraits64Bit::SubstractAvx512(
    const __m512i left,
    const __m512i right) noexcept
{
    return _mm512_sub_epi64(left, right);
}

  __m256i CountTraits64Bit::SubstractAvx(
    const __m256i left,
    const __m256i right) noexcept
{
    return _mm256_sub_epi64(left, right);
}

  __m128i CountTraits64Bit::SubstractSse(
    const __m128i left,
    const __m128i right) noexcept
{
    return _mm_sub_epi64(left, right);
}

  std::size_t CountTraits64Bit::ReduceAvx512(const __m512i value) noexcept {
    return 0;
}

  std::size_t CountTraits64Bit::ReduceAvx(const __m256i value) noexcept {
    const __m128i _Lo64 = _mm256_extracti128_si256(value, 0);
    const __m128i _Hi64 = _mm256_extracti128_si256(value, 1);

    const __m128i _Rx8 = _mm_add_epi64(_Lo64, _Hi64);

    return ReduceSse(_Rx8);
}

  std::size_t CountTraits64Bit::ReduceSse(const __m128i value) noexcept {
#ifdef base_processor_x86_32
    return static_cast<uint32>(
        _mm_cvtsi128_si32(value)) + static_cast<uint32>(
            _mm_extract_epi32(value, 2));
#else
    return _mm_cvtsi128_si64(value) + _mm_extract_epi64(value, 1);
#endif
}


// ==================================================================
// ==================================================================
// ==================================================================


  __m512i CountTraits32Bit::SubstractAvx512(
    const __m512i left,
    const __m512i right) noexcept
{
    return _mm512_sub_epi32(left, right);
}

  __m256i CountTraits32Bit::SubstractAvx(
    const __m256i left,
    const __m256i right) noexcept
{
    return _mm256_sub_epi32(left, right);
}

  __m128i CountTraits32Bit::SubstractSse(
    const __m128i left,
    const __m128i right) noexcept
{
    return _mm_sub_epi32(left, right);
}

  std::size_t CountTraits32Bit::ReduceAvx512(const __m512i value) noexcept {
    return 0;
}

  std::size_t CountTraits32Bit::ReduceAvx(const __m256i value) noexcept {
    constexpr auto _Shuf = _MM_SHUFFLE(3, 1, 2, 0); // Cross lane, to reduce further on low lane

    const __m256i _Rx4 = _mm256_hadd_epi32(value, _mm256_setzero_si256()); // (0+1),(2+3),0,0 per lane
    const __m256i _Rx5 = _mm256_permute4x64_epi64(_Rx4, _Shuf); // low lane  (0+1),(2+3),(4+5),(6+7)

    const __m256i _Rx6 = _mm256_hadd_epi32(_Rx5, _mm256_setzero_si256()); // (0+...+3),(4+...+7),0,0
    const __m256i _Rx7 = _mm256_hadd_epi32(_Rx6, _mm256_setzero_si256()); // (0+...+7),0,0,0

    return static_cast<uint32_t>(_mm_cvtsi128_si32(_mm256_castsi256_si128(_Rx7)));
}

  std::size_t CountTraits32Bit::ReduceSse(const __m128i value) noexcept {
    const __m128i _Rx4 = _mm_hadd_epi32(value, _mm_setzero_si128()); // (0+1),(2+3),0,0
    const __m128i _Rx5 = _mm_hadd_epi32(_Rx4, _mm_setzero_si128()); // (0+...+3),0,0,0

    return static_cast<uint32_t>(_mm_cvtsi128_si32(_Rx5));
}


// ==================================================================
// ==================================================================
// ==================================================================


  __m512i CountTraits16Bit::SubstractAvx512(
    const __m512i left,
    const __m512i right) noexcept
{
    return _mm512_sub_epi16(left, right);

}
  __m256i CountTraits16Bit::SubstractAvx(
    const __m256i left,
    const __m256i right) noexcept
{
    return _mm256_sub_epi16(left, right);
}

  __m128i CountTraits16Bit::SubstractSse(
    const __m128i left,
    const __m128i right) noexcept
{
    return _mm_sub_epi16(left, right);
}

  std::size_t CountTraits16Bit::ReduceAvx512(const __m512i value) noexcept {
    return 0;
}

  std::size_t CountTraits16Bit::ReduceAvx(const __m256i value) noexcept {
    const __m256i _Rx2 = _mm256_hadd_epi16(value, _mm256_setzero_si256());
    const __m256i _Rx3 = _mm256_unpacklo_epi16(_Rx2, _mm256_setzero_si256());

    return CountTraits32Bit::ReduceAvx(_Rx3);
}

  std::size_t CountTraits16Bit::ReduceSse(const __m128i value) noexcept {
    const __m128i _Rx2 = _mm_hadd_epi16(value, _mm_setzero_si128());
    const __m128i _Rx3 = _mm_unpacklo_epi16(_Rx2, _mm_setzero_si128());

    return CountTraits32Bit::ReduceSse(_Rx3);
}


// ==================================================================
// ==================================================================
// ==================================================================


  __m512i CountTraits8Bit::SubstractAvx512(
    const __m512i left,
    const __m512i right) noexcept
{
    return _mm512_sub_epi8(left, right);
}

  __m256i CountTraits8Bit::SubstractAvx(
    const __m256i left,
    const __m256i right) noexcept
{
    return _mm256_sub_epi8(left, right);
}

  __m128i CountTraits8Bit::SubstractSse(
    const __m128i left,
    const __m128i right) noexcept
{
    return _mm_sub_epi8(left, right);
}

  std::size_t CountTraits8Bit::ReduceAvx512(const __m512i value) noexcept {
    return 0;
}

  std::size_t CountTraits8Bit::ReduceAvx(const __m256i value) noexcept {
    const __m256i _Rx1 = _mm256_sad_epu8(value, _mm256_setzero_si256());
    return CountTraits64Bit::ReduceAvx(_Rx1);
}

  std::size_t CountTraits8Bit::ReduceSse(const __m128i value) noexcept {
    const __m128i _Rx1 = _mm_sad_epu8(value, _mm_setzero_si128());
    return CountTraits64Bit::ReduceSse(_Rx1);
}

__BASE_NAMESPACE_END
