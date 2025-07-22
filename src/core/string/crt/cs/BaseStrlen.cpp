#include <src/core/string/crt/cs/BaseStrlen.h>
#include <src/core/string/crt/BaseStrlenInternal.h>

#include <src/core/memory/MemoryUtility.h>
#include <base/core/arch/ProcessorFeatures.h>

__BASE_STRING_NAMESPACE_BEGIN

DECLARE_NOALIAS std::size_t __CDECL __base_strlenScalar(const char* string) noexcept {
	const char* eos = string;
	while (*eos++);

	return(eos - string - 1);
}

DECLARE_NOALIAS std::size_t __CDECL __base_strlenSse2(const char* string) noexcept {
	const void* current = string;

	while (true) {
		const auto comparisonResult = __checkForZeroBytes<CpuFeature::SSE, 1>(_mm_loadu_epi8(current));

		if (comparisonResult.mask != 0)
			return reinterpret_cast<const char*>(current) - string + comparisonResult.trailingZeros;

		memory::AdvanceBytes(current, 16);
	}

	AssertUnreachable();
	return -1;
}

DECLARE_NOALIAS std::size_t __CDECL __base_strlenAvx(const char* string) noexcept {
	const void* current = string;

	while (true) {
		const auto comparisonResult = __checkForZeroBytes<CpuFeature::AVX, 1>(_mm256_loadu_epi8(current));

		if (comparisonResult.mask != 0)
			return reinterpret_cast<const char*>(current) - string + comparisonResult.trailingZeros;

		memory::AdvanceBytes(current, 32);
	}

	AssertUnreachable();
	return -1;
}

DECLARE_NOALIAS std::size_t __CDECL __base_strlenAvx512(const char* string) noexcept {
	const void* current = string;

	while (true) {
		const auto comparisonResult = __checkForZeroBytes<CpuFeature::AVX512, 1>(_mm512_loadu_epi8(current));

		if (comparisonResult.mask != 0)
			return reinterpret_cast<const char*>(current) - string + comparisonResult.trailingZeros;

		memory::AdvanceBytes(current, 64);
	}

	AssertUnreachable();
	return -1;
}

DECLARE_NOALIAS std::size_t	__CDECL __base_strlen(const char* string) noexcept {
	if (ProcessorFeatures::AVX512F())
		return __base_strlenAvx512(string);
	else if (ProcessorFeatures::AVX())
		return __base_strlenAvx(string);
	else if (ProcessorFeatures::SSE2())
		return __base_strlenSse2(string);

	return __base_strlenScalar(string);
}

__BASE_STRING_NAMESPACE_END
