#include <src/core/string/crt/wcs/BaseWcslen.h>
#include <src/core/string/crt/BaseStrlenCheckForZeroBytes.h>

#include <src/core/memory/MemoryUtility.h>
#include <base/core/arch/ProcessorFeatures.h>

#include <base/core/utility/BitOps.h>

__BASE_STRING_NAMESPACE_BEGIN

DECLARE_NOALIAS std::size_t __CDECL __base_wcslenScalar(const wchar_t* string) noexcept {
	const wchar_t* eos = string;
	while (*eos++);

	return(eos - string - 1);
}

DECLARE_NOALIAS std::size_t __CDECL __base_wcslenSse2(const wchar_t* string) noexcept {
	const void* current = string;

	while (true) {
		const uint8 mask = _mm_movemask_epi8(
			_mm_cmpeq_epi16(
				_mm_loadu_si128(reinterpret_cast<const __m128i*>(current)), _mm_setzero_si128()));

		if (mask != 0)
			return (static_cast<std::size_t>(
				reinterpret_cast<const wchar_t*>(current) - string + CountTrailingZeroBits(mask)));

		memory::AdvanceBytes(current, 16);
	}

	AssertUnreachable();
	return -1;
}

DECLARE_NOALIAS std::size_t __CDECL __base_wcslenAvx(const wchar_t* string) noexcept {
	const void* current = string;

	while (true) {
		const auto mask = _mm256_movemask_epi8(_mm256_cmpeq_epi16(_mm256_loadu_epi16(current), _mm256_setzero_si256()));

		if (mask != 0)
			return (static_cast<std::size_t>(
				reinterpret_cast<const wchar_t*>(current) - string + CountTrailingZeroBits(static_cast<uint16>(mask))));

		memory::AdvanceBytes(current, 32);
	}

	AssertUnreachable();
	return -1;
}

DECLARE_NOALIAS std::size_t __CDECL __base_wcslenAvx512(const wchar_t* string) noexcept {
	const void* current = string;

	while (true) {
		const auto mask = _mm512_cmpeq_epi16_mask(_mm512_loadu_epi16(current), _mm512_setzero_si512());

		if (mask != 0)
			return (static_cast<std::size_t>(
				reinterpret_cast<const wchar_t*>(current) - string + CountTrailingZeroBits(static_cast<uint32>(mask))));

		memory::AdvanceBytes(current, 64);
	}

	AssertUnreachable();
	return -1;
}

DECLARE_NOALIAS std::size_t __CDECL __base_wcslen(const wchar_t* string) noexcept {
	if (arch::ProcessorFeatures::AVX512F())
		return __base_wcslenAvx512(string);
	else if (arch::ProcessorFeatures::AVX())
		return __base_wcslenAvx(string);
	else if (arch::ProcessorFeatures::SSE2())
		return __base_wcslenSse2(string);

	return __base_wcslenScalar(string);
}

__BASE_STRING_NAMESPACE_END
