#include <src/core/string/crt/wcs/BaseWcslen.h>
#include <src/core/string/crt/BaseStrlenInternal.h>

#include <src/core/memory/MemoryUtility.h>
#include <base/core/arch/ProcessorFeatures.h>


__BASE_STRING_NAMESPACE_BEGIN

DECLARE_NOALIAS std::size_t __CDECL __base_wcslenScalar(const wchar_t* string) noexcept {
	const wchar_t* eos = string;
	while (*eos++);

	return(eos - string - 1);
}

DECLARE_NOALIAS std::size_t __CDECL __base_wcslenSse2(const wchar_t* string) noexcept {
	const void* current = string;

	while (true) {
		const auto comparisonResult = __checkForZeroBytes<CpuFeature::SSE, 2>(_mm_loadu_epi16(current));

		if (comparisonResult.mask != 0)
			return (static_cast<std::size_t>(
				reinterpret_cast<const wchar_t*>(current) - string + comparisonResult.trailingZeros));

		memory::AdvanceBytes(current, 16);
	}

	AssertUnreachable();
	return -1;
}

DECLARE_NOALIAS std::size_t __CDECL __base_wcslenAvx(const wchar_t* string) noexcept {
	const void* current = string;

	while (true) {
		const auto comparisonResult = __checkForZeroBytes<CpuFeature::AVX, 2>(_mm256_loadu_epi16(current));

		if (comparisonResult.mask != 0)
			return (static_cast<std::size_t>(
				reinterpret_cast<const wchar_t*>(current) - string + comparisonResult.trailingZeros));

		memory::AdvanceBytes(current, 32);
	}

	AssertUnreachable();
	return -1;
}

DECLARE_NOALIAS std::size_t __CDECL __base_wcslenAvx512(const wchar_t* string) noexcept {
	const void* current = string;

	while (true) {
		const auto comparisonResult = __checkForZeroBytes<CpuFeature::AVX512, 2>(_mm512_loadu_epi16(current));

		if (comparisonResult.mask != 0)
			return (static_cast<std::size_t>(
				reinterpret_cast<const wchar_t*>(current) - string + comparisonResult.trailingZeros));

		memory::AdvanceBytes(current, 64);
	}

	AssertUnreachable();
	return -1;
}

DECLARE_NOALIAS std::size_t __CDECL __base_wcslen(const wchar_t* string) noexcept {
	if (ProcessorFeatures::AVX512F())
		return __base_wcslenAvx512(string);
	else if (ProcessorFeatures::AVX())
		return __base_wcslenAvx(string);
	else if (ProcessorFeatures::SSE2())
		return __base_wcslenSse2(string);

	return __base_wcslenScalar(string);
}

__BASE_STRING_NAMESPACE_END
