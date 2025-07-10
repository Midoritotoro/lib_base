#include <src/core/string/crt/BaseStrlen.h>
#include <base/core/arch/ProcessorFeatures.h>

#include <src/core/memory/MemoryUtility.h>
#include <base/core/utility/BitOps.h>

#include <src/core/utility/simd/SimdConstexprHelpers.h>

__BASE_STRING_NAMESPACE_BEGIN

DECLARE_NOALIAS std::size_t __CDECL __base_strlenScalar(const char* string) noexcept {
	const char* eos = string;
	while (*eos++);

	return(eos - string - 1);
}

DECLARE_NOALIAS std::size_t __CDECL __base_strlenSse2(const char* string) noexcept {
	static constexpr const auto constexprComparand = base_constexpr_mm128_setzero();
	const auto comparand = base_vec128i_t_pointer_as_m128i(&constexprComparand);

	const void* current = string;

	while (true) {
		const auto loadedString = _mm_loadu_epi8(current);
		const uint16 bingo = _mm_movemask_epi8(_mm_cmpeq_epi8(loadedString, comparand));

		if (bingo != 0)
			return (static_cast<std::size_t>(
				reinterpret_cast<const char*>(current) - 
				string + CountTrailingZeroBits(bingo)));

		memory::AdvanceBytes(current, 16);
	}

	AssertUnreachable();
	return -1;
}

DECLARE_NOALIAS std::size_t __CDECL __base_strlenAvx(const char* string) noexcept {
	static constexpr const auto constexprComparand = base_constexpr_mm256_setzero();
	const auto comparand = base_vec256i_t_pointer_as_m256i(&constexprComparand);

	const void* current = string;

	while (true) {
		const auto loadedString = _mm256_loadu_epi8(current);
		const uint bingo = _mm256_cmpeq_epi8_mask(loadedString, comparand);

		if (bingo != 0)
			return (static_cast<std::size_t>(
				reinterpret_cast<const char*>(current) - 
				string + CountTrailingZeroBits(bingo)));

		memory::AdvanceBytes(current, 32);
	}

	AssertUnreachable();
	return -1;
}

DECLARE_NOALIAS std::size_t __CDECL __base_strlenAvx512(const char* string) noexcept {
	static constexpr const auto constexprComparand = base_constexpr_mm512_setzero();
	const auto comparand = base_vec512i_t_pointer_as_m512i(&constexprComparand);

	const void* current = string;

	while (true) {
		const auto loadedString = _mm512_loadu_epi8(current);
		const uint64 bingo = _mm512_cmpeq_epi8_mask(loadedString, comparand);

		if (bingo != 0)
			return (static_cast<std::size_t>(
				reinterpret_cast<const char*>(current) - 
				string + CountTrailingZeroBits(bingo)));

		memory::AdvanceBytes(current, 64);
	}

	AssertUnreachable();
	return -1;
}

// ===================================================================================

DECLARE_NOALIAS std::size_t __CDECL __base_wcslenScalar(const wchar_t* string) noexcept {
	const wchar_t* eos = string;
	while (*eos++);

	return(eos - string - 1);
}

DECLARE_NOALIAS std::size_t __CDECL __base_wcslenSse2(const wchar_t* string) noexcept {
	static constexpr const auto constexprComparand = base_constexpr_mm128_setzero();
	const auto comparand = base_vec128i_t_pointer_as_m128i(&constexprComparand);

	const void* current = string;

	while (true) {
		// *
		const auto loadedString = _mm_loadu_epi16(current);
		const uint8 bingo = _mm_movemask_epi8(_mm_cmpeq_epi16(loadedString, comparand));

		if (bingo != 0)
			return (static_cast<std::size_t>(
				reinterpret_cast<const wchar_t*>(current) 
				- string + CountTrailingZeroBits(bingo)));
		// *
		memory::AdvanceBytes(current, 16);
	}

	AssertUnreachable();
	return -1;
}

DECLARE_NOALIAS std::size_t __CDECL __base_wcslenAvx(const wchar_t* string) noexcept {
	static constexpr const auto constexprComparand = base_constexpr_mm256_setzero();
	const auto comparand = base_vec256i_t_pointer_as_m256i(&constexprComparand);

	const void* current = string;

	while (true) {
		const auto loadedString = _mm256_loadu_epi16(current);
		const ushort bingo = _mm256_cmpeq_epi16_mask(loadedString, comparand);

		if (bingo != 0)
			return (static_cast<std::size_t>(
				reinterpret_cast<const wchar_t*>(current) 
				- string + CountTrailingZeroBits(bingo)));

		memory::AdvanceBytes(current, 32);
	}

	AssertUnreachable();
	return -1;
}

DECLARE_NOALIAS std::size_t __CDECL __base_wcslenAvx512(const wchar_t* string) noexcept {
	static constexpr const auto constexprComparand = base_constexpr_mm512_setzero();
	const auto comparand = base_vec512i_t_pointer_as_m512i(&constexprComparand);

	const void* current = string;

	while (true) {
		const auto str = _mm512_loadu_epi16(current);
		const uint64 bingo = _mm512_cmpeq_epi16_mask(str, comparand);

		if (bingo != 0)
			return (static_cast<std::size_t>(
				reinterpret_cast<const wchar_t*>(current) 
				- string + CountTrailingZeroBits(bingo)));

		memory::AdvanceBytes(current, 64);
	}

	AssertUnreachable();
	return -1;
}

// ===================================================================================

DECLARE_NOALIAS std::size_t __CDECL __base_c32lenAvx512(const char32_t* string) noexcept {
	static constexpr const auto constexprComparand = base_constexpr_mm512_setzero();
	const auto comparand = base_vec512i_t_pointer_as_m512i(&constexprComparand);

	const void* current = string;

	while (true) {
		const auto str = _mm512_loadu_epi32(current);
		const uint32 bingo = _mm512_cmpeq_epi32_mask(str, comparand);

		if (bingo != 0)
			return (static_cast<std::size_t>(
				reinterpret_cast<const char32_t*>(current)
				- string + CountTrailingZeroBits(bingo)));

		memory::AdvanceBytes(current, 64);
	}

	AssertUnreachable();
	return -1;
}

DECLARE_NOALIAS std::size_t __CDECL __base_c32lenAvx(const char32_t* string) noexcept {
	static constexpr const auto constexprComparand = base_constexpr_mm256_setzero();
	const auto comparand = base_vec256i_t_pointer_as_m256i(&constexprComparand);

	const void* current = string;

	while (true) {
		const auto str = _mm256_loadu_epi32(current);
		const uint8 bingo = _mm256_cmpeq_epi32_mask(str, comparand);

		if (bingo != 0)
			return (static_cast<std::size_t>(
				reinterpret_cast<const char32_t*>(current)
				- string + CountTrailingZeroBits(bingo)));

		memory::AdvanceBytes(current, 32);
	}

	AssertUnreachable();
	return -1;
}

DECLARE_NOALIAS std::size_t __CDECL __base_c32lenSse2(const char32_t* string) noexcept {
	static constexpr const auto constexprComparand = base_constexpr_mm128_setzero();
	const auto comparand = base_vec128i_t_pointer_as_m128i(&constexprComparand);

	const void* current = string;

	while (true) {
		const auto str = _mm_loadu_epi32(current);
		const uint8 bingo = _mm_movemask_epi8(_mm_cmpeq_epi32(str, comparand));

		if (bingo != 0)
			return (static_cast<std::size_t>(
				reinterpret_cast<const char32_t*>(current)
				- string + CountTrailingZeroBits(bingo)));

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

DECLARE_NOALIAS std::size_t	__CDECL __base_strlen(const char* string) noexcept {
	if (ProcessorFeatures::AVX512F())
		return __base_strlenAvx512(string);
	else if (ProcessorFeatures::AVX())
		return __base_strlenAvx(string);
	else if (ProcessorFeatures::SSE2())
		return __base_strlenSse2(string);

	return __base_strlenScalar(string);
}

DECLARE_NOALIAS std::size_t __CDECL __base_wcslen(const wchar_t* string) noexcept  {
	if constexpr (sizeof(wchar_t) == 8)
		return __base_c32len(reinterpret_cast<const char32_t*>(string));

	if (ProcessorFeatures::AVX512F())
		return __base_wcslenAvx512(string);
	else if (ProcessorFeatures::AVX())
		return __base_wcslenAvx(string);
	else if (ProcessorFeatures::SSE2())
		return __base_wcslenSse2(string);

	return __base_wcslenScalar(string);
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
