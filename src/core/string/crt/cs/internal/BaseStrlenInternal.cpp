#include <src/core/string/crt/cs/internal/BaseStrlenInternal.h>
#include <src/core/string/crt/BaseStrlenCheckForZeroBytes.h>

#include <src/core/memory/MemoryUtility.h>
#include <base/core/memory/FixedMemcmp.h>

#include <base/core/utility/BitOps.h>
#include <src/core/utility/simd/SimdHelpers.h>

__BASE_STRING_NAMESPACE_BEGIN

template <>
DECLARE_NOALIAS sizetype __CDECL BaseFeatureAwareStrlen<arch::CpuFeature::None>(const char* string) noexcept {
	const char* eos = string;
	while (*eos++);

	return(eos - string - 1);
}

template <>
DECLARE_NOALIAS sizetype __CDECL BaseFeatureAwareStrlen<arch::CpuFeature::SSE2>(const char* string) noexcept {
	const void* current = string;

	while (true) {
		const uint16 mask = _mm_movemask_epi8(
			_mm_cmpeq_epi8(
				_mm_loadu_si128(reinterpret_cast<const __m128i*>(current)), _mm_setzero_si128()));

		if (mask != 0)
			return reinterpret_cast<const char*>(current) - string + CountTrailingZeroBits(mask);

		memory::AdvanceBytes(current, 16);
	}

	AssertUnreachable();
	return -1;
}

template <>
DECLARE_NOALIAS sizetype __CDECL BaseFeatureAwareStrlen<arch::CpuFeature::AVX2>(const char* string) noexcept {
	const void* current = string;

	while (true) {
		const uint32 mask = _mm256_movemask_epi8(
			_mm256_cmpeq_epi8(
				_mm256_lddqu_si256(reinterpret_cast<const __m256i*>(current)), _mm256_setzero_si256()));

		if (mask != 0)
			return reinterpret_cast<const char*>(current) - string + CountTrailingZeroBits(mask);

		memory::AdvanceBytes(current, 32);
	}

	AssertUnreachable();
	return -1;
}


template <>
DECLARE_NOALIAS sizetype __CDECL BaseFeatureAwareStrlen<arch::CpuFeature::AVX512BW>(const char* string) noexcept {
	const void* current = string;

	while (true) {
		const uint64 mask = _mm512_cmpeq_epi8_mask(_mm512_loadu_epi8(current), _mm512_setzero_si512());

		if (mask != 0)
			return reinterpret_cast<const char*>(current) - string + CountTrailingZeroBits(mask);

		memory::AdvanceBytes(current, 64);
	}
	
	AssertUnreachable();
	return -1;
}

__BASE_STRING_NAMESPACE_END
