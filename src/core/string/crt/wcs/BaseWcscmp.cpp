#include <src/core/string/crt/wcx/BaseWcslen.h>
#include <src/core/string/crt/BaseStrlenInternal.h>

#include <src/core/memory/MemoryUtility.h>
#include <base/core/arch/ProcessorFeatures.h>


__BASE_STRING_NAMESPACE_BEGIN

DECLARE_NOALIAS int __CDECL __base_wcscmpScalar(
	const wchar_t* firstString,
	const wchar_t* secondString) noexcept
{
	int ret = 0;

	while ((ret = *firstString - *secondString) == 0 && *secondString)
		++firstString, ++secondString;

	// (if positive) - (if negative) generates branchless code
	return ((-ret) < 0) - (ret < 0);
}

DECLARE_NOALIAS int __CDECL __base_wcscmpSse2(
	const wchar_t* firstString,
	const wchar_t* secondString) noexcept
{
	int ret = 0;

	while (true) {
		const auto loadedFirst = _mm_loadu_epi16(firstString);
		const auto loadedSecond = _mm_loadu_epi16(secondString);

		const auto zeroComparison = __checkForZeroBytes<CpuFeature::SSE, 2>(loadedSecond);

		// End of string not found
		if (zeroComparison.mask == 0) {
			ret = ((_mm_movemask_epi8(_mm_sub_epi16(loadedFirst, loadedSecond))) == 0);

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

DECLARE_NOALIAS int __CDECL __base_wcscmpAvx(
	const wchar_t* firstString,
	const wchar_t* secondString) noexcept
{
	int ret = 0;

	while (true) {
		const auto loadedFirst = _mm256_loadu_epi16(firstString);
		const auto loadedSecond = _mm256_loadu_epi16(secondString);

		const auto zeroComparison = __checkForZeroBytes<CpuFeature::AVX, 2>(loadedSecond);

		// End of string not found
		if (zeroComparison.mask == 0) {
			ret = ((_mm256_movemask_epi8(_mm256_sub_epi16(loadedFirst, loadedSecond))) == 0);

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

DECLARE_NOALIAS int __CDECL __base_wcscmpAvx512(
	const wchar_t* firstString,
	const wchar_t* secondString) noexcept
{
	int ret = 0;

	while (true) {
		const auto loadedFirst = _mm512_loadu_epi16(firstString);
		const auto loadedSecond = _mm512_loadu_epi16(secondString);

		const auto zeroComparison = __checkForZeroBytes<CpuFeature::AVX512, 2>(loadedSecond);

		// End of string not found
		if (zeroComparison.mask == 0) {
			ret = ((_mm512_movepi8_mask(_mm512_sub_epi16(loadedFirst, loadedSecond))) == 0);

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

DECLARE_NOALIAS int __CDECL __base_wcscmp(
	const wchar_t* firstString,
	const wchar_t* secondString) noexcept
{
	if (ProcessorFeatures::AVX512F())
		return __base_wcscmpAvx512(firstString, secondString);
	else if (ProcessorFeatures::AVX())
		return __base_wcscmpAvx(firstString, secondString);
	else if (ProcessorFeatures::SSE2())
		return __base_wcscmpSse2(firstString, secondString);

	return __base_wcscmpScalar(firstString, secondString);
}

__BASE_STRING_NAMESPACE_END
