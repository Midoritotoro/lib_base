#pragma once 

#include <base/core/arch/Platform.h>

__BASE_NAMESPACE_BEGIN

class FindTraits8Bit {
public:
    static NODISCARD __m512i SetAvx512(const uint8 value) noexcept;
    static NODISCARD __m256i SetAvx(const uint8 value) noexcept;
    static NODISCARD __m128i SetSse(const uint8 value) noexcept;

    static NODISCARD __mmask64 CompareAvx512(
        const __m512i left,
        const __m512i right) noexcept;
    static NODISCARD __m256i CompareAvx(
        const __m256i left,
        const __m256i right) noexcept;
    static NODISCARD __m128i CompareSse(
        const __m128i left,
        const __m128i right) noexcept;
};

class FindTraits16Bit {
public:
    static NODISCARD __m512i SetAvx512(const uint16 value) noexcept;
    static NODISCARD __m256i SetAvx(const uint16 value) noexcept;
    static NODISCARD __m128i SetSse(const uint16 value) noexcept;

    static NODISCARD __mmask32 CompareAvx512(
        const __m512i left,
        const __m512i right) noexcept;
    static NODISCARD __m256i CompareAvx(
        const __m256i left,
        const __m256i right) noexcept;
    static NODISCARD __m128i CompareSse(
        const __m128i left,
        const __m128i right) noexcept;
};

class FindTraits32Bit {
public:
    static NODISCARD __m512i SetAvx512(const uint32 value) noexcept;
    static NODISCARD __m256i SetAvx(const uint32 value) noexcept;
    static NODISCARD __m128i SetSse(const uint32 value) noexcept;

    static NODISCARD __mmask16 CompareAvx512(
        const __m512i left,
        const __m512i right) noexcept;
    static NODISCARD __m256i CompareAvx(
        const __m256i left,
        const __m256i right) noexcept;
    static NODISCARD __m128i CompareSse(
        const __m128i left,
        const __m128i right) noexcept;
};

class FindTraits64Bit {
public:
    static NODISCARD __m512i SetAvx512(const uint64 value) noexcept;
    static NODISCARD __m256i SetAvx(const uint64 value) noexcept;
    static NODISCARD __m128i SetSse(const uint64 value) noexcept;

    static NODISCARD __mmask8 CompareAvx512(
        const __m512i left,
        const __m512i right) noexcept;
    static NODISCARD __m256i CompareAvx(
        const __m256i left,
        const __m256i right) noexcept;
    static NODISCARD __m128i CompareSse(
        const __m128i left,
        const __m128i right) noexcept;
};


__BASE_NAMESPACE_END