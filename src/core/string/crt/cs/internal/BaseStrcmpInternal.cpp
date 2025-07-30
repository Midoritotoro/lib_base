#include <src/core/string/crt/cs/internal/BaseStrcmpInternal.h>

#include <src/core/memory/MemoryUtility.h>
#include <base/core/utility/BitOps.h>


__BASE_STRING_NAMESPACE_BEGIN

template <>
DECLARE_NOALIAS int __CDECL BaseFeatureAwareStrcmp<arch::CpuFeature::None>(
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
DECLARE_NOALIAS int __CDECL BaseFeatureAwareStrcmp<arch::CpuFeature::SSE2>(
	const char* firstString,
	const char* secondString) noexcept
{
	int ret = 0;

	while (true) {
		const auto loadedFirst	= _mm_loadu_si128(reinterpret_cast<const __m128i*>(firstString));
		const auto loadedSecond = _mm_loadu_si128(reinterpret_cast<const __m128i*>(secondString));

		const uint16 mask = _mm_movemask_epi8(_mm_cmpeq_epi8(loadedSecond, _mm_setzero_si128()));

		// End of string not found
		if (mask == 0) {
			ret = ((_mm_movemask_epi8(_mm_sub_epi8(loadedFirst, loadedSecond))) == 0);

			if (ret == 0)
				break;
		}
		else {
			auto current = uint8(0);
			const auto trailingZeros = CountTrailingZeroBits(mask);

			while ((ret = *(uchar*)firstString - *(uchar*)secondString) == 0 && current < trailingZeros)
				++firstString, ++secondString, ++current;

			break;
		}

		memory::AdvanceBytes(firstString, 16);
		memory::AdvanceBytes(secondString, 16);
	}

	return ((-ret) < 0) - (ret < 0);
}

template <>
DECLARE_NOALIAS int __CDECL BaseFeatureAwareStrcmp<arch::CpuFeature::AVX2>(
	const char* firstString,
	const char* secondString) noexcept
{
	int ret = 0;

	while (true) {
		const auto loadedFirst	= _mm256_lddqu_si256(reinterpret_cast<const __m256i*>(firstString));
		const auto loadedSecond = _mm256_lddqu_si256(reinterpret_cast<const __m256i*>(secondString));

		const uint32 mask = _mm256_movemask_epi8(_mm256_cmpeq_epi8(loadedSecond, _mm256_setzero_si256()));

		// End of string not found
		if (mask == 0) {
			ret = ((_mm256_movemask_epi8(_mm256_sub_epi8(loadedFirst, loadedSecond))) == 0);

			if (ret == 0)
				break;
		}
		else {
			auto current = uint8(0);
			const auto trailingZeroes = CountTrailingZeroBits(mask);

			while ((ret = *(uchar*)firstString - *(uchar*)secondString) == 0 && current < trailingZeroes)
				++firstString, ++secondString, ++current;

			break;
		}

		memory::AdvanceBytes(firstString, 32);
		memory::AdvanceBytes(secondString, 32);
	}

	return ((-ret) < 0) - (ret < 0);
}

template <>
DECLARE_NOALIAS int __CDECL BaseFeatureAwareStrcmp<arch::CpuFeature::AVX512BW>(
	const char* firstString,
	const char* secondString) noexcept
{
	int ret = 0;

	while (true) {
		const auto loadedFirst	= _mm512_loadu_si512(firstString);
		const auto loadedSecond = _mm512_loadu_si512(secondString);

		const auto mask = _mm512_cmpeq_epi8_mask(loadedSecond, _mm512_setzero_si512());

		// End of string not found
		if (mask == 0) {
			ret = ((_mm512_movepi8_mask(_mm512_sub_epi8(loadedFirst, loadedSecond))) == 0);

			if (ret == 0)
				break;
		}
		else {
			auto current = uint8(0);
			const auto trailingZeros = CountTrailingZeroBits(mask);

			while ((ret = *(uchar*)firstString - *(uchar*)secondString) == 0 && current < trailingZeros)
				++firstString, ++secondString, ++current;

			break;
		}

		memory::AdvanceBytes(firstString, 64);
		memory::AdvanceBytes(secondString, 64);
	}

	return ((-ret) < 0) - (ret < 0);
}

__BASE_STRING_NAMESPACE_END
