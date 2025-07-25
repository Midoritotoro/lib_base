#include <src/core/string/crt/cs/internal/BaseStrstrnMemcmpInternal.h>
#include <base/core/utility/BitOps.h>


#if defined(CPP_CLANG) || defined(CPP_MSVC)
    #include <src/core/string/crt/cs/internal/BaseStrstrClangMsvcHelper.h>
#endif

__BASE_STRING_NAMESPACE_BEGIN

template <
	sizetype needleLength,
	typename _MemCmpLike_>
DECLARE_NOALIAS const char* BaseFeatureAwareStrstrnMemcmp<needleLength, _MemCmpLike_, arch::CpuFeature::AVX512F>(
	const char*		mainString,
	const sizetype	mainLength,
	const char*		subString,
	_MemCmpLike_	memcmpLike) noexcept
{
    if constexpr (needleLength <= 0)
        return nullptr;

    if (mainLength <= 0)
        return nullptr;

    const auto first    = _mm512_set1_epi8(subString[0]);
    const auto last     = _mm512_set1_epi8(subString[needleLength - 1]);

    char* haystack = const_cast<char*>(mainString);
    char* end = haystack + mainLength;

    for (/**/; haystack < end; haystack += 64) {

        const auto block_first  = _mm512_loadu_si512(haystack + 0);
        const auto block_last   = _mm512_loadu_si512(haystack + needleLength - 1);

        const auto first_zeros  = _mm512_xor_si512(block_first, first);
        const auto zeros        = _mm512_ternarylogic_epi32(first_zeros, block_last, last, 0xf6);

        uint32_t mask = ZeroByteMask(zeros);

        while (mask) {

            const uint64_t p = CountTrailingZeroBits(mask);

            if (memcmpLike(haystack + 4 * p + 0, subString))
                return mainString + ((haystack - mainString) + 4 * p + 0);

            if (memcmpLike(haystack + 4 * p + 1, subString))
                return mainString + ((haystack - mainString) + 4 * p + 1);

            if (memcmpLike(haystack + 4 * p + 2, subString))
                return mainString + ((haystack - mainString) + 4 * p + 2);

            if (memcmpLike(haystack + 4 * p + 3, subString))
                return mainString + ((haystack - mainString) + 4 * p + 3);

            mask = ClearLeftMostSet(mask);
        }
    }

    return nullptr;
}

template <
	sizetype needleLength,
	typename _MemCmpLike_>
DECLARE_NOALIAS const char* BaseFeatureAwareStrstrnMemcmp<needleLength, _MemCmpLike_, arch::CpuFeature::AVX2>(
	const char*		mainString,
	const sizetype	mainLength,
	const char*		subString,
	_MemCmpLike_	memcmpLike) noexcept
{
    if constexpr (needleLength <= 0)
        return nullptr;

    if (mainLength <= 0)
        return nullptr;

    const auto first = _mm256_set1_epi8(subString[0]);
    const auto last = _mm256_set1_epi8(subString[needleLength - 1]);

    for (sizetype i = 0; i < mainLength; i += 32) {

        const auto blockFirst   = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(mainString + i));
        const auto blockLast    = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(mainString + i + needleLength - 1));

        const auto equalFirst = _mm256_cmpeq_epi8(first, blockFirst);
        const auto equalLast = _mm256_cmpeq_epi8(last, blockLast);

        uint32_t mask = _mm256_movemask_epi8(_mm256_and_si256(equalFirst, equalLast));

        while (mask != 0) {
            const auto bitpos = CountTrailingZeroBits(mask);

            if (memcmpLike(mainString + i + bitpos + 1, subString + 1))
                return mainString + i + bitpos;
           
            mask = ClearLeftMostSet(mask);
        }
    }

    return nullptr;
}

template <
	sizetype needleLength,
	typename _MemCmpLike_>
DECLARE_NOALIAS const char* BaseFeatureAwareStrstrnMemcmp<needleLength, _MemCmpLike_, arch::CpuFeature::SSE2>(
	const char*		mainString,
	const sizetype	mainLength,
	const char*		subString,
	_MemCmpLike_	memcmpLike) noexcept
{
	if constexpr (needleLength <= 0)
        return nullptr;

    if (mainLength <= 0)
        return nullptr;

    const auto first    = _mm_set1_epi8(subString[0]);
    const auto last     = _mm_set1_epi8(subString[needleLength - 1]);

    for (sizetype i = 0; i < mainLength; i += 16) {

        const auto blockFirst   = _mm_loadu_si128(reinterpret_cast<const __m128i*>(string + i));
        const auto blockLast    = _mm_loadu_si128(reinterpret_cast<const __m128i*>(string + i + needleLength - 1));

        const auto equalFirst   = _mm_cmpeq_epi8(first, blockFirst);
        const auto equalLast    = _mm_cmpeq_epi8(last, blockLast);

        uint32_t mask = _mm_movemask_epi8(_mm_and_si128(equalFirst, equalLast));

        while (mask != 0) {

            const auto bitpos = CountTrailingZeroBits(mask);

            if (memcmpLike(mainString + i + bitpos + 1, subString + 1))
                return mainString + i + bitpos;

            mask = ClearLeftMostSet(mask);
        }
    }

    return nullptr;
}

__BASE_STRING_NAMESPACE_END
