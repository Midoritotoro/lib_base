#include <src/core/string/crt/BaseStrstrn.h>
#include <base/core/arch/ProcessorFeatures.h>

#include <src/core/memory/MemoryUtility.h>
#include <base/core/utility/BitOps.h>

#include <src/core/string/crt/simd/VectorizedSafeStringAlgorithm.h>

__BASE_STRING_NAMESPACE_BEGIN

DECLARE_NOALIAS const char* CDECL __base_strstrn(
	const char*			mainString,
	const std::size_t	mainLength,
	const char*			subString,
	const std::size_t	subLength) noexcept
{
    if (ProcessorFeatures::AVX512F())
        return __base_strstrnAvx512F(mainString, mainLength, subString, subLength);
    else if (ProcessorFeatures::AVX2())
        return __base_strstrnAvx2(mainString, mainLength, subString, subLength);
    else if (ProcessorFeatures::SSE2())
        return __base_strstrnSse2(mainString, mainLength, subString, subLength);

    return __base_strstrnScalar(mainString, mainLength, subString, subLength);
}

//DECLARE_NOALIAS const wchar_t* CDECL __base_wcsstrn(
//	const wchar_t*		mainString,
//	const std::size_t	mainLength,
//	const wchar_t*		subString,
//	const std::size_t	subLength) noexcept
//{
//
//    return __base_wcsstrstrnScalar(
//        memory::UnCheckedToWChar(mainString), mainLength,
//        memory::UnCheckedToWChar(subString), subLength);
//}

__BASE_STRING_NAMESPACE_END
