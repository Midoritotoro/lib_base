#include <src/core/string/crt/cs/internal/BaseStrstrnEqualInternal.h>
#include <src/core/string/crt/cs/internal/BaseStrstrClangMsvcHelper.h>

__BASE_STRING_NAMESPACE_BEGIN

template <sizetype needleLength>
DECLARE_NOALIAS NODISCARD always_inline const char* BaseFeatureAwareStrstrnEqual<arch::CpuFeature::AVX2>(
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
        auto equal = _mm256_cmpeq_epi8(curr, broadcasted[0]);

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
        StringFindLoop<needleLength>()(equal, next1, curr, broadcasted);
#endif

        curr = next;
        const uint32_t mask = _mm256_movemask_epi8(equal);

        if (mask != 0)
            return string + i + CountTrailingZeroBits(mask);
    }

    return nullptr;
}

__BASE_STRING_NAMESPACE_END
