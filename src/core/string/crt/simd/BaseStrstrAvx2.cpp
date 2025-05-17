#include <src/core/string/crt/simd/VectorizedSafeStringAlgorithm.h>
#include <src/core/string/crt/simd/BaseStrStrAvx2Clang.h>

#include <base/core/utility/BitOps.h>
#include <base/core/string/String.h>

#include <src/core/memory/crt/FixedMemcmp.h>

__BASE_STRING_NAMESPACE_BEGIN

DECLARE_NOALIAS NODISCARD always_inline const char* __base_strstrnAvx2AnySize(
    const char* string,
    size_t      stringLength,
    const char* needle,
    size_t      needleLength) noexcept
{
    if (needleLength <= 0 || stringLength <= 0)
        return nullptr;

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

            if (memcmp(string + i + bitpos + 1, needle + 1, needleLength - 2) == 0)
                return string + i + bitpos;
            
            mask = ClearLeftMostSet(mask);
        }
    }

    return nullptr;
}

template <size_t needleLength>
DECLARE_NOALIAS NODISCARD always_inline const char* __base_strstrnAvx2Equal(
    const char* string, 
    size_t      stringLength, 
    const char* needle) noexcept
{
    static_assert(needleLength > 0 && needleLength < 16, "needleLength must be in range [1..15]");
    
    if (stringLength <= 0)
        return nullptr;

    __m256i broadcasted[needleLength];

    for (unsigned i = 0; i < needleLength; i++)
        broadcasted[i] = _mm256_set1_epi8(needle[i]);

    auto curr = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(string));

    for (size_t i = 0; i < needleLength; i += 32) {

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

#ifndef CPP_CLANG
        for (unsigned i = 1; i < needleLength; i++) {
            const auto substring = _mm256_alignr_epi8(next1, curr, i);
            equal = _mm256_and_si256(equal, _mm256_cmpeq_epi8(substring, broadcasted[i]));
        }
#else
        inner_loop<needleLength>()(eq, next1, curr, broadcasted);
#endif

        curr = next;
        const uint32_t mask = _mm256_movemask_epi8(equal);

        if (mask != 0)
            return string + i + CountTrailingZeroBits(mask);
    }

    return nullptr;
}

template <
    size_t      needleLength, 
    typename    MEMCMP>
DECLARE_NOALIAS NODISCARD always_inline const char* __base_strstrnAvx2Memcmp(
    const char* string,
    size_t      stringLength,
    const char* needle,
    MEMCMP      memcmpFunction) noexcept
{
    if constexpr (needleLength <= 0)
        return nullptr;

    if (stringLength <= 0)
        return nullptr;

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

            if (memcmpFunction(string + i + bitpos + 1, needle + 1))
                return string + i + bitpos;
           
            mask = ClearLeftMostSet(mask);
        }
    }

    return nullptr;
}

DECLARE_NOALIAS NODISCARD const char* __base_strstrnAvx2(
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

        case 1: {
            const char* res = reinterpret_cast<const char*>(strchr(string, needle[0]));
            return res;
        }

        case 2:
            result = __base_strstrnAvx2Equal<2>(string, stringLength, needle);
            break;

        case 3:
            result = __base_strstrnAvx2Memcmp<3>(string, stringLength, 
                needle, memory::memcmp1);
            break;

        case 4:
            result = __base_strstrnAvx2Memcmp<4>(string, stringLength,
                needle, memory::memcmp2);
            break;

        case 5:
            // Note: use memcmp4 rather memcmp3, as the last character
            //       of needle is already proven to be equal
            result = __base_strstrnAvx2Memcmp<5>(string, stringLength, 
                needle, memory::memcmp4);
            break;

        case 6:
            result = __base_strstrnAvx2Memcmp<6>(string, stringLength,
                needle, memory::memcmp4);
            break;

        case 7:
            result = __base_strstrnAvx2Memcmp<7>(string, stringLength, 
                needle, memory::memcmp5);
            break;

        case 8:
            result = __base_strstrnAvx2Memcmp<8>(string, stringLength, 
                needle, memory::memcmp6);
            break;

        case 9:
            // Note: use memcmp8 rather memcmp7 for the same reason as above.
            result = __base_strstrnAvx2Memcmp<9>(string, stringLength, 
                needle, memory::memcmp8);
            break;

        case 10:
            result = __base_strstrnAvx2Memcmp<10>(string, stringLength,
                needle, memory::memcmp8);
            break;

        case 11:
            result = __base_strstrnAvx2Memcmp<11>(string, stringLength,
                needle, memory::memcmp9);
            break;

        case 12:
            result = __base_strstrnAvx2Memcmp<12>(string, stringLength, 
                needle, memory::memcmp10);
            break;

        default:
            result = __base_strstrnAvx2(string, stringLength,
                needle, needleLength);
            break;
    }

    if (result - string <= stringLength - needleLength)
        return result;

    return nullptr;
}


__BASE_STRING_NAMESPACE_END
