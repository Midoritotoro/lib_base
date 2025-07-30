#include <src/core/string/crt/cs/BaseStrlen.h>

#include <src/core/memory/MemoryUtility.h>
#include <base/core/arch/ProcessorFeatures.h>

#include <base/core/utility/BitOps.h>

__BASE_STRING_NAMESPACE_BEGIN

DECLARE_NOALIAS std::size_t __CDECL __base_c32lenAvx512(const char32_t* string) noexcept {
	const void* current = string;

	while (true) {
		const uint16 mask = _mm512_cmpeq_epi32_mask(_mm512_loadu_epi32(current), _mm512_setzero_si512());

		if (mask != 0)
			return reinterpret_cast<const char32_t*>(current) - string + CountTrailingZeroBits(mask);

		memory::AdvanceBytes(current, 64);
	}

	AssertUnreachable();
	return -1;
}

DECLARE_NOALIAS std::size_t __CDECL __base_c32lenAvx(const char32_t* string) noexcept {
	const void* current = string;

	while (true) {
		const uint8 mask = _mm256_movemask_epi8(
			_mm256_cmpeq_epi32(
				_mm256_lddqu_si256(reinterpret_cast<const __m256i*>(current)), _mm256_setzero_si256()));

		if (mask != 0)
			return reinterpret_cast<const char32_t*>(current) - string + CountTrailingZeroBits(mask);

		memory::AdvanceBytes(current, 32);
	}

	AssertUnreachable();
	return -1;
}

DECLARE_NOALIAS std::size_t __CDECL __base_c32lenSse2(const char32_t* string) noexcept {
	const void* current = string;

	while (true) {
		const uint8 mask = _mm_movemask_epi8(
			_mm_cmpeq_epi32(
				_mm_loadu_si128(reinterpret_cast<const __m128i*>(current)), _mm_setzero_si128()));

		if (mask != 0)
			return reinterpret_cast<const char32_t*>(current) - string + CountTrailingZeroBits(mask);

		memory::AdvanceBytes(current, 16);
	}

	AssertUnreachable();
	return -1;
}

DECLARE_NOALIAS std::size_t __CDECL __base_c32lenScalar(const char32_t* string) noexcept {
	const char32_t* eos = string;
	while (*eos++);

	return(eos - string - 1);
}

DECLARE_NOALIAS std::size_t __CDECL __base_c32len(const char32_t* string) noexcept {
	if (arch::ProcessorFeatures::AVX512F())
		return __base_c32lenAvx512(string);
	else if (arch::ProcessorFeatures::AVX())
		return __base_c32lenAvx(string);
	else if (arch::ProcessorFeatures::SSE2())
		return __base_c32lenSse2(string);

	return __base_c32lenScalar(string);
}

__BASE_STRING_NAMESPACE_END
