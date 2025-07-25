#include <src/core/string/crt/cs/internal/BaseStrlenInternal.h>
#include <src/core/string/crt/BaseStrlenCheckForZeroBytes.h>

#include <src/core/memory/MemoryUtility.h>

__BASE_STRING_NAMESPACE_BEGIN

template <arch::CpuFeature feature>
DECLARE_NOALIAS sizetype __CDECL BaseFeatureAwareStrlen(const char* string) noexcept
{
	AssertUnreachable();
	return -1;
}

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
		const auto comparisonResult = __checkForZeroBytes<arch::CpuFeature::SSE2, 1>(_mm_loadu_si128(reinterpret_cast<const __m128i*>(current)));

		if (comparisonResult.mask != 0)
			return reinterpret_cast<const char*>(current) - string + comparisonResult.trailingZeros;

		memory::AdvanceBytes(current, 16);
	}

	AssertUnreachable();
	return -1;
}

template <>
DECLARE_NOALIAS sizetype __CDECL BaseFeatureAwareStrlen<arch::CpuFeature::AVX2>(const char* string) noexcept {
	const void* current = string;

	while (true) {
		const auto comparisonResult = __checkForZeroBytes<arch::CpuFeature::AVX2, 1>(_mm256_lddqu_si256(reinterpret_cast<const __m256i*>(current)));

		if (comparisonResult.mask != 0)
			return reinterpret_cast<const char*>(current) - string + comparisonResult.trailingZeros;

		memory::AdvanceBytes(current, 32);
	}

	AssertUnreachable();
	return -1;
}

template <>
DECLARE_NOALIAS sizetype __CDECL BaseFeatureAwareStrlen<arch::CpuFeature::AVX512BW>(const char* string) noexcept {
	const void* current = string;

	while (true) {
		const auto comparisonResult = __checkForZeroBytes<arch::CpuFeature::AVX512BW, 1>(_mm512_loadu_epi8(current));

		if (comparisonResult.mask != 0)
			return reinterpret_cast<const char*>(current) - string + comparisonResult.trailingZeros;

		memory::AdvanceBytes(current, 64);
	}

	AssertUnreachable();
	return -1;
}

__BASE_STRING_NAMESPACE_END
