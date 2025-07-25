#include <src/core/string/crt/cs/internal/BaseStrstrnInternal.h>


__BASE_STRING_NAMESPACE_BEGIN

template <arch::CpuFeature feature>
DECLARE_NOALIAS const char* BaseFeatureAwareStrstr(
	const char*		mainString,
	const sizetype	mainLength,
	const char*		subString,
	const sizetype	subLength) noexcept
{
	AssertUnreachable();
	return nullptr;
}

template <>
DECLARE_NOALIAS const char* BaseFeatureAwareStrstr<arch::CpuFeature::None>(
	const char*		mainString,
	const sizetype	mainLength,
	const char*		subString,
	const sizetype	subLength) noexcept
{
	if (mainLength == subLength)
        return (memcmp(mainString, subString, mainLength) == 0) ? mainString : nullptr;

    const char first        = subString[0];
    const sizetype maxpos   = sizetype(mainLength) - sizetype(subLength) + 1;
    
    for (sizetype i = 0; i < maxpos; i++) {
        if (mainString[i] != first) {
            i++;

            while (i < maxpos && mainString[i] != first)
                i++;

            if (i == maxpos)
                break;
        }

        sizetype j = 1;
        
        for (; j < subLength; ++j)
            if (mainString[i + j] != subString[j])
                break;
        
        if (j == subLength)
            return (mainString + i);
    }

    return nullptr;
}

template <>
DECLARE_NOALIAS const char* BaseFeatureAwareStrstr<arch::CpuFeature::AVX512BW>(
	const char*		mainString,
	const sizetype	mainLength,
	const char*		subString,
	const sizetype	subLength) noexcept
{

}

template <>
DECLARE_NOALIAS const char* BaseFeatureAwareStrstr<arch::CpuFeature::AVX2>(
	const char*		mainString,
	const sizetype	mainLength,
	const char*		subString,
	const sizetype	subLength) noexcept
{

}

template <>
DECLARE_NOALIAS const char* BaseFeatureAwareStrstr<arch::CpuFeature::SSE2>(
	const char*		mainString,
	const sizetype	mainLength,
	const char*		subString,
	const sizetype	subLength) noexcept
{

}

__BASE_STRING_NAMESPACE_END
