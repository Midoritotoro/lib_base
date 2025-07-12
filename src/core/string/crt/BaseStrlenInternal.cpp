#include <src/core/string/crt/BaseStrlenInternal.h>
#include <base/core/utility/BitOps.h>


__BASE_STRING_NAMESPACE_BEGIN

template <CpuFeature feature, uint64 bytesCount>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes(const void* start) noexcept {
	static_assert(feature == CpuFeature::SSE || feature == CpuFeature::AVX || feature == CpuFeature::AVX512, "base::string::__checkForZeroBytes: Unsupported feature");
	static_assert(bytesCount == 1 || feature == 2 || feature == 4, "base::string::__checkForZeroBytes: Unsupported bytesCount");
	return -1;
}

template <CpuFeature feature, uint64 bytesCount>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes(__m128i vector) noexcept {
	static_assert(feature == CpuFeature::SSE || feature == CpuFeature::AVX || feature == CpuFeature::AVX512, "base::string::__checkForZeroBytes: Unsupported feature");
	static_assert(bytesCount == 1 || feature == 2 || feature == 4, "base::string::__checkForZeroBytes: Unsupported bytesCount");
	return -1;
}

template <CpuFeature feature, uint64 bytesCount>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes(__m256i vector) noexcept {
	static_assert(feature == CpuFeature::SSE || feature == CpuFeature::AVX || feature == CpuFeature::AVX512, "base::string::__checkForZeroBytes: Unsupported feature");
	static_assert(bytesCount == 1 || feature == 2 || feature == 4, "base::string::__checkForZeroBytes: Unsupported bytesCount");
	return -1;
}

template <CpuFeature feature, uint64 bytesCount>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes(__m512i vector) noexcept {
	static_assert(feature == CpuFeature::SSE || feature == CpuFeature::AVX || feature == CpuFeature::AVX512, "base::string::__checkForZeroBytes: Unsupported feature");
	static_assert(bytesCount == 1 || feature == 2 || feature == 4, "base::string::__checkForZeroBytes: Unsupported bytesCount");
	return -1;
}


template <>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes<CpuFeature::SSE, 1>(__m128i vector) noexcept {
	ZeroComparison comparison;

	comparison.mask				= _mm_movemask_epi8(_mm_cmpeq_epi8(vector, _mm_setzero_si128()));
	comparison.trailingZeros	= comparison.mask == 0 ? 0 : CountTrailingZeroBits(static_cast<uint16>(comparison.mask));

	return comparison;
}

template <>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes<CpuFeature::SSE, 2>(__m128i vector) noexcept {
	ZeroComparison comparison;

	comparison.mask				= _mm_movemask_epi8(_mm_cmpeq_epi16(vector, _mm_setzero_si128()));
	comparison.trailingZeros	= comparison.mask == 0 ? 0 : CountTrailingZeroBits(static_cast<uint8>(comparison.mask));

	return comparison;
}

template <>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes<CpuFeature::SSE, 4>(__m128i vector) noexcept {
	ZeroComparison comparison;

	comparison.mask				= _mm_movemask_epi8(_mm_cmpeq_epi32(vector, _mm_setzero_si128()));
	comparison.trailingZeros	= comparison.mask == 0 ? 0 : CountTrailingZeroBits(static_cast<uint8>(comparison.mask));

	return comparison;
}

template <>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes<CpuFeature::AVX, 1>(__m256i vector) noexcept {
	ZeroComparison comparison;

	comparison.mask				= _mm256_cmpeq_epi8_mask(vector, _mm256_setzero_si256());
	comparison.trailingZeros	= comparison.mask == 0 ? 0 : CountTrailingZeroBits(static_cast<uint32>(comparison.mask));

	return comparison;
}

template <>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes<CpuFeature::AVX, 2>(__m256i vector) noexcept {
	ZeroComparison comparison;

	comparison.mask				= _mm256_cmpeq_epi16_mask(vector, _mm256_setzero_si256());
	comparison.trailingZeros	= comparison.mask == 0 ? 0 : CountTrailingZeroBits(static_cast<uint16>(comparison.mask));

	return comparison;
}

template <>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes<CpuFeature::AVX, 4>(__m256i vector) noexcept {
	ZeroComparison comparison;

	comparison.mask				= _mm256_cmpeq_epi32_mask(vector, _mm256_setzero_si256());
	comparison.trailingZeros	= comparison.mask == 0 ? 0 : CountTrailingZeroBits(static_cast<uint8>(comparison.mask));

	return comparison;
}

template <>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes<CpuFeature::AVX512, 1>(__m512i vector) noexcept {
	ZeroComparison comparison;

	comparison.mask				= _mm512_cmpeq_epi8_mask(vector, _mm512_setzero_si512());
	comparison.trailingZeros	= comparison.mask == 0 ? 0 : CountTrailingZeroBits(static_cast<uint64>(comparison.mask));

	return comparison;
}

template <>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes<CpuFeature::AVX512, 2>(__m512i vector) noexcept {
	ZeroComparison comparison;

	comparison.mask				= _mm512_cmpeq_epi16_mask(vector, _mm512_setzero_si512());
	comparison.trailingZeros	= comparison.mask == 0 ? 0 : CountTrailingZeroBits(static_cast<uint32>(comparison.mask));

	return comparison;
}

template <>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes<CpuFeature::AVX512, 4>(__m512i vector) noexcept {
	ZeroComparison comparison;

	comparison.mask				= _mm512_cmpeq_epi32_mask(vector, _mm512_setzero_si512());
	comparison.trailingZeros	= comparison.mask == 0 ? 0 : CountTrailingZeroBits(static_cast<uint16>(comparison.mask));

	return comparison;
}

__BASE_STRING_NAMESPACE_END
