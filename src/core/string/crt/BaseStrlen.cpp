#include <src/core/string/crt/BaseStrlen.h>
#include <base/core/arch/ProcessorFeatures.h>

#include <src/core/memory/MemoryUtility.h>
#include <base/core/utility/BitOps.h>

__BASE_STRING_NAMESPACE_BEGIN

DECLARE_NOALIAS std::size_t CDECL __base_strlenScalar(const char* string) noexcept {
	const char* eos = string;
	while (*eos++);

	return(eos - string - 1);
}

DECLARE_NOALIAS std::size_t CDECL __base_strlenSse2(const char* string) noexcept {
	const auto comparand = _mm_set1_epi8('\0');
	const void* current = string;

	while (true) {
		const auto str = _mm_loadu_epi8(string);
		const uint bingo = _mm_cmpeq_epi8_mask(str, comparand);

		if (bingo != 0) {
			const auto result = CountTrailingZeroBits(bingo);
			return (static_cast<std::size_t>(
				reinterpret_cast<const char*>(current) - string + result));
		}

		memory::AdvanceBytes(current, 16);
	}

	AssertUnreachable();
	return -1;
}

DECLARE_NOALIAS std::size_t CDECL __base_strlenAvx(const char* string) noexcept {
	const auto comparand = _mm256_set1_epi8('\0');
	const void* current = string;

	while (true) {
		const auto str = _mm256_loadu_epi8(string);
		const uint bingo = _mm256_cmpeq_epi8_mask(str, comparand);

		if (bingo != 0) {
			const auto result = CountTrailingZeroBits(bingo);
			return (static_cast<std::size_t>(
				reinterpret_cast<const char*>(current) - string + result));
		}

		memory::AdvanceBytes(current, 32);
	}

	AssertUnreachable();
	return -1;
}

DECLARE_NOALIAS std::size_t CDECL __base_strlenAvx512(const char* string) noexcept {
	const auto comparand = _mm512_set1_epi8('\0');
	const void* current = string;

	while (true) {
		const auto str = _mm512_loadu_epi8(string);
		const uint64 bingo = _mm512_cmpeq_epi8_mask(str, comparand);

		if (bingo != 0) {
			const auto result = CountTrailingZeroBits(bingo);
			return (static_cast<std::size_t>(
				reinterpret_cast<const char*>(current) - string + result));
		}

		memory::AdvanceBytes(current, 64);
	}

	AssertUnreachable();
	return -1;
}

// ===================================================================================

DECLARE_NOALIAS std::size_t CDECL __base_wcslenScalar(const wchar_t* string) noexcept {
	const wchar_t* eos = string;
	while (*eos++);

	return(eos - string - 1);
}

DECLARE_NOALIAS std::size_t CDECL __base_wcslenSse2(const wchar_t* string) noexcept {
	const auto comparand = _mm_set1_epi16('\0');
	const void* current = string;

	while (true) {
		const auto str = _mm_loadu_epi16(string);
		const ushort bingo = _mm_cmpeq_epi16_mask(str, comparand);

		if (bingo != 0) {
			const auto result = CountTrailingZeroBits(bingo);
			return (static_cast<std::size_t>(
				reinterpret_cast<const wchar_t*>(current) - string + result));
		}

		memory::AdvanceBytes(current, 16);
	}

	AssertUnreachable();
	return -1;
}

DECLARE_NOALIAS std::size_t CDECL __base_wcslenAvx(const wchar_t* string) noexcept {
	const auto comparand = _mm256_set1_epi16('\0');
	const void* current = string;

	while (true) {
		const auto str = _mm256_loadu_epi16(string);
		const ushort bingo = _mm256_cmpeq_epi16_mask(str, comparand);

		if (bingo != 0) {
			const auto result = CountTrailingZeroBits(bingo);
			return (static_cast<std::size_t>(
				reinterpret_cast<const wchar_t*>(current) - string + result));
		}

		memory::AdvanceBytes(current, 32);
	}

	AssertUnreachable();
	return -1;
}

DECLARE_NOALIAS std::size_t CDECL __base_wcslenAvx512(const wchar_t* string) noexcept {
	const auto comparand = _mm512_set1_epi16('\0');
	const void* current = string;

	while (true) {
		const auto str = _mm512_loadu_epi16(string);
		const uint64 bingo = _mm512_cmpeq_epi16_mask(str, comparand);

		if (bingo != 0) {
			const auto result = CountTrailingZeroBits(bingo);
			return (static_cast<std::size_t>(
				reinterpret_cast<const wchar_t*>(current) - string + result));
		}

		memory::AdvanceBytes(current, 64);
	}

	AssertUnreachable();
	return -1;
}

// ===================================================================================

DECLARE_NOALIAS std::size_t	CDECL __base_strlen(const char* string) noexcept {
	if (ProcessorFeatures::AVX512F())
		return __base_strlenAvx512(string);
	else if (ProcessorFeatures::AVX())
		return __base_strlenAvx(string);
	else if (ProcessorFeatures::SSE2())
		return __base_strlenSse2(string);

	return __base_strlenScalar(string);
}

DECLARE_NOALIAS std::size_t CDECL __base_wcslen(const wchar_t* string) noexcept  {
	if (ProcessorFeatures::AVX512F())
		return __base_wcslenAvx512(string);
	else if (ProcessorFeatures::AVX())
		return __base_wcslenAvx(string);
	else if (ProcessorFeatures::SSE2())
		return __base_wcslenSse2(string);

	return __base_wcslenScalar(string);
}

__BASE_STRING_NAMESPACE_END
