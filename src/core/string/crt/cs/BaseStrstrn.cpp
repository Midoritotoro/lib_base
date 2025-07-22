#include <src/core/string/crt/cs/BaseStrstrn.h>
#include <base/core/arch/ProcessorFeatures.h>

#include <src/core/memory/MemoryUtility.h>
#include <base/core/utility/BitOps.h>

#include <src/core/string/crt/VectorizedSafeStringAlgorithm.h>
#include <src/core/memory/crt/FixedMemcmp.h>

__BASE_STRING_NAMESPACE_BEGIN


/*
    The following templates implement the loop, where K is a template parameter.

        for (unsigned i=1; i < K; i++) {
            const __m256i substring = _mm256_alignr_epi8(next1, curr, i);
            eq = _mm256_and_si256(eq, _mm256_cmpeq_epi8(substring, broadcasted[i]));
        }

    Clang and MSVC complains that the loop parameter `i` is a variable and it cannot be
    applied as a parameter _mm256_alignr_epi8.  GCC somehow deals with it.
*/

#if defined(CPP_CLANG) || defined(CPP_MSVC)

template <
    sizetype    K,
    int         i,
    bool        terminate>
struct inner_loop_aux;

template <
    sizetype K,
    int i>
struct inner_loop_aux<K, i, false> {
    void operator()(
        __m256i&        eq,
        const __m256i&  next1,
        const __m256i&  curr,
        const __m256i   (&broadcasted)[K])
    {
        const __m256i substring = _mm256_alignr_epi8(next1, curr, i);
        eq                      = _mm256_and_si256(eq, _mm256_cmpeq_epi8(substring, broadcasted[i]));

        inner_loop_aux<K, i + 1, i + 1 == K>()(eq, next1, curr, broadcasted);
    }
};

template <
    sizetype    K,
    int         i>
struct inner_loop_aux<K, i, true> {
    void operator()(
        __m256i&,
        const __m256i&,
        const __m256i&,
        const __m256i (&)[K])
    {
        // nop
    }
};

template <sizetype K>
struct inner_loop {
    void operator()(
        __m256i&        eq,
        const __m256i&  next1,
        const __m256i&  curr,
        const __m256i   (&broadcasted)[K])
    {
        static_assert(K > 0, "wrong value");
        inner_loop_aux<K, 0, false>()(eq, next1, curr, broadcasted);
    }
};

#endif

DECLARE_NOALIAS NODISCARD const char* CDECL __base_strstrnScalar(
    const char* hay,
    sizetype    size, 
    const char* needle,
    sizetype    needlesize) noexcept
{
    if (size == needlesize)
        return (memcmp(hay, needle, size) == 0) ? hay : nullptr;

    const char first        = needle[0];
    const sizetype maxpos   = sizetype(size) - sizetype(needlesize) + 1;
    
    for (sizetype i = 0; i < maxpos; i++) {
        if (hay[i] != first) {
            i++;

            while (i < maxpos && hay[i] != first)
                i++;

            if (i == maxpos)
                break;
        }

        sizetype j = 1;
        
        for (; j < needlesize; ++j)
            if (hay[i + j] != needle[j]) 
                break;
        
        if (j == needlesize)
            return (hay + i);
    }

    return nullptr;
}

// ======================================================================================

DECLARE_NOALIAS NODISCARD always_inline const char* __base_strstrnSse2AnySize(
    const char* string, 
    sizetype    stringLength,
    const char* needle,
    sizetype    needleLength) noexcept
{
    if (stringLength <= 0 || needleLength <= 0)
        return nullptr;

    const auto first    = _mm_set1_epi8(needle[0]);
    const auto last     = _mm_set1_epi8(needle[needleLength - 1]);

    for (sizetype i = 0; i < stringLength; i += 16) {

        const auto blockFirst   = _mm_loadu_si128(reinterpret_cast<const __m128i*>(string + i));
        const auto blockLast    = _mm_loadu_si128(reinterpret_cast<const __m128i*>(string + i + needleLength - 1));

        const auto equalFirst   = _mm_cmpeq_epi8(first, blockFirst);
        const auto equalLast    = _mm_cmpeq_epi8(last, blockLast);

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
    sizetype needleLength, 
    typename _MemCmpLike_>
DECLARE_NOALIAS NODISCARD always_inline const char* __base_strstrnSse2Memcmp(
    const char*     string, 
    sizetype          stringLength, 
    const char*     needle,
    _MemCmpLike_  memcmpLike) noexcept
{
    if constexpr (needleLength <= 0)
        return nullptr;

    if (stringLength <= 0)
        return nullptr;

    const auto first    = _mm_set1_epi8(needle[0]);
    const auto last     = _mm_set1_epi8(needle[needleLength - 1]);

    for (sizetype i = 0; i < stringLength; i += 16) {

        const auto blockFirst   = _mm_loadu_si128(reinterpret_cast<const __m128i*>(string + i));
        const auto blockLast    = _mm_loadu_si128(reinterpret_cast<const __m128i*>(string + i + needleLength - 1));

        const auto equalFirst   = _mm_cmpeq_epi8(first, blockFirst);
        const auto equalLast    = _mm_cmpeq_epi8(last, blockLast);

        uint32_t mask = _mm_movemask_epi8(_mm_and_si128(equalFirst, equalLast));

        while (mask != 0) {

            const auto bitpos = CountTrailingZeroBits(mask);

            if (memcmpLike(string + i + bitpos + 1, needle + 1))
                return string + i + bitpos;

            mask = ClearLeftMostSet(mask);
        }
    }

    return nullptr;
}

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

// ======================================================================================

DECLARE_NOALIAS NODISCARD always_inline const char* __base_strstrnAvx2AnySize(
    const char* string,
    sizetype    stringLength,
    const char* needle,
    sizetype    needleLength) noexcept
{
    if (needleLength <= 0 || stringLength <= 0)
        return nullptr;

    const auto first    = _mm256_set1_epi8(needle[0]);
    const auto last     = _mm256_set1_epi8(needle[needleLength - 1]);

    for (sizetype i = 0; i < stringLength; i += 32) {
        const auto blockFirst   = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(string + i));
        const auto blockLast    = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(string + i + needleLength - 1));

        const auto equalFirst   = _mm256_cmpeq_epi8(first, blockFirst);
        const auto equalLast    = _mm256_cmpeq_epi8(last, blockLast);

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

template <
    sizetype    needleLength, 
    typename    _MemCmpLike_>
DECLARE_NOALIAS NODISCARD always_inline const char* __base_strstrnAvx2Memcmp(
    const char*     string,
    sizetype        stringLength,
    const char*     needle,
    _MemCmpLike_    memcmpLike) noexcept
{
    if constexpr (needleLength <= 0)
        return nullptr;

    if (stringLength <= 0)
        return nullptr;

    const auto first = _mm256_set1_epi8(needle[0]);
    const auto last = _mm256_set1_epi8(needle[needleLength - 1]);

    for (sizetype i = 0; i < stringLength; i += 32) {

        const auto blockFirst = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(string + i));
        const auto blockLast = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(string + i + needleLength - 1));

        const auto equalFirst = _mm256_cmpeq_epi8(first, blockFirst);
        const auto equalLast = _mm256_cmpeq_epi8(last, blockLast);

        uint32_t mask = _mm256_movemask_epi8(_mm256_and_si256(equalFirst, equalLast));

        while (mask != 0) {
            const auto bitpos = CountTrailingZeroBits(mask);

            if (memcmpLike(string + i + bitpos + 1, needle + 1))
                return string + i + bitpos;
           
            mask = ClearLeftMostSet(mask);
        }
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
        case 1:     const char* res = reinterpret_cast<const char*>(strchr(string, needle[0]));             return res;
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
    sizetype    stringLength,
    const char* needle,
    sizetype    needleLength) noexcept
{
    if (stringLength <= 0 || needleLength <= 0)
        return nullptr;

    const auto first    = _mm512_set1_epi8(needle[0]);
    const auto last     = _mm512_set1_epi8(needle[needleLength - 1]);

    char* haystack  = const_cast<char*>(string);
    char* end       = haystack + stringLength;

    for (; haystack < end; haystack += 64) {

        const auto blockFirst   = _mm512_loadu_si512(haystack + 0);
        const auto blockLast    = _mm512_loadu_si512(haystack + needleLength - 1);

        const auto firstZeros   = _mm512_xor_si512(blockFirst, first);

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
    sizetype needleLength,
    typename _MemCmpLike_>
DECLARE_NOALIAS NODISCARD always_inline const char* __base_strstrnAvx512FMemcmp(
    const char*     string,
    sizetype        stringLength,
    const char*     needle,
    _MemCmpLike_    memcmpLike) noexcept
{
    if constexpr (needleLength <= 0)
        return nullptr;

    if (stringLength <= 0)
        return nullptr;

    const auto first    = _mm512_set1_epi8(needle[0]);
    const auto last     = _mm512_set1_epi8(needle[needleLength - 1]);

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

            if (memcmpLike(haystack + 4 * p + 0, needle))
                return string + ((haystack - string) + 4 * p + 0);

            if (memcmpLike(haystack + 4 * p + 1, needle))
                return string + ((haystack - string) + 4 * p + 1);

            if (memcmpLike(haystack + 4 * p + 2, needle))
                return string + ((haystack - string) + 4 * p + 2);

            if (memcmpLike(haystack + 4 * p + 3, needle))
                return string + ((haystack - string) + 4 * p + 3);

            mask = ClearLeftMostSet(mask);
        }
    }

    return nullptr;
}

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


DECLARE_NOALIAS const char* CDECL __base_strstrn(
	const char*			mainString,
	const sizetype	mainLength,
	const char*			subString,
	const sizetype	subLength) noexcept
{
    if (ProcessorFeatures::AVX512F())
        return __base_strstrnAvx512F(mainString, mainLength, subString, subLength);
    else if (ProcessorFeatures::AVX2())
        return __base_strstrnAvx2(mainString, mainLength, subString, subLength);
    else if (ProcessorFeatures::SSE2())
        return __base_strstrnSse2(mainString, mainLength, subString, subLength);

    return __base_strstrnScalar(mainString, mainLength, subString, subLength);
}

__BASE_STRING_NAMESPACE_END
