#include <src/core/utility/simd/SimdAlgorithm.h>

#include <src/core/utility/simd/SimdAlgorithmSafety.h>

#include <src/core/utility/simd/SimdTailMask.h>
#include <base/core/arch/ProcessorFeatures.h>

#include <src/core/memory/MemoryUtility.h>
#include <base/core/utility/Math.h>

__BASE_NAMESPACE_BEGIN

DECLARE_NOALIAS always_inline void __CDECL ReverseTriviallyCopyable8BitSse2(
    void* firstPointer,
    void* lastPointer,
    void* destinationPointer) noexcept
{
    const auto length = memory::ByteLength(firstPointer, lastPointer);

    if (length >= 32) {
        const auto reverseCharSse = _mm_set_epi8(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
        const void* stopAt = firstPointer;

        memory::AdvanceBytes(stopAt, (length >> 1) & ~size_t{ 0xF });
        do {
            memory::AdvanceBytes(lastPointer, -16);

            const auto right = _mm_loadu_si128(static_cast<__m128i*>(lastPointer));
            const auto rightReversed = _mm_shuffle_epi8(right, reverseCharSse);

            _mm_storeu_si128(static_cast<__m128i*>(destinationPointer), rightReversed);

            memory::AdvanceBytes(destinationPointer, 16);
        } while (firstPointer != stopAt);
    }

    if (firstPointer != lastPointer)
        return memory::ReverseCopyTail(
            static_cast<uchar*>(firstPointer),
            static_cast<uchar*>(lastPointer),
            destinationPointer);
}

DECLARE_NOALIAS always_inline void __CDECL ReverseTriviallyCopyable8BitAvx(
    void* firstPointer,
    void* lastPointer,
    void* destinationPointer) noexcept
{
    const auto length = memory::ByteLength(firstPointer, lastPointer);

    if (length >= 64) {
        const __m256i reverseCharLanesAvx = _mm256_set_epi8(
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);

        const void* stopAt = firstPointer;
        memory::AdvanceBytes(stopAt, (length >> 1) & ~size_t{ 0x1F });

        do {
            memory::AdvanceBytes(lastPointer, -32);

            const __m256i right = _mm256_loadu_si256(static_cast<__m256i*>(lastPointer));
            const __m256i rightPerm = _mm256_permute4x64_epi64(right, _MM_SHUFFLE(1, 0, 3, 2));

            const __m256i rightReversed = _mm256_shuffle_epi8(rightPerm, reverseCharLanesAvx);
            _mm256_storeu_si256(static_cast<__m256i*>(destinationPointer), rightReversed);

            memory::AdvanceBytes(destinationPointer, 32);
        } while (firstPointer != stopAt);

        _mm256_zeroupper();
    }

    if (firstPointer != lastPointer)
        return ReverseTriviallyCopyable8BitSse2(firstPointer, lastPointer);
}

DECLARE_NOALIAS always_inline void __CDECL ReverseTriviallyCopyable8BitAvx512(
    void* firstPointer,
    void* lastPointer,
    void* destinationPointer) noexcept
{
    const auto length = memory::ByteLength(firstPointer, lastPointer);

    if (length >= 128) {
        const auto reverseCharLanesAvx512 = _mm512_set_epi8(
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
            17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
            17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31);

        const void* stopAt = firstPointer;
        memory::AdvanceBytes(stopAt, (length >> 1) & ~size_t{ 0x3F });

        do {
            memory::AdvanceBytes(lastPointer, -64);

            const auto left = _mm512_loadu_si512(firstPointer);
            const auto rightReversed = _mm512_shuffle_epi8(right, reverseCharLanesAvx512);

            _mm512_storeu_si512(destinationPointer, rightReversed);
            memory::AdvanceBytes(destinationPointer, 64);
        } while (firstPointer != stopAt);
    }

    if (firstPointer != lastPointer)
        return ReverseTriviallyCopyable8BitAvx(firstPointer, lastPointer);
}

// =======================================================================================


DECLARE_NOALIAS always_inline void __CDECL ReverseTriviallyCopyable16BitSse2(
    void* firstPointer,
    void* lastPointer,
    void* destinationPointer) noexcept
{
    const auto length = memory::ByteLength(firstPointer, lastPointer);

    if (length >= 32) {
        const auto reverseShortSse = _mm_set_epi8(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
        const void* stopAt = firstPointer;

        memory::AdvanceBytes(stopAt, (length >> 1) & ~size_t{ 0xF });
        do {
            memory::AdvanceBytes(lastPointer, -16);

            const auto right = _mm_loadu_si128(static_cast<__m128i*>(lastPointer));
            const auto rightReversed = _mm_shuffle_epi8(right, reverseShortSse);

            _mm_storeu_si128(static_cast<__m128i*>(destinationPointer), rightReversed);
            memory::AdvanceBytes(destinationPointer, 16);
        } while (firstPointer != stopAt);
    }

    if (firstPointer != lastPointer)
        return memory::ReverseCopyTail(
            static_cast<ushort*>(firstPointer),
            static_cast<ushort*>(lastPointer),
            destinationPointer);
}

DECLARE_NOALIAS always_inline void __CDECL ReverseTriviallyCopyable16BitAvx(
    void* firstPointer,
    void* lastPointer,
    void* destinationPointer) noexcept
{
    const auto length = memory::ByteLength(firstPointer, lastPointer);

    if (length >= 64) {
        const __m256i reverseShortLanesAvx = _mm256_set_epi8(
            1, 0, 3, 2, 5, 4, 7, 6, 9, 8, 11, 10, 13, 12, 15, 14,
            1, 0, 3, 2, 5, 4, 7, 6, 9, 8, 11, 10, 13, 12, 15, 14);

        const void* stopAt = firstPointer;
        memory::AdvanceBytes(stopAt, (length >> 1) & ~size_t{ 0x1F });

        do {
            memory::AdvanceBytes(lastPointer, -32);

            const __m256i right = _mm256_loadu_si256(static_cast<__m256i*>(lastPointer));
            const __m256i rightPerm = _mm256_permute4x64_epi64(right, _MM_SHUFFLE(1, 0, 3, 2));

            const __m256i rightReversed = _mm256_shuffle_epi8(rightPerm, reverseShortLanesAvx);
            _mm256_storeu_si256(static_cast<__m256i*>(destinationPointer), rightReversed);

            memory::AdvanceBytes(firstPointer, 32);
        } while (firstPointer != stopAt);

        _mm256_zeroupper();
    }

    if (firstPointer != lastPointer)
        return ReverseTriviallyCopyable16BitSse2(firstPointer, lastPointer);
}

DECLARE_NOALIAS always_inline void __CDECL ReverseTriviallyCopyable16BitAvx512(
    void* firstPointer,
    void* lastPointer,
    void* destinationPointer) noexcept
{
    const auto length = memory::ByteLength(firstPointer, lastPointer);

    if (length >= 128) {
        const auto reverseShortLanesAvx512 = _mm512_set_epi8(
            1, 0, 3, 2, 5, 4, 7, 6, 9, 8, 11, 10, 13, 12, 15, 14, 17, 16, 19, 18, 21, 20, 23, 22, 25, 24, 27, 26, 29, 28, 31, 30,
            1, 0, 3, 2, 5, 4, 7, 6, 9, 8, 11, 10, 13, 12, 15, 14, 17, 16, 19, 18, 21, 20, 23, 22, 25, 24, 27, 26, 29, 28, 31, 30);

        const void* stopAt = firstPointer;
        memory::AdvanceBytes(stopAt, (length >> 1) & ~size_t{ 0x3F });

        do {
            memory::AdvanceBytes(lastPointer, -64);

            const auto right = _mm512_loadu_si512(lastPointer);
            const auto rightPerm = _mm512_permutex_epi64(right, _MM_SHUFFLE(1, 0, 3, 2));

            const auto rightReversed = _mm512_shuffle_epi8(rightPerm, reverseShortLanesAvx512);
            _mm512_storeu_si512(destinationPointer, rightReversed);

            memory::AdvanceBytes(firstPointer, 64);
        } while (firstPointer != stopAt);
    }

    if (firstPointer != lastPointer)
        return ReverseTriviallyCopyable16BitAvx(firstPointer, lastPointer);
}

// ==================================================================

DECLARE_NOALIAS always_inline void __CDECL ReverseTriviallyCopyable32BitSse2(
    void* firstPointer,
    void* lastPointer,
    void* destinationPointer) noexcept
{
    const auto length = memory::ByteLength(firstPointer, lastPointer);

    if (length >= 32) {
        const void* stopAt = firstPointer;
        memory::AdvanceBytes(stopAt, (length >> 1) & ~size_t{ 0xF });

        const auto shuffle = _mm_set_epi16(0, 1, 2, 3, 4, 5, 6, 7);

        do {
            memory::AdvanceBytes(lastPointer, -16);

            const auto right = _mm_loadu_si128(static_cast<__m128i*>(lastPointer));
            const auto rightReversed = _mm_shuffle_epi32(right, _MM_SHUFFLE(0, 1, 2, 3));

            _mm_storeu_si128(static_cast<__m128i*>(destinationPointer), rightReversed);
            memory::AdvanceBytes(firstPointer, 16);
        } while (firstPointer != stopAt);
    }

    if (firstPointer != lastPointer)
        return memory::ReverseCopyTail(
            static_cast<uint32*>(firstPointer),
            static_cast<uint32*>(lastPointer),
            destinationPointer);
}

DECLARE_NOALIAS always_inline void __CDECL ReverseTriviallyCopyable32BitAvx(
    void* firstPointer,
    void* lastPointer,
    void* destinationPointer) noexcept
{
    const auto length = memory::ByteLength(firstPointer, lastPointer);

    if (length >= 64) {
        const void* stopAt = firstPointer;
        memory::AdvanceBytes(stopAt, (length >> 1) & ~size_t{ 0x1F });

        const __m256i shuffle = _mm256_set_epi32(0, 1, 2, 3, 4, 5, 6, 7);

        do {
            memory::AdvanceBytes(lastPointer, -32);


            const auto right = _mm256_loadu_si256(static_cast<__m256i*>(lastPointer));
            const auto rightReversed = _mm256_permutevar8x32_epi32(right, shuffle);

            _mm256_storeu_si256(static_cast<__m256i*>(destinationPointer), rightReversed);
            memory::AdvanceBytes(firstPointer, 32);
        } while (firstPointer != stopAt);

        _mm256_zeroupper();
    }

    if (firstPointer != lastPointer)
        return ReverseTriviallyCopyable32BitSse2(firstPointer, lastPointer);
}

DECLARE_NOALIAS always_inline void __CDECL ReverseTriviallyCopyable32BitAvx512(
    void* firstPointer,
    void* lastPointer,
    void* destinationPointer) noexcept
{
    const auto length = memory::ByteLength(firstPointer, lastPointer);

    if (length >= 128) {
        const void* stopAt = firstPointer;
        memory::AdvanceBytes(stopAt, (length >> 1) & ~size_t{ 0x3F });

        const auto shuffle = _mm512_set_epi32(
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);

        do {
            memory::AdvanceBytes(lastPointer, -64);

            const auto left = _mm512_loadu_si512(static_cast<__m512i*>(firstPointer));
            const auto right = _mm512_loadu_si512(static_cast<__m512i*>(lastPointer));

            const auto leftReversed = _mm512_permutevar_epi32(left, shuffle);
            const auto rightReversed = _mm512_permutevar_epi32(right, shuffle);

            _mm512_storeu_si512(firstPointer, rightReversed);
            _mm512_storeu_si512(lastPointer, leftReversed);

            memory::AdvanceBytes(firstPointer, 64);
        } while (firstPointer != stopAt);
    }

    if (firstPointer != lastPointer)
        return ReverseTriviallyCopyable32BitAvx(firstPointer, lastPointer);
}

// ====================================================================================================

DECLARE_NOALIAS always_inline void __CDECL ReverseTriviallyCopyable64BitSse2(
    void* firstPointer,
    void* lastPointer,
    void* destinationPointer) noexcept
{
    const auto length = memory::ByteLength(firstPointer, lastPointer);

    if (length >= 32) {
        const void* stopAt = firstPointer;
        memory::AdvanceBytes(stopAt, (length >> 1) & ~size_t{ 0xF });

        do {
            memory::AdvanceBytes(lastPointer, -16);

            const auto left = _mm_loadu_si128(static_cast<__m128i*>(firstPointer));
            const auto right = _mm_loadu_si128(static_cast<__m128i*>(lastPointer));

            const auto leftReversed = _mm_shuffle_epi32(left, _MM_SHUFFLE(1, 0, 3, 2));
            const auto rightReversed = _mm_shuffle_epi32(right, _MM_SHUFFLE(1, 0, 3, 2));

            _mm_storeu_si128(static_cast<__m128i*>(firstPointer), rightReversed);
            _mm_storeu_si128(static_cast<__m128i*>(lastPointer), leftReversed);

            memory::AdvanceBytes(firstPointer, 16);
        } while (firstPointer != stopAt);
    }

    if (firstPointer != lastPointer)
        return memory::ReverseTail(
            static_cast<uint64*>(firstPointer),
            static_cast<uint64*>(lastPointer));
}

DECLARE_NOALIAS always_inline void __CDECL ReverseTriviallyCopyable64BitAvx(
    void* firstPointer,
    void* lastPointer,
    void* destinationPointer) noexcept
{
    const auto length = memory::ByteLength(firstPointer, lastPointer);

    if (length >= 64) {
        const void* stopAt = firstPointer;
        memory::AdvanceBytes(stopAt, (length >> 1) & ~size_t{ 0x1F });

        do {
            memory::AdvanceBytes(lastPointer, -32);

            const auto left = _mm256_loadu_si256(static_cast<__m256i*>(firstPointer));
            const auto right = _mm256_loadu_si256(static_cast<__m256i*>(lastPointer));

            const auto leftReversed = _mm256_permute4x64_epi64(left, _MM_SHUFFLE(0, 1, 2, 3));
            const auto rightReversed = _mm256_permute4x64_epi64(right, _MM_SHUFFLE(0, 1, 2, 3));

            _mm256_storeu_si256(static_cast<__m256i*>(firstPointer), rightReversed);
            _mm256_storeu_si256(static_cast<__m256i*>(lastPointer), leftReversed);

            memory::AdvanceBytes(firstPointer, 32);
        } while (firstPointer != stopAt);

        _mm256_zeroupper();
    }

    if (firstPointer != lastPointer)
        return ReverseTriviallyCopyable64BitSse2(firstPointer, lastPointer);
}

DECLARE_NOALIAS always_inline void __CDECL ReverseTriviallyCopyable64BitAvx512(
    void* firstPointer,
    void* lastPointer,
    void* destinationPointer) noexcept
{
    const auto length = memory::ByteLength(firstPointer, lastPointer);

    if (length >= 128) {
        const void* stopAt = firstPointer;
        memory::AdvanceBytes(stopAt, (length >> 1) & ~size_t{ 0x3F });

        do {
            memory::AdvanceBytes(lastPointer, -64);

            const auto left = _mm512_loadu_si512(firstPointer);
            const auto right = _mm512_loadu_si512(lastPointer);

            const auto leftReversed = _mm512_permutex_epi64(left, _MM_SHUFFLE(0, 1, 2, 3));
            const auto rightReversed = _mm512_permutex_epi64(right, _MM_SHUFFLE(0, 1, 2, 3));

            _mm512_storeu_si512(firstPointer, rightReversed);
            _mm512_storeu_si512(lastPointer, leftReversed);

            memory::AdvanceBytes(firstPointer, 64);
        } while (firstPointer != stopAt);
    }

    if (firstPointer != lastPointer)
        return ReverseTriviallyCopyable64BitAvx(firstPointer, lastPointer);
}

__BASE_NAMESPACE_END
