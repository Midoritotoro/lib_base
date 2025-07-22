#include <src/core/string/crt/cs/BaseStrlen.h>
#include <src/core/string/crt/BaseStrlenInternal.h>

#include <src/core/memory/MemoryUtility.h>
#include <base/core/arch/ProcessorFeatures.h>


__BASE_STRING_NAMESPACE_BEGIN

DECLARE_NOALIAS std::size_t __CDECL __base_c32lenAvx512(const char32_t* string) noexcept {
	const void* current = string;

	while (true) {
		const auto comparisonResult = __checkForZeroBytes<CpuFeature::AVX512, 4>(_mm512_loadu_epi32(current));

		if (comparisonResult.mask != 0)
			return (static_cast<std::size_t>(
				reinterpret_cast<const char32_t*>(current) - string + comparisonResult.trailingZeros));

		memory::AdvanceBytes(current, 64);
	}

	AssertUnreachable();
	return -1;
}

DECLARE_NOALIAS std::size_t __CDECL __base_c32lenAvx(const char32_t* string) noexcept {
	const void* current = string;

	while (true) {
		const auto comparisonResult = __checkForZeroBytes<CpuFeature::AVX, 4>(_mm256_loadu_epi32(current));

		if (comparisonResult.mask != 0)
			return (static_cast<std::size_t>(
				reinterpret_cast<const char32_t*>(current) - string + comparisonResult.trailingZeros));

		memory::AdvanceBytes(current, 32);
	}

	AssertUnreachable();
	return -1;
}

DECLARE_NOALIAS std::size_t __CDECL __base_c32lenSse2(const char32_t* string) noexcept {
	const void* current = string;

	while (true) {
		const auto comparisonResult = __checkForZeroBytes<CpuFeature::SSE, 4>(_mm_loadu_epi32(current));

		if (comparisonResult.mask != 0)
			return (static_cast<std::size_t>(
				reinterpret_cast<const char32_t*>(current) - string + comparisonResult.trailingZeros));

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
	if (ProcessorFeatures::AVX512F())
		return __base_c32lenAvx512(string);
	else if (ProcessorFeatures::AVX())
		return __base_c32lenAvx(string);
	else if (ProcessorFeatures::SSE2())
		return __base_c32lenSse2(string);

	return __base_c32lenScalar(string);
}

__BASE_STRING_NAMESPACE_END
