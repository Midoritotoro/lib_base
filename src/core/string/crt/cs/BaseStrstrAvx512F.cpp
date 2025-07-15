#include <src/core/string/crt/simd/VectorizedSafeStringAlgorithm.h>

#include <base/core/utility/BitOps.h>
#include <base/core/string/String.h>

#include <src/core/memory/crt/FixedMemcmp.h>

__BASE_STRING_NAMESPACE_BEGIN

DECLARE_NOALIAS NODISCARD __mmask16 always_inline ZeroByteMask(const __m512i v) {

    const auto v01 = _mm512_set1_epi8(0x01);
    const auto v80 = _mm512_set1_epi8(int8(0x80));

    const auto v1 = _mm512_sub_epi32(v, v01);
    // tmp1 = (v - 0x01010101) & ~v & 0x80808080
    const auto tmp1 = _mm512_ternarylogic_epi32(v1, v, v80, 0x20);
    
    return _mm512_test_epi32_mask(tmp1, tmp1);
}


DECLARE_NOALIAS NODISCARD always_inline const char* __base_strstrnAvx512FAnySize(
    const char* string, 
    size_t      stringLength,
    const char* needle,
    size_t      needleLength) noexcept
{
    if (stringLength <= 0 || needleLength <= 0)
        return nullptr;

    const auto first    = _mm512_set1_epi8(needle[0]);
    const auto last     = _mm512_set1_epi8(needle[needleLength - 1]);

    char* haystack      = const_cast<char*>(string);
    char* end           = haystack + stringLength;

    for (; haystack < end; haystack += 64) {

        const auto blockFirst = _mm512_loadu_si512(haystack + 0);
        const auto blockLast = _mm512_loadu_si512(haystack + needleLength - 1);

        const auto firstZeros = _mm512_xor_si512(blockFirst, first);

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

            if (memcmp(haystack + 4 * p + 0, needle, needleLength) == 0)
                return string + ((haystack - string) + 4 * p + 0);

            if (memcmp(haystack + 4 * p + 1, needle, needleLength) == 0)
                return string + ((haystack - string) + 4 * p + 1);
            
            if (memcmp(haystack + 4 * p + 2, needle, needleLength) == 0)
                return string + ((haystack - string) + 4 * p + 2);

            if (memcmp(haystack + 4 * p + 3, needle, needleLength) == 0)
                return string + ((haystack - string) + 4 * p + 3);

            mask = ClearLeftMostSet(mask);
        }
    }

    return nullptr;
}


template <
    size_t needleLength, 
    typename MemcmpFunction>
DECLARE_NOALIAS NODISCARD always_inline const char* __base_strstrnAvx512FMemcmp(
    const char*     string, 
    size_t          stringLength,
    const char*     needle,
    MemcmpFunction  memcmpFunction) noexcept
{
    if constexpr (needleLength <= 0)
        return nullptr;

    if (stringLength <= 0)
        return nullptr;

    const auto first = _mm512_set1_epi8(needle[0]);
    const auto last = _mm512_set1_epi8(needle[needleLength - 1]);

    char* haystack = const_cast<char*>(string);
    char* end = haystack + stringLength;

    for (/**/; haystack < end; haystack += 64) {

        const auto block_first = _mm512_loadu_si512(haystack + 0);
        const auto block_last = _mm512_loadu_si512(haystack + needleLength - 1);

        const auto first_zeros = _mm512_xor_si512(block_first, first);
        const auto zeros = _mm512_ternarylogic_epi32(first_zeros, block_last, last, 0xf6);

        uint32_t mask = ZeroByteMask(zeros);

        while (mask) {

            const uint64_t p = CountTrailingZeroBits(mask);

            if (memcmpFunction(haystack + 4 * p + 0, needle))
                return string + ((haystack - string) + 4 * p + 0);

            if (memcmpFunction(haystack + 4 * p + 1, needle))
                return string + ((haystack - string) + 4 * p + 1);

            if (memcmpFunction(haystack + 4 * p + 2, needle))
                return string + ((haystack - string) + 4 * p + 2);

            if (memcmpFunction(haystack + 4 * p + 3, needle))
                return string + ((haystack - string) + 4 * p + 3);

            mask = ClearLeftMostSet(mask);
        }
    }

    return nullptr;
}

DECLARE_NOALIAS NODISCARD const char* __base_strstrnAvx512F(
    const char* string,
    size_t      stringLength,
    const char* needle,
    size_t      needleLength) noexcept
{
    const char* result = nullptr;

    if (stringLength < needleLength)
        return nullptr;

    switch (needleLength) {
        case 0:
            return 0;

        case 1:     const char* res = reinterpret_cast<const char*>(strchr(string, needle[0]));                 return res;
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

__BASE_STRING_NAMESPACE_END
