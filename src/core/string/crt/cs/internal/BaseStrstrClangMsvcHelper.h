#pragma once 

#include <base/core/Types.h>


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
struct StringFindLoopImplementation;

template <
    sizetype K,
    int i>
struct StringFindLoopImplementation<K, i, false> {
    void operator()(
        __m256i& eq,
        const __m256i& next1,
        const __m256i& curr,
        const __m256i   (&broadcasted)[K])
    {
        const __m256i substring = _mm256_alignr_epi8(next1, curr, i);
        eq = _mm256_and_si256(eq, _mm256_cmpeq_epi8(substring, broadcasted[i]));

        StringFindLoopImplementation<K, i + 1, i + 1 == K>()(eq, next1, curr, broadcasted);
    }
};

template <
    sizetype    K,
    int         i>
struct StringFindLoopImplementation<K, i, true> {
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
struct StringFindLoop {
    void operator()(
        __m256i& eq,
        const __m256i& next1,
        const __m256i& curr,
        const __m256i   (&broadcasted)[K])
    {
        static_assert(K > 0, "wrong value");
        StringFindLoopImplementation<K, 0, false>()(eq, next1, curr, broadcasted);
    }
};

#endif

__BASE_STRING_NAMESPACE_END
