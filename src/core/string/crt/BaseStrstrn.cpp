#include <src/core/string/crt/BaseStrstrn.h>
#include <base/core/arch/ProcessorFeatures.h>

#include <src/core/memory/MemoryUtility.h>
#include <base/core/utility/BitOps.h>

__BASE_STRING_NAMESPACE_BEGIN

NODISCARD char* __base_strstrnScalar(
    char*       hay,
    std::size_t size, 
    char*       needle,
    std::size_t needlesize) 
{
    if (size == needlesize)
        return memcmp(hay, needle, size) == 0
            ? hay 
            : nullptr;

    const char first = needle[0];
    const std::size_t maxpos = std::size_t(size) - std::size_t(needlesize) + 1;
    
    for (std::size_t i = 0; i < maxpos; i++) {
        if (hay[i] != first) {
            i++;

            while (i < maxpos && hay[i] != first)
                i++;

            if (i == maxpos)
                break;
        }

        std::size_t j = 1;
        
        for (; j < needlesize; ++j)
            if (hay[i + j] != needle[j]) 
                break;
        
        if (j == needlesize)
            return (hay + i);
    }

    return nullptr;
}


NODISCARD wchar_t* __base_wcsstrstrnScalar(
    wchar_t*    hay,
    std::size_t size,
    wchar_t*    needle,
    std::size_t needlesize)
{

    if (size == needlesize)
        return memcmp(hay, needle, size * sizeof(wchar_t)) == 0
        ? hay
        : nullptr;

    const char first = needle[0];
    const std::size_t maxpos = std::size_t(size) - std::size_t(needlesize) + 1;

    for (std::size_t i = 0; i < maxpos; i++) {
        if (hay[i] != first) {
            i++;

            while (i < maxpos && hay[i] != first)
                i++;

            if (i == maxpos)
                break;
        }

        std::size_t j = 1;

        for (; j < needlesize; ++j)
            if (hay[i + j] != needle[j])
                break;

        if (j == needlesize)
            return (hay + i);
    }

    return nullptr;
}

NODISCARD char* __base_strstrnAvx2(
    char*   string,
    size_t  stringLength,
    char*   needle,
    size_t  needleLength) 
{

    const auto first = _mm256_set1_epi8(needle[0]);
    const auto last = _mm256_set1_epi8(needle[needleLength - 1]);

    for (size_t i = 0; i < stringLength; i += 32) {
        const auto blockFirst = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(string + i));
        const auto blockLast = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(string + i + needleLength - 1));

        const auto equalFirst = _mm256_cmpeq_epi8(first, blockFirst);
        const auto equalLast = _mm256_cmpeq_epi8(last, blockLast);

        uint32_t mask = _mm256_movemask_epi8(_mm256_and_si256(equalFirst, equalLast));

        while (mask != 0) {

            const auto bitpos = CountTrailingZeroBits(mask);

            if (memcmp(
                string + i + bitpos + 1, needle + 1,
                needleLength - 2) == 0
            )
                return (string + i + bitpos);

            mask = ClearLeftMostSet(mask);
        }
    }

    return nullptr;
}

NODISCARD char* __base_strstrnSse42(
    char*   string,
    size_t  stringLength,
    char*   needle,
    size_t  needleLength)
{
    return nullptr;
}

NODISCARD char* __base_strstrnSse2(
    char*   string,
    size_t  stringLength,
    char*   needle,
    size_t  needleLength)
{
    return nullptr;
}

NODISCARD char* __base_strstrnAvx512F(
    char* string,
    size_t  stringLength,
    char* needle,
    size_t  needleLength)
{
    return nullptr;
}



DECLARE_NOALIAS char* CDECL __base_strstrn(
	const char*			mainString,
	const std::size_t	mainLength,
	const char*			subString,
	const std::size_t	subLength) noexcept
{
    if (ProcessorFeatures::AVX512F())
        return __base_strstrnAvx512F(
            memory::UnCheckedToChar(mainString), mainLength,
            memory::UnCheckedToChar(subString), subLength);

    else if (ProcessorFeatures::AVX2())
        return __base_strstrnAvx2(
            memory::UnCheckedToChar(mainString), mainLength,
            memory::UnCheckedToChar(subString), subLength);

    else if (ProcessorFeatures::SSE42())
        return __base_strstrnSse42(
            memory::UnCheckedToChar(mainString), mainLength,
            memory::UnCheckedToChar(subString), subLength);

    else if (ProcessorFeatures::SSE2())
        return __base_strstrnSse2(
            memory::UnCheckedToChar(mainString), mainLength,
            memory::UnCheckedToChar(subString), subLength);

    return __base_strstrnScalar(
        memory::UnCheckedToChar(mainString), mainLength,
        memory::UnCheckedToChar(subString), subLength);
}

DECLARE_NOALIAS wchar_t* CDECL __base_wcsstrn(
	const wchar_t*		mainString,
	const std::size_t	mainLength,
	const wchar_t*		subString,
	const std::size_t	subLength) noexcept
{

    return __base_wcsstrstrnScalar(
        memory::UnCheckedToWChar(mainString), mainLength,
        memory::UnCheckedToWChar(subString), subLength);
}

__BASE_STRING_NAMESPACE_END
