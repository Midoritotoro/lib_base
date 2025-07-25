#include <src/core/string/crt/BaseStrlenCheckForZeroBytes.h>
#include <base/core/utility/BitOps.h>


__BASE_STRING_NAMESPACE_BEGIN

template <arch::CpuFeature feature, uint64 bytesCount>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes(const void* start) noexcept {
	static_assert(feature == arch::CpuFeature::SSE2 || feature == arch::CpuFeature::AVX2 || feature == arch::CpuFeature::AVX512, "base::string::__checkForZeroBytes: Unsupported feature");
	static_assert(bytesCount == 1 || bytesCount == 2 || bytesCount == 4, "base::string::__checkForZeroBytes: Unsupported bytesCount");
	return -1;
}

template <arch::CpuFeature feature, uint64 bytesCount>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes(__m128i vector) noexcept {
	static_assert(feature == arch::CpuFeature::SSE2 || feature == arch::CpuFeature::AVX2 || feature == arch::CpuFeature::AVX512BW, "base::string::__checkForZeroBytes: Unsupported feature");
	static_assert(bytesCount == 1 || bytesCount == 2 || bytesCount == 4, "base::string::__checkForZeroBytes: Unsupported bytesCount");
	return -1;
}

template <arch::CpuFeature feature, uint64 bytesCount>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes(__m256i vector) noexcept {
	static_assert(feature == arch::CpuFeature::SSE2 || feature == arch::CpuFeature::AVX2 || feature == arch::CpuFeature::AVX512BW, "base::string::__checkForZeroBytes: Unsupported feature");
	static_assert(bytesCount == 1 || bytesCount == 2 || bytesCount == 4, "base::string::__checkForZeroBytes: Unsupported bytesCount");
	return -1;
}

template <arch::CpuFeature feature, uint64 bytesCount>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes(__m512i vector) noexcept {
	static_assert(feature == arch::CpuFeature::SSE2 || feature == arch::CpuFeature::AVX2 || feature == arch::CpuFeature::AVX512BW, "base::string::__checkForZeroBytes: Unsupported feature");
	static_assert(bytesCount == 1 || bytesCount == 2 || bytesCount == 4, "base::string::__checkForZeroBytes: Unsupported bytesCount");
	return -1;
}


template <>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes<arch::CpuFeature::SSE2, 1>(__m128i vector) noexcept {
	ZeroComparison comparison;

	comparison.mask				= _mm_movemask_epi8(_mm_cmpeq_epi8(vector, _mm_setzero_si128()));
	comparison.trailingZeros	= comparison.mask == 0 ? 0 : CountTrailingZeroBits(static_cast<uint16>(comparison.mask));

	return comparison;
}

template <>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes<arch::CpuFeature::SSE2, 2>(__m128i vector) noexcept {
	ZeroComparison comparison;

	comparison.mask				= _mm_movemask_epi8(_mm_cmpeq_epi16(vector, _mm_setzero_si128()));
	comparison.trailingZeros	= comparison.mask == 0 ? 0 : CountTrailingZeroBits(static_cast<uint8>(comparison.mask));

	return comparison;
}

template <>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes<arch::CpuFeature::SSE2, 4>(__m128i vector) noexcept {
	ZeroComparison comparison;

	comparison.mask				= _mm_movemask_epi8(_mm_cmpeq_epi32(vector, _mm_setzero_si128()));
	comparison.trailingZeros	= comparison.mask == 0 ? 0 : CountTrailingZeroBits(static_cast<uint8>(comparison.mask));

	return comparison;
}



template <>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes<arch::CpuFeature::AVX2, 1>(__m256i vector) noexcept {
	ZeroComparison comparison;

	comparison.mask				= _mm256_movemask_epi8(_mm256_cmpeq_epi8(vector, _mm256_setzero_si256()));
	comparison.trailingZeros	= comparison.mask == 0 ? 0 : CountTrailingZeroBits(static_cast<uint32>(comparison.mask));

	return comparison;
}

template <>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes<arch::CpuFeature::AVX2, 2>(__m256i vector) noexcept {
	ZeroComparison comparison;

	comparison.mask				= _mm256_movemask_epi8(_mm256_cmpeq_epi16(vector, _mm256_setzero_si256()));
	comparison.trailingZeros	= comparison.mask == 0 ? 0 : CountTrailingZeroBits(static_cast<uint16>(comparison.mask));

	return comparison;
}

template <>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes<arch::CpuFeature::AVX2, 4>(__m256i vector) noexcept {
	ZeroComparison comparison;

	comparison.mask				= _mm256_movemask_epi8(_mm256_cmpeq_epi32(vector, _mm256_setzero_si256()));
	comparison.trailingZeros	= comparison.mask == 0 ? 0 : CountTrailingZeroBits(static_cast<uint8>(comparison.mask));

	return comparison;
}



template <>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes<arch::CpuFeature::AVX512BW, 1>(__m512i vector) noexcept {
	ZeroComparison comparison;

	comparison.mask				= _mm512_cmpeq_epi8_mask(vector, _mm512_setzero_si512());
	comparison.trailingZeros	= comparison.mask == 0 ? 0 : CountTrailingZeroBits(static_cast<uint64>(comparison.mask));

	return comparison;
}

template <>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes<arch::CpuFeature::AVX512BW, 2>(__m512i vector) noexcept {
	ZeroComparison comparison;

	comparison.mask				= _mm512_cmpeq_epi16_mask(vector, _mm512_setzero_si512());
	comparison.trailingZeros	= comparison.mask == 0 ? 0 : CountTrailingZeroBits(static_cast<uint32>(comparison.mask));

	return comparison;
}

template <>
DECLARE_NOALIAS NODISCARD ZeroComparison __checkForZeroBytes<arch::CpuFeature::AVX512BW, 4>(__m512i vector) noexcept {
	ZeroComparison comparison;

	comparison.mask				= _mm512_cmpeq_epi32_mask(vector, _mm512_setzero_si512());
	comparison.trailingZeros	= comparison.mask == 0 ? 0 : CountTrailingZeroBits(static_cast<uint16>(comparison.mask));

	return comparison;
}

__BASE_STRING_NAMESPACE_END
