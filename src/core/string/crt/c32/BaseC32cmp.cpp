#include <src/core/string/crt/wcs/BaseWcslen.h>
#include <src/core/string/crt/BaseStrlenCheckForZeroBytes.h>

#include <src/core/memory/MemoryUtility.h>
#include <base/core/arch/ProcessorFeatures.h>


__BASE_STRING_NAMESPACE_BEGIN

DECLARE_NOALIAS int __CDECL __base_c32cmpAvx512(
	const char32_t* firstString,
	const char32_t* secondString) noexcept
{
	int ret = 0;

	while (true) {
		const auto loadedFirst = _mm512_loadu_epi32(firstString);
		const auto loadedSecond = _mm512_loadu_epi32(secondString);

		const auto zeroComparison = __checkForZeroBytes<arch::CpuFeature::AVX512BW, 4>(loadedSecond);

		// End of string not found
		if (zeroComparison.mask == 0) {
			ret = ((_mm512_movepi32_mask(_mm512_sub_epi32(loadedFirst, loadedSecond))) == 0);

			if (ret == 0)
				break;
		}
		else {
			auto current = size_t(0);
			while ((ret = *firstString - *secondString) == 0 && current < zeroComparison.trailingZeros)
				++firstString, ++secondString, ++current;

			break;
		}

		memory::AdvanceBytes(firstString, 64);
		memory::AdvanceBytes(secondString, 64);
	}

	return ((-ret) < 0) - (ret < 0);
}

DECLARE_NOALIAS int __CDECL __base_c32cmpAvx(
	const char32_t* firstString,
	const char32_t* secondString) noexcept
{
	int ret = 0;

	while (true) {
		const auto loadedFirst = _mm256_loadu_epi32(firstString);
		const auto loadedSecond = _mm256_loadu_epi32(secondString);

		const auto zeroComparison = __checkForZeroBytes<arch::CpuFeature::AVX2, 4>(loadedSecond);

		// End of string not found
		if (zeroComparison.mask == 0) {
			ret = ((_mm256_movemask_epi8(_mm256_sub_epi32(loadedFirst, loadedSecond))) == 0);

			if (ret == 0)
				break;
		}
		else {
			auto current = size_t(0);
			while ((ret = *firstString - *secondString) == 0 && current < zeroComparison.trailingZeros)
				++firstString, ++secondString, ++current;

			break;
		}

		memory::AdvanceBytes(firstString, 32);
		memory::AdvanceBytes(secondString, 32);
	}

	return ((-ret) < 0) - (ret < 0);
}

DECLARE_NOALIAS int __CDECL __base_c32cmpSse2(
	const char32_t* firstString,
	const char32_t* secondString) noexcept
{
	int ret = 0;

	while (true) {
		const auto loadedFirst = _mm_loadu_epi32(firstString);
		const auto loadedSecond = _mm_loadu_epi32(secondString);

		const auto zeroComparison = __checkForZeroBytes<arch::CpuFeature::SSE2, 4>(loadedSecond);

		// End of string not found
		if (zeroComparison.mask == 0) {
			ret = ((_mm_movemask_epi8(_mm_sub_epi32(loadedFirst, loadedSecond))) == 0);

			if (ret == 0)
				break;
		}
		else {
			auto current = size_t(0);
			while ((ret = *firstString - *secondString) == 0 && current < zeroComparison.trailingZeros)
				++firstString, ++secondString, ++current;

			break;
		}

		memory::AdvanceBytes(firstString, 16);
		memory::AdvanceBytes(secondString, 16);
	}

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


DECLARE_NOALIAS int __CDECL __base_c32cmp(
	const char32_t* firstString,
	const char32_t* secondString) noexcept
{
	if (arch::ProcessorFeatures::AVX512F())
		return __base_c32cmpAvx512(firstString, secondString);
	else if (arch::ProcessorFeatures::AVX())
		return __base_c32cmpAvx(firstString, secondString);
	else if (arch::ProcessorFeatures::SSE2())
		return __base_c32cmpSse2(firstString, secondString);

	return __base_c32cmpScalar(firstString, secondString);
}


__BASE_STRING_NAMESPACE_END
