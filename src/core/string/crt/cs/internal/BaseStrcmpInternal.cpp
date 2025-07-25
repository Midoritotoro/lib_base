#include <src/core/string/crt/cs/internal/BaseStrcmpInternal.h>

#include <src/core/string/crt/BaseStrlenCheckForZeroBytes.h>
#include <src/core/memory/MemoryUtility.h>

__BASE_STRING_NAMESPACE_BEGIN

template <arch::CpuFeature feature>
DECLARE_NOALIAS int __CDECL __baseFeatureAwareStrcmp(
	const char* firstString,
	const char* secondString) noexcept
{
	AssertUnreachable();
	return -1;
}

template <>
DECLARE_NOALIAS int __CDECL __baseFeatureAwareStrcmp<arch::CpuFeature::None>(
	const char* firstString,
	const char* secondString) noexcept
{
	int ret = 0;

	while ((ret = *(uchar*)firstString - *(uchar*)secondString) == 0 && *secondString)
		++firstString, ++secondString;

	// (if positive) - (if negative) generates branchless code
	return ((-ret) < 0) - (ret < 0);
}

template <>
DECLARE_NOALIAS int __CDECL __baseFeatureAwareStrcmp<arch::CpuFeature::SSE2>(
	const char* firstString,
	const char* secondString) noexcept
{
	int ret = 0;

	while (true) {
		const auto loadedFirst	= _mm_loadu_si128(reinterpret_cast<const __m128i*>(firstString));
		const auto loadedSecond = _mm_loadu_si128(reinterpret_cast<const __m128i*>(secondString));

		const auto zeroComparison = __checkForZeroBytes<arch::CpuFeature::SSE2, 1>(loadedSecond);

		// End of string not found
		if (zeroComparison.mask == 0) {
			ret = ((_mm_movemask_epi8(_mm_sub_epi8(loadedFirst, loadedSecond))) == 0);

			if (ret == 0)
				break;
		}
		else {
			auto current = size_t(0);
			while ((ret = *(uchar*)firstString - *(uchar*)secondString) == 0 && current < zeroComparison.trailingZeros)
				++firstString, ++secondString, ++current;

			break;
		}

		memory::AdvanceBytes(firstString, 16);
		memory::AdvanceBytes(secondString, 16);
	}

	return ((-ret) < 0) - (ret < 0);
}

template <>
DECLARE_NOALIAS int __CDECL __baseFeatureAwareStrcmp<arch::CpuFeature::AVX2>(
	const char* firstString,
	const char* secondString) noexcept
{
	int ret = 0;

	while (true) {
		const auto loadedFirst	= _mm256_lddqu_si256(reinterpret_cast<const __m256i*>(firstString));
		const auto loadedSecond = _mm256_lddqu_si256(reinterpret_cast<const __m256i*>(secondString));

		const auto zeroComparison = __checkForZeroBytes<arch::CpuFeature::AVX2, 1>(loadedSecond);

		// End of string not found
		if (zeroComparison.mask == 0) {
			ret = ((_mm256_movemask_epi8(_mm256_sub_epi8(loadedFirst, loadedSecond))) == 0);

			if (ret == 0)
				break;
		}
		else {
			auto current = size_t(0);
			while ((ret = *(uchar*)firstString - *(uchar*)secondString) == 0 && current < zeroComparison.trailingZeros)
				++firstString, ++secondString, ++current;

			break;
		}

		memory::AdvanceBytes(firstString, 32);
		memory::AdvanceBytes(secondString, 32);
	}

	return ((-ret) < 0) - (ret < 0);
}

template <>
DECLARE_NOALIAS int __CDECL __baseFeatureAwareStrcmp<arch::CpuFeature::AVX512BW>(
	const char* firstString,
	const char* secondString) noexcept
{
	int ret = 0;

	while (true) {
		const auto loadedFirst	= _mm512_loadu_si512(firstString);
		const auto loadedSecond = _mm512_loadu_si512(secondString);

		const auto zeroComparison = __checkForZeroBytes<arch::CpuFeature::AVX512BW, 1>(loadedSecond);

		// End of string not found
		if (zeroComparison.mask == 0) {
			ret = ((_mm512_movepi8_mask(_mm512_sub_epi8(loadedFirst, loadedSecond))) == 0);

			if (ret == 0)
				break;
		}
		else {
			auto current = size_t(0);
			while ((ret = *(uchar*)firstString - *(uchar*)secondString) == 0 && current < zeroComparison.trailingZeros)
				++firstString, ++secondString, ++current;

			break;
		}

		memory::AdvanceBytes(firstString, 64);
		memory::AdvanceBytes(secondString, 64);
	}

	return ((-ret) < 0) - (ret < 0);
}

__BASE_STRING_NAMESPACE_END
