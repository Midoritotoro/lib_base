#include <src/core/utility/simd/SimdAlgorithm.h>
#include <base/core/arch/ProcessorFeatures.h>

#include <src/core/memory/MemoryUtility.h>

__BASE_NAMESPACE_BEGIN


DECLARE_NOALIAS always_inline void __CDECL SwapRangesTriviallySwappableScalar(
    void* firstPointer1,
    void* lastPointer1,
    void* firstPointer2) noexcept
{
    const auto length = memory::ByteLength(firstPointer1, lastPointer1);

#if defined(PROCESSOR_X86_64)
    constexpr size_t mask8 = ~((static_cast<size_t>(1) << 3) - 1);

    if (length >= 8) {
        const void* stopAt = firstPointer1;
        memory::AdvanceBytes(stopAt, length & mask8);

        do {
            const uint64 left = *static_cast<uint64*>(firstPointer1);
            const uint64 right = *static_cast<uint64*>(firstPointer2);

            *static_cast<uint64*>(firstPointer1) = right;
            *static_cast<uint64*>(firstPointer2) = left;

            memory::AdvanceBytes(firstPointer1, 8);
            memory::AdvanceBytes(firstPointer2, 8);
        } while (firstPointer1 != stopAt);
    }
#elif defined(PROCESSOR_X86)
    constexpr size_t mask4 = ~((static_cast<size_t>(1) << 2) - 1);
    const auto length = memory::ByteLength(firstPointer1, lastPointer1);

    if (length >= 4) {
        const void* stopAt = firstPointer1;
        memory::AdvanceBytes(stopAt, length & mask4);

        do {
            const ulong left  = *static_cast<ulong*>(firstPointer1);
            const ulong right = *static_cast<ulong*>(firstPointer2);

            *static_cast<ulong*>(firstPointer1) = right;
            *static_cast<ulong*>(firstPointer2) = left;

            memory::AdvanceBytes(firstPointer1, 4);
            memory::AdvanceBytes(firstPointer2, 4);
        } while (firstPointer1 != stopAt);
    }
#endif

    auto firstPointer1Char = static_cast<uchar*>(firstPointer1);
    auto lastPointer1Char  = static_cast<uchar*>(lastPointer1);
    auto firstPointer2Char = static_cast<uchar*>(firstPointer2);

    for (; firstPointer1Char != lastPointer1Char; ++firstPointer1Char, ++firstPointer2Char) {
        uchar _Ch = *firstPointer1Char;

        *firstPointer1Char = *firstPointer2Char;
        *firstPointer2Char = _Ch;
    }
}

DECLARE_NOALIAS always_inline void __CDECL SwapRangesTriviallySwappableSse2(
    void* firstPointer1,
    void* lastPointer1,
    void* firstPointer2) noexcept
{
    constexpr size_t mask16 = ~((static_cast<size_t>(1) << 4) - 1);
    const auto length = memory::ByteLength(firstPointer1, lastPointer1);

    if (length >= 16) {
        const void* stopAt = firstPointer1;
        memory::AdvanceBytes(stopAt, length & mask16);

        do {
            const __m128i left  = _mm_loadu_si128(static_cast<__m128i*>(firstPointer1));
            const __m128i right = _mm_loadu_si128(static_cast<__m128i*>(firstPointer2));

            _mm_storeu_si128(static_cast<__m128i*>(firstPointer1), right);
            _mm_storeu_si128(static_cast<__m128i*>(firstPointer2), left);

            memory::AdvanceBytes(firstPointer1, 16);
            memory::AdvanceBytes(firstPointer2, 16);
        } while (firstPointer1 != stopAt);
    }

    SwapRangesTriviallySwappableScalar(firstPointer1, lastPointer1, firstPointer2);
}

DECLARE_NOALIAS always_inline void __CDECL SwapRangesTriviallySwappableAvx(
    void* firstPointer1,
    void* lastPointer1,
    void* firstPointer2) noexcept
{
    constexpr size_t mask32 = ~((static_cast<size_t>(1) << 5) - 1);
    const auto length = memory::ByteLength(firstPointer1, lastPointer1);

    if (length >= 32) {
        const void* stopAt = firstPointer1;
        memory::AdvanceBytes(stopAt, length & mask32);

        do {
            const __m256i left  = _mm256_lddqu_si256(static_cast<__m256i*>(firstPointer1));
            const __m256i right = _mm256_lddqu_si256(static_cast<__m256i*>(firstPointer2));

            _mm256_storeu_si256(static_cast<__m256i*>(firstPointer1), right);
            _mm256_storeu_si256(static_cast<__m256i*>(firstPointer2), left);

            memory::AdvanceBytes(firstPointer1, 32);
            memory::AdvanceBytes(firstPointer2, 32);
        } while (firstPointer1 != stopAt);

        _mm256_zeroupper();
    }

    SwapRangesTriviallySwappableSse2(firstPointer1, lastPointer1, firstPointer2);
}

DECLARE_NOALIAS always_inline void __CDECL SwapRangesTriviallySwappableAvx512(
    void* firstPointer1,
    void* lastPointer1,
    void* firstPointer2) noexcept
{
    constexpr size_t mask64 = ~((static_cast<size_t>(1) << 6) - 1);
    const auto length = memory::ByteLength(firstPointer1, lastPointer1);

    if (length >= 64) {
        const void* stopAt = firstPointer1;
        memory::AdvanceBytes(stopAt, length & mask64);

        do {
            const __m512i left  = _mm512_loadu_si512(firstPointer1);
            const __m512i right = _mm512_loadu_si512(firstPointer2);

            _mm512_storeu_si512(firstPointer1, right);
            _mm512_storeu_si512(firstPointer2, left);

            memory::AdvanceBytes(firstPointer1, 64);
            memory::AdvanceBytes(firstPointer2, 64);
        } while (firstPointer1 != stopAt);
    }

    SwapRangesTriviallySwappableAvx(firstPointer1, lastPointer1, firstPointer2);
}

DECLARE_NOALIAS void __CDECL SwapRangesTriviallySwappable(
    void* firstPointer1,
    void* lastPointer1,
    void* firstPointer2) noexcept
{
    if (arch::ProcessorFeatures::AVX512F())
        return SwapRangesTriviallySwappableAvx512(
            firstPointer1, lastPointer1, firstPointer2);
    else if (arch::ProcessorFeatures::AVX())
        return SwapRangesTriviallySwappableAvx(
            firstPointer1, lastPointer1, firstPointer2);
    else if (arch::ProcessorFeatures::SSE2())
        return SwapRangesTriviallySwappableSse2(
            firstPointer1, lastPointer1, firstPointer2);

    return SwapRangesTriviallySwappableScalar(
        firstPointer1, lastPointer1, firstPointer2);
}

__BASE_NAMESPACE_END
