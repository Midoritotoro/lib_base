#include <src/core/string/crt/cs/internal/BaseStrstrnAnySizeInternal.h>
#include <base/core/utility/BitOps.h>

#include <src/core/utility/simd/SimdHelpers.h>

__BASE_STRING_NAMESPACE_BEGIN

#define _mm512_set1_epu8(c) _mm512_set1_epi32(uint32_t(c) * 0x01010101u)

template <>
DECLARE_NOALIAS const char* BaseFeatureAwareStrstrnAnySize<arch::CpuFeature::AVX512F>(
	const char*		mainString,
	const sizetype	mainLength,
	const char*		subString,
	const sizetype	subLength) noexcept
{

	if (mainLength <= 0 || subLength <= 0)
        return nullptr;

    const auto first    = _mm512_set1_epu8(subString[0]);
    const auto last     = _mm512_set1_epu8(subString[subLength - 1]);

    char* haystack  = const_cast<char*>(mainString);
    char* end       = haystack + mainLength;

    for (; haystack < end; haystack += 64) {

        const auto blockFirst   = _mm512_loadu_si512(haystack + 0);
        const auto blockLast    = _mm512_loadu_si512(haystack + subLength - 1);

        const auto firstZeros   = _mm512_xor_si512(blockFirst, first);

        /*
            first_zeros | block_last | last |  first_zeros | (block_last ^ last)
            ------------+------------+------+------------------------------------
                 0      |      0     |   0  |      0
                 0      |      0     |   1  |      1
                 0      |      1     |   0  |      1
                 0      |      1     |   1  |      0
                 1      |      0     |   0  |      1
                 1      |      0     |   1  |      1
                 1      |      1     |   0  |      1
                 1      |      1     |   1  |      1
        */

        const auto zeros = _mm512_ternarylogic_epi32(firstZeros, blockLast, last, 0xf6);
        uint32_t mask = ZeroByteMask(zeros);

        while (mask) {

            const uint64_t p = CountTrailingZeroBits(mask);

            if (memcmp(haystack + 4 * p + 0, subString, subLength) == 0)
                return mainString + ((haystack - mainString) + 4 * p + 0);

            if (memcmp(haystack + 4 * p + 1, subString, subLength) == 0)
                return mainString + ((haystack - mainString) + 4 * p + 1);

            if (memcmp(haystack + 4 * p + 2, subString, subLength) == 0)
                return mainString + ((haystack - mainString) + 4 * p + 2);

            if (memcmp(haystack + 4 * p + 3, subString, subLength) == 0)
                return mainString + ((haystack - mainString) + 4 * p + 3);

            mask = ClearLeftMostSet(mask);
        }
    }

    return nullptr;
}

template <>
DECLARE_NOALIAS const char* BaseFeatureAwareStrstrnAnySize<arch::CpuFeature::AVX2>(
	const char*		mainString,
	const sizetype	mainLength,
	const char*		subString,
	const sizetype	subLength) noexcept
{
	if (subLength <= 0 || mainLength <= 0)
        return nullptr;

    const auto first    = _mm256_set1_epi8(subString[0]);
    const auto last     = _mm256_set1_epi8(subString[subLength - 1]);

    for (sizetype i = 0; i < mainLength; i += 32) {
        const auto blockFirst   = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(mainString + i));
        const auto blockLast    = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(mainString + i + subLength - 1));

        const auto equalFirst   = _mm256_cmpeq_epi8(first, blockFirst);
        const auto equalLast    = _mm256_cmpeq_epi8(last, blockLast);

        uint32_t mask = _mm256_movemask_epi8(_mm256_and_si256(equalFirst, equalLast));

        while (mask != 0) {
            const auto bitpos = CountTrailingZeroBits(mask);

            if (memcmp(mainString + i + bitpos + 1, subString + 1, subLength - 2) == 0)
                return mainString + i + bitpos;
            
            mask = ClearLeftMostSet(mask);
        }
    }

    return nullptr;
}

template <>
DECLARE_NOALIAS const char* BaseFeatureAwareStrstrnAnySize<arch::CpuFeature::SSE2>(
	const char*		mainString,
	const sizetype	mainLength,
	const char*		subString,
	const sizetype	subLength) noexcept
{
	if (mainLength <= 0 || subLength <= 0)
		return nullptr;

	const auto first = _mm_set1_epi8(subString[0]);
	const auto last = _mm_set1_epi8(subString[subLength - 1]);

	for (sizetype i = 0; i < mainLength; i += 16) {

		const auto blockFirst = _mm_loadu_si128(reinterpret_cast<const __m128i*>(mainString + i));
		const auto blockLast = _mm_loadu_si128(reinterpret_cast<const __m128i*>(mainString + i + subLength - 1));

		const auto equalFirst = _mm_cmpeq_epi8(first, blockFirst);
		const auto equalLast = _mm_cmpeq_epi8(last, blockLast);

		uint16_t mask = _mm_movemask_epi8(_mm_and_si128(equalFirst, equalLast));

		while (mask != 0) {

			const auto bitpos = CountTrailingZeroBits(mask);

			if (memcmp(mainString + i + bitpos + 1, subString + 1, subLength - 2) == 0)
				return mainString + i + bitpos;

			mask = ClearLeftMostSet(mask);
		}
	}

	return nullptr;
}

__BASE_STRING_NAMESPACE_END
