#include <src/core/string/crt/simd/VectorizedSafeStringAlgorithm.h>

#include <base/core/utility/BitOps.h>
#include <base/core/string/String.h>

#include <src/core/memory/crt/FixedMemcmp.h>

__BASE_STRING_NAMESPACE_BEGIN

DECLARE_NOALIAS NODISCARD always_inline const char* __base_strstrnSse2AnySize(
    const char* string, 
    size_t      stringLength,
    const char* needle,
    size_t      needleLength) noexcept
{
    if (stringLength <= 0 || needleLength <= 0)
        return nullptr;

    const auto first = _mm_set1_epi8(needle[0]);
    const auto last = _mm_set1_epi8(needle[needleLength - 1]);

    for (size_t i = 0; i < stringLength; i += 16) {

        const auto blockFirst = _mm_loadu_si128(reinterpret_cast<const __m128i*>(string + i));
        const auto blockLast = _mm_loadu_si128(reinterpret_cast<const __m128i*>(string + i + needleLength - 1));

        const auto equalFirst = _mm_cmpeq_epi8(first, blockFirst);
        const auto equalLast = _mm_cmpeq_epi8(last, blockLast);

        uint16_t mask = _mm_movemask_epi8(_mm_and_si128(equalFirst, equalLast));

        while (mask != 0) {

            const auto bitpos = CountTrailingZeroBits(mask);

            if (memcmp(string + i + bitpos + 1, needle + 1, needleLength - 2) == 0)
                return string + i + bitpos;

            mask = ClearLeftMostSet(mask);
        }
    }

    return nullptr;
}

template <
    size_t needleLength, 
    typename MemcmpFunction>
DECLARE_NOALIAS NODISCARD always_inline const char* __base_strstrnSse2Memcmp(
    const char*     string, 
    size_t          stringLength, 
    const char*     needle,
    MemcmpFunction  memcmpFunction) noexcept
{
    if constexpr (needleLength <= 0)
        return nullptr;

    if (stringLength <= 0)
        return nullptr;

    const auto first = _mm_set1_epi8(needle[0]);
    const auto last = _mm_set1_epi8(needle[needleLength - 1]);

    for (size_t i = 0; i < stringLength; i += 16) {

        const auto blockFirst = _mm_loadu_si128(reinterpret_cast<const __m128i*>(string + i));
        const auto blockLast = _mm_loadu_si128(reinterpret_cast<const __m128i*>(string + i + needleLength - 1));

        const auto equalFirst = _mm_cmpeq_epi8(first, blockFirst);
        const auto equalLast = _mm_cmpeq_epi8(last, blockLast);

        uint32_t mask = _mm_movemask_epi8(_mm_and_si128(equalFirst, equalLast));

        while (mask != 0) {

            const auto bitpos = CountTrailingZeroBits(mask);

            if (memcmpFunction(string + i + bitpos + 1, needle + 1))
                return string + i + bitpos;

            mask = ClearLeftMostSet(mask);
        }
    }

    return nullptr;
}

DECLARE_NOALIAS NODISCARD const char* __base_strstrnSse2(
    const char* string,
    size_t      stringLength,
    const char* needle, 
    size_t      needleLength) noexcept 
{
    const char* result = nullptr;

    if (stringLength < needleLength)
        return result;

    switch (needleLength) {
        case 0:                                                                                             return 0;
        case 1:     const char* res = reinterpret_cast<const char*>(strchr(string, needle[0]));             return res;
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

__BASE_STRING_NAMESPACE_END
