#include <src/core/string/crt/cs/BaseStrstrn.h>
#include <base/core/arch/ProcessorFeatures.h>

#include <src/core/memory/MemoryUtility.h>
#include <base/core/utility/BitOps.h>

#include <src/core/string/crt/VectorizedSafeStringAlgorithm.h>
#include <src/core/memory/crt/FixedMemcmp.h>

__BASE_STRING_NAMESPACE_BEGIN





// ======================================================================================




DECLARE_NOALIAS NODISCARD const char* __base_strstrnAvx2(
    const char*     string, 
    sizetype        stringLength, 
    const char*     needle,
    sizetype        needleLength) noexcept 
{
    const char* result = nullptr;

    if (stringLength < needleLength)
        return nullptr;

    switch (needleLength) {
        case 0:                                                                                             return 0;
        case 1:     { const char* res = reinterpret_cast<const char*>(strchr(string, needle[0]));           return res; }
        case 2:     result = __base_strstrnAvx2Equal<2>(string, stringLength, needle);                      break;
        case 3:     result = __base_strstrnAvx2Memcmp<3>(string, stringLength, needle, memory::memcmp1);    break;
        case 4:     result = __base_strstrnAvx2Memcmp<4>(string, stringLength, needle, memory::memcmp2);    break;
        // Note: use memcmp4 rather memcmp3, as the last character of needle is already proven to be equal
        case 5:     result = __base_strstrnAvx2Memcmp<5>(string, stringLength, needle, memory::memcmp4);    break;
        case 6:     result = __base_strstrnAvx2Memcmp<6>(string, stringLength, needle, memory::memcmp4);    break;
        case 7:     result = __base_strstrnAvx2Memcmp<7>(string, stringLength, needle, memory::memcmp5);    break;
        case 8:     result = __base_strstrnAvx2Memcmp<8>(string, stringLength, needle, memory::memcmp6);    break;
        // Note: use memcmp8 rather memcmp7 for the same reason as above.
        case 9:     result = __base_strstrnAvx2Memcmp<9>(string, stringLength, needle, memory::memcmp8);    break; 
        case 10:    result = __base_strstrnAvx2Memcmp<10>(string, stringLength, needle, memory::memcmp8);   break;
        case 11:    result = __base_strstrnAvx2Memcmp<11>(string, stringLength, needle, memory::memcmp9);   break;
        case 12:    result = __base_strstrnAvx2Memcmp<12>(string, stringLength, needle, memory::memcmp10);  break;
        default:    result = __base_strstrnAvx2(string, stringLength, needle, needleLength);                break;
    }

    if (result - string <= stringLength - needleLength)
        return result;

    return nullptr;
}

// ======================================================================================


DECLARE_NOALIAS NODISCARD const char* __base_strstrnAvx512F(
    const char* string,
    sizetype      stringLength,
    const char* needle,
    sizetype      needleLength) noexcept
{
    const char* result = nullptr;

    if (stringLength < needleLength)
        return nullptr;

    switch (needleLength) {
        case 0:                                                                                                 return 0;
        case 1:     { const char* res = reinterpret_cast<const char*>(strchr(string, needle[0]));               return res; }
        case 2:     result = __base_strstrnAvx512FMemcmp<2>(string, stringLength, needle, memory::memcmp2);     break;
        case 3:     result = __base_strstrnAvx512FMemcmp<3>(string, stringLength, needle, memory::memcmp3);     break;
        case 4:     result = __base_strstrnAvx512FMemcmp<4>(string, stringLength, needle, memory::memcmp4);     break;
        case 5:     result = __base_strstrnAvx512FMemcmp<5>(string, stringLength, needle, memory::memcmp5);     break;
        case 6:     result = __base_strstrnAvx512FMemcmp<6>(string, stringLength, needle, memory::memcmp6);     break;
        case 7:     result = __base_strstrnAvx512FMemcmp<7>(string, stringLength, needle, memory::memcmp7);     break;
        case 8:     result = __base_strstrnAvx512FMemcmp<8>(string, stringLength, needle, memory::memcmp8);     break;
        case 9:     result = __base_strstrnAvx512FMemcmp<9>(string, stringLength, needle, memory::memcmp9);     break;
        case 10:    result = __base_strstrnAvx512FMemcmp<10>(string, stringLength, needle, memory::memcmp10);   break;
        case 11:    result = __base_strstrnAvx512FMemcmp<11>(string, stringLength, needle, memory::memcmp11);   break;
        case 12:    result = __base_strstrnAvx512FMemcmp<12>(string, stringLength, needle, memory::memcmp12);   break;
        default:    result = __base_strstrnAvx512FAnySize(string, stringLength, needle, needleLength);          break;
    }

    if (result - string <= stringLength - needleLength)
        return result;

    return nullptr;
}


DECLARE_NOALIAS const char* CDECL __base_strstrn(
	const char*			mainString,
	const sizetype	mainLength,
	const char*			subString,
	const sizetype	subLength) noexcept
{
    if (arch::ProcessorFeatures::AVX512F())
        return __base_strstrnAvx512F(mainString, mainLength, subString, subLength);
    else if (arch::ProcessorFeatures::AVX2())
        return __base_strstrnAvx2(mainString, mainLength, subString, subLength);
    else if (arch::ProcessorFeatures::SSE2())
        return __base_strstrnSse2(mainString, mainLength, subString, subLength);

    return __base_strstrnScalar(mainString, mainLength, subString, subLength);
}

__BASE_STRING_NAMESPACE_END
