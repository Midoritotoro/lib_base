#include <src/core/string/crt/cs/BaseStrstrn.h>
#include <base/core/arch/ProcessorFeatures.h>

#include <src/core/memory/MemoryUtility.h>
#include <base/core/utility/BitOps.h>

#include <src/core/string/crt/VectorizedSafeStringAlgorithm.h>
#include <src/core/memory/crt/FixedMemcmp.h>

__BASE_STRING_NAMESPACE_BEGIN





// ======================================================================================




DECLARE_NOALIAS NODISCARD const char* __base_strstrnSse2(
    const char* string,
    sizetype      stringLength,
    const char* needle, 
    sizetype      needleLength) noexcept 
{
    const char* result = nullptr;

    if (stringLength < needleLength)
        return result;

    switch (needleLength) {
        case 0:                                                                                             return 0;
        case 1:     { const char* res = reinterpret_cast<const char*>(strchr(string, needle[0]));           return res; }
        case 2:     result = __base_strstrnSse2Memcmp<2>(string, stringLength, needle, memory::alwaysTrue); break;
        case 3:     result = __base_strstrnSse2Memcmp<3>(string, stringLength, needle, memory::memcmp1);    break;
        case 4:     result = __base_strstrnSse2Memcmp<4>(string, stringLength, needle, memory::memcmp2);    break;
        case 5:     result = __base_strstrnSse2Memcmp<5>(string, stringLength, needle, memory::memcmp4);    break;
        case 6:     result = __base_strstrnSse2Memcmp<6>(string, stringLength, needle, memory::memcmp4);    break;
        case 7:     result = __base_strstrnSse2Memcmp<7>(string, stringLength, needle, memory::memcmp5);    break;
        case 8:     result = __base_strstrnSse2Memcmp<8>(string, stringLength, needle, memory::memcmp6);    break;
        case 9:     result = __base_strstrnSse2Memcmp<9>(string, stringLength, needle, memory::memcmp8);    break;
        case 10:    result = __base_strstrnSse2Memcmp<10>(string, stringLength, needle, memory::memcmp8);   break;
        case 11:    result = __base_strstrnSse2Memcmp<11>(string, stringLength, needle, memory::memcmp9);   break;
        case 12:    result = __base_strstrnSse2Memcmp<12>(string, stringLength, needle, memory::memcmp10);  break;
        default:    result = __base_strstrnSse2AnySize(string, stringLength, needle, needleLength);         break;
     }

    if (result - string <= stringLength - needleLength)
        return result;

    return nullptr;
}

// ======================================================================================

template <sizetype needleLength>
DECLARE_NOALIAS NODISCARD always_inline const char* __base_strstrnAvx2Equal(
    const char* string, 
    sizetype    stringLength, 
    const char* needle) noexcept
{
    static_assert(needleLength > 0 && needleLength < 16, "needleLength must be in range [1..15]");
    
    if (stringLength <= 0)
        return nullptr;

    __m256i broadcasted[needleLength];

    for (unsigned i = 0; i < needleLength; i++)
        broadcasted[i] = _mm256_set1_epi8(needle[i]);

    auto curr = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(string));

    for (sizetype i = 0; i < needleLength; i += 32) {

        const auto next = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(string + i + 32));
        auto equal      = _mm256_cmpeq_epi8(curr, broadcasted[0]);

        // AVX2 palignr works on 128-bit lanes, thus some extra work is needed
        //
        // curr = [a, b] (2 x 128 bit)
        // next = [c, d]
        // substring = [palignr(b, a, i), palignr(c, b, i)]
        __m256i next1;

        next1 = _mm256_inserti128_si256(next1, _mm256_extracti128_si256(curr, 1), 0); // b
        next1 = _mm256_inserti128_si256(next1, _mm256_extracti128_si256(next, 0), 1); // c

#if !defined(CPP_CLANG) && !defined(CPP_MSVC)
        for (unsigned i = 1; i < needleLength; i++) {
            const auto substring = _mm256_alignr_epi8(next1, curr, i);
            equal = _mm256_and_si256(equal, _mm256_cmpeq_epi8(substring, broadcasted[i]));
        }
#else
        inner_loop<needleLength>()(equal, next1, curr, broadcasted);
#endif

        curr = next;
        const uint32_t mask = _mm256_movemask_epi8(equal);

        if (mask != 0)
            return string + i + CountTrailingZeroBits(mask);
    }

    return nullptr;
}


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
