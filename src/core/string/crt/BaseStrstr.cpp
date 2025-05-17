#include <src/core/string/crt/BaseStrlen.h>
#include <base/core/arch/ProcessorFeatures.h>

#include <src/core/memory/MemoryUtility.h>
#include <base/core/utility/BitOps.h>

#include <src/core/string/crt/simd/VectorizedSafeStringAlgorithm.h>

__BASE_STRING_NAMESPACE_BEGIN

DECLARE_NOALIAS NODISCARD const char* CDECL __base_strstrnScalar(
    const char* hay,
    std::size_t size, 
    const char* needle,
    std::size_t needlesize) noexcept
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

DECLARE_NOALIAS NODISCARD const char* CDECL __base_strstr(
	const char* string,
	const char* needle) noexcept
{
	if (ProcessorFeatures::AVX512F())
		return __base_strstrnAvx512F(
			string, __base_strlen(string), 
			needle, __base_strlen(needle));

	else if (ProcessorFeatures::AVX2())
		return __base_strstrnAvx2(
			string, __base_strlen(string),
			needle, __base_strlen(needle));

	else if (ProcessorFeatures::SSE2())
		return __base_strstrnSse2(
			string, __base_strlen(string),
			needle, __base_strlen(needle));

	return __base_strstrnScalar(
		string, __base_strlen(string),
		needle, __base_strlen(needle));
}


__BASE_STRING_NAMESPACE_END
