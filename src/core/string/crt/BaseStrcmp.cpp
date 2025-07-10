#include <src/core/string/crt/BaseStrcmp.h>
#include <base/core/arch/ProcessorFeatures.h>

#include <src/core/memory/MemoryUtility.h>
#include <base/core/utility/BitOps.h>

#include <src/core/utility/simd/SimdConstexprHelpers.h>

always_inline NODISCARD bool IsXmmZero(__m128i xmmRegister) noexcept {
	constexpr const auto cmp = base_constexpr_mm_setzero();
	return _mm_movemask_epi8(
		_mm_cmpeq_epi64(xmmRegister, 
			base_vec128i_t_pointer_as_m128i(&cmp))) == 0xFFFFFFFFFFFFFFFF;
}

always_inline NODISCARD bool IsYmmZero(__m256i ymmRegister) noexcept {
	constexpr const auto cmp = base_constexpr_mm256_setzero();
	return _mm256_cmpeq_epi64_mask(ymmRegister, base_vec256i_t_pointer_as_m256i(&cmp))== 0xFFFFFFFFFFFFFFFF;
}

always_inline NODISCARD bool IsZmmZero(__m512i zmmRegister) noexcept {
	constexpr const auto cmp = base_constexpr_mm512_setzero();
	return _mm512_cmpeq_epi64_mask(zmmRegister, base_vec512i_t_pointer_as_m512i(&cmp)) == 0xFFFFFFFFFFFFFFFF;
}

__BASE_STRING_NAMESPACE_BEGIN

DECLARE_NOALIAS int __CDECL __base_strcmpScalar(
	const char* firstString,
	const char* secondString) noexcept 
{
	int ret = 0;

	while ((ret = *(uchar*)firstString - *(uchar*)secondString) == 0 && *secondString)
		++firstString, ++secondString;
	
	// (if positive) - (if negative) generates branchless code
	return ((-ret) < 0) - (ret < 0);
}

DECLARE_NOALIAS int __CDECL __base_strcmpSse2(
	const char* firstString, 
	const char* secondString) noexcept
{
	int ret = 0;

	while (true)
	{
		const auto loadedFirst	= _mm_loadu_epi8(firstString);
		const auto loadedSecond = _mm_loadu_epi8(secondString);

		ret = ((_mm_movemask_epi8(_mm_sub_epi8(loadedFirst, loadedSecond))) == 0);
		const uint16 bingo = _mm_cmpeq_epi8_mask(loadedFirst, comparand);

		if (bingo != 0)
			return (static_cast<std::size_t>(
				reinterpret_cast<const char*>(current) -
				string + CountTrailingZeroBits(bingo)));

		memory::AdvanceBytes(firstString, 16);
		memory::AdvanceBytes(secondString, 16);
	}

	return ((-ret) < 0) - (ret < 0);
}

DECLARE_NOALIAS int __CDECL __base_strcmpAvx(
	const char* firstString,
	const char* secondString) noexcept 
{
	int ret = 0;
	return ((-ret) < 0) - (ret < 0);
}

DECLARE_NOALIAS int __CDECL __base_strcmpAvx512(
	const char* firstString,
	const char* secondString) noexcept 
{
	int ret = 0;
	return ((-ret) < 0) - (ret < 0);
}

// ===================================================================================

DECLARE_NOALIAS int __CDECL __base_wcscmpScalar(
	const wchar_t* firstString, 
	const wchar_t* secondString) noexcept
{
	int ret = 0;

	while ((ret = *(wchar_t*)firstString - *(wchar_t*)secondString) == 0 && *secondString)
		++firstString, ++secondString;

	// (if positive) - (if negative) generates branchless code
	return ((-ret) < 0) - (ret < 0);
}

DECLARE_NOALIAS int __CDECL __base_wcscmpSse2(
	const wchar_t* firstString, 
	const wchar_t* secondString) noexcept 
{
	int ret = 0;
	return ((-ret) < 0) - (ret < 0);
}

DECLARE_NOALIAS int __CDECL __base_wcscmpAvx(
	const wchar_t* firstString,
	const wchar_t* secondString) noexcept
{
	int ret = 0;
	return ((-ret) < 0) - (ret < 0);
}

DECLARE_NOALIAS int __CDECL __base_wcscmpAvx512(
	const wchar_t* firstString, 
	const wchar_t* secondString) noexcept
{
	int ret = 0;
	return ((-ret) < 0) - (ret < 0);
}

// ===================================================================================

DECLARE_NOALIAS int __CDECL __base_c32cmpAvx512(
	const char32_t* firstString,
	const char32_t* secondString) noexcept 
{
	int ret = 0;
	return ((-ret) < 0) - (ret < 0);
}

DECLARE_NOALIAS int __CDECL __base_c32cmpAvx(
	const char32_t* firstString, 
	const char32_t* secondString) noexcept 
{
	int ret = 0;
	return ((-ret) < 0) - (ret < 0);
}

DECLARE_NOALIAS int __CDECL __base_c32cmpSse2(
	const char32_t* firstString, 
	const char32_t* secondString) noexcept
{
	int ret = 0;
	return ((-ret) < 0) - (ret < 0);
}

DECLARE_NOALIAS int __CDECL __base_c32cmpScalar(
	const char32_t* firstString, 
	const char32_t* secondString) noexcept 
{
	int ret = 0;

	while ((ret = *(char32_t*)firstString - *(char32_t*)secondString) == 0 && *secondString)
		++firstString, ++secondString;

	// (if positive) - (if negative) generates branchless code
	return ((-ret) < 0) - (ret < 0);
}

DECLARE_NOALIAS int	__CDECL __base_strcmp(
	const char* firstString,
	const char* secondString) noexcept 
{
	if (ProcessorFeatures::AVX512F())
		return __base_strcmpAvx512(firstString, secondString);
	else if (ProcessorFeatures::AVX())
		return __base_strcmpAvx(firstString, secondString);
	else if (ProcessorFeatures::SSE2())
		return __base_strcmpSse2(firstString, secondString);

	return __base_strcmpScalar(firstString, secondString);
}

DECLARE_NOALIAS int __CDECL __base_wcscmp(
	const wchar_t* firstString,
	const wchar_t* secondString) noexcept
{
	if constexpr (sizeof(wchar_t) == 4)
		return __base_c32cmp(
			reinterpret_cast<const char32_t*>(firstString), 
			reinterpret_cast<const char32_t*>(secondString));

	if (ProcessorFeatures::AVX512F())
		return __base_wcscmpAvx512(firstString, secondString);
	else if (ProcessorFeatures::AVX())
		return __base_wcscmpAvx(firstString, secondString);
	else if (ProcessorFeatures::SSE2())
		return __base_wcscmpSse2(firstString, secondString);

	return __base_wcscmpScalar(firstString, secondString);
}

DECLARE_NOALIAS int __CDECL __base_c32cmp(
	const char32_t* firstString, 
	const char32_t* secondString) noexcept 
{
	if (ProcessorFeatures::AVX512F())
		return __base_c32cmpAvx512(firstString, secondString);
	else if (ProcessorFeatures::AVX())
		return __base_c32cmpAvx(firstString, secondString);
	else if (ProcessorFeatures::SSE2())
		return __base_c32cmpSse2(firstString, secondString);

	return __base_c32cmpScalar(firstString, secondString);
}

__BASE_STRING_NAMESPACE_END
