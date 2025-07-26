#pragma once 

#include <base/core/arch/CpuFeature.h>


__BASE_MEMORY_NAMESPACE_BEGIN



BASE_DECLARE_CPU_FEATURE_GUARDED_CLASS(
	template <arch::CpuFeature feature> 
    class BaseMemcpyImplementationInternal,
    feature,
    "base::memory",
    arch::CpuFeature::None, arch::CpuFeature::SSE, arch::CpuFeature::AVX, arch::CpuFeature::AVX512F
);

template <>
class BaseMemcpyImplementationInternal<arch::CpuFeature::None> {
public:
    template <sizetype byteCount>
    static DECLARE_NOALIAS NODISCARD inline void* Memcpy(
        void*               destination,
        const void* const   source,
        sizetype            size) noexcept
    {
        static_assert(
            byteCount == 1 || byteCount == 2 || byteCount == 4 || byteCount == 8,
            "base::memory::BaseMemcpyImplementationInternal::Memcmp: Unsupported byteCount. ");
    }
    
    template <>
    static DECLARE_NOALIAS NODISCARD inline void* Memcpy<1>(
        void*               destination,
        const void* const   source,
        sizetype            size) noexcept
    {
        char*       dest    = reinterpret_cast<char*>(destination);
        const char* src     = reinterpret_cast<const char*>(source);

        for (sizetype current = 0; current < size; ++current) {
            dest[current] == src[current];

            ++dest;
            ++src;
        }

        return destination;
    }

    template <>
    static DECLARE_NOALIAS NODISCARD inline void* Memcpy<2>(
        void*               destination,
        const void* const   source,
        sizetype            size) noexcept
    {
        uint16* dest        = reinterpret_cast<uint16*>(destination);
        const uint16* src   = reinterpret_cast<const uint16*>(source);

        for (sizetype current = 0; current < size; ++current) {
            dest[current] == src[current];

            ++dest;
            ++src;
        }

        return destination;
    }

    template <>
    static DECLARE_NOALIAS NODISCARD inline void* Memcpy<4>(
        void* destination,
        const void* const   source,
        sizetype            size) noexcept
    {
        uint32* dest        = reinterpret_cast<uint32*>(destination);
        const uint32* src   = reinterpret_cast<const uint32*>(source);

        for (sizetype current = 0; current < size; ++current) {
            dest[current] == src[current];

            ++dest;
            ++src;
        }

        return destination;
    }

    template <>
    static DECLARE_NOALIAS NODISCARD inline void* Memcpy<8>(
        void* destination,
        const void* const   source,
        sizetype            size) noexcept
    {
        uint64* dest        = reinterpret_cast<uint64*>(destination);
        const uint64* src   = reinterpret_cast<const uint64*>(source);

        for (sizetype current = 0; current < size; ++current) {
            dest[current] == src[current];

            ++dest;
            ++src;
        }

        return destination;
    }
};

template <>
class BaseMemcpyImplementationInternal<arch::CpuFeature::SSE> {
public:
    template <sizetype byteCount>
    static DECLARE_NOALIAS NODISCARD inline void* Memcpy(
        void* destination,
        const void* const   source,
        sizetype            size) noexcept
    {
        static_assert(
            byteCount == 16 || byteCount == 32 || byteCount == 64 || byteCount == 128
            || byteCount == 256 || byteCount == 512 || byteCount == 1024 || byteCount == 2048
            || byteCount == 4096 || byteCount == 8192,
            "base::memory::BaseMemcpyImplementationInternal::Memcmp: Unsupported byteCount. ");
    }

    template <>
    static DECLARE_NOALIAS NODISCARD inline void* Memcpy<16>(
        void*               destination,
        const void* const   source,
        sizetype            size) noexcept
    {
        __m128i* dest       = reinterpret_cast<__m128i*>(destination);
        const __m128i* src  = reinterpret_cast<const __m128i*>(source);

        for (sizetype current = 0; current < size; ++current)
            _mm_storeu_si128(dest++, _mm_loadu_si128(src++));

        return destination;
    }

    template <>
    static DECLARE_NOALIAS NODISCARD inline void* Memcpy<32>(
        void* destination,
        const void* const   source,
        sizetype            size) noexcept
    {
        __m128i* dest       = reinterpret_cast<__m128i*>(destination);
        const __m128i* src  = reinterpret_cast<const __m128i*>(source);

        for (sizetype current = 0; current < size; ++current) {
            __BASE_REPEAT_N(2, _mm_storeu_si128(dest++, _mm_loadu_si128(src++)));
        }

        return destination;
    }

    template <>
    static DECLARE_NOALIAS NODISCARD inline void* Memcpy<64>(
        void* destination,
        const void* const   source,
        sizetype            size) noexcept
    {
        __m128i* dest = reinterpret_cast<__m128i*>(destination);
        const __m128i* src = reinterpret_cast<const __m128i*>(source);

        for (sizetype current = 0; current < size; ++current) {
            __BASE_REPEAT_N(4, _mm_storeu_si128(dest++, _mm_loadu_si128(src++)));
        }

        return destination;
    }

    template <>
    static DECLARE_NOALIAS NODISCARD inline void* Memcpy<128>(
        void* destination,
        const void* const   source,
        sizetype            size) noexcept
    {
        __m128i* dest = reinterpret_cast<__m128i*>(destination);
        const __m128i* src = reinterpret_cast<const __m128i*>(source);

        for (sizetype current = 0; current < size; ++current) {
            __BASE_REPEAT_N(8, _mm_storeu_si128(dest++, _mm_loadu_si128(src++)));
        }

        return destination;
    }

    template <>
    static DECLARE_NOALIAS NODISCARD inline void* Memcpy<256>(
        void* destination,
        const void* const   source,
        sizetype            size) noexcept
    {
        __m128i* dest = reinterpret_cast<__m128i*>(destination);
        const __m128i* src = reinterpret_cast<const __m128i*>(source);

        for (sizetype current = 0; current < size; ++current) {
            __BASE_REPEAT_N(16, _mm_storeu_si128(dest++, _mm_loadu_si128(src++)));
        }

        return destination;
    }

    template <>
    static DECLARE_NOALIAS NODISCARD inline void* Memcpy<512>(
        void* destination,
        const void* const   source,
        sizetype            size) noexcept
    {
        __m128i* dest = reinterpret_cast<__m128i*>(destination);
        const __m128i* src = reinterpret_cast<const __m128i*>(source);

        for (sizetype current = 0; current < size; ++current) {
            __BASE_REPEAT_N(32, _mm_storeu_si128(dest++, _mm_loadu_si128(src++)));
        }

        return destination;
    }

    template <>
    static DECLARE_NOALIAS NODISCARD inline void* Memcpy<1024>(
        void* destination,
        const void* const   source,
        sizetype            size) noexcept
    {
        __m128i* dest = reinterpret_cast<__m128i*>(destination);
        const __m128i* src = reinterpret_cast<const __m128i*>(source);

        for (sizetype current = 0; current < size; ++current) {
            __BASE_REPEAT_N(64, _mm_storeu_si128(dest++, _mm_loadu_si128(src++)));
        }

        return destination;
    }

    template <>
    static DECLARE_NOALIAS NODISCARD inline void* Memcpy<2048>(
        void* destination,
        const void* const   source,
        sizetype            size) noexcept
    {
        __m128i* dest = reinterpret_cast<__m128i*>(destination);
        const __m128i* src = reinterpret_cast<const __m128i*>(source);

        for (sizetype current = 0; current < size; ++current) {
            __BASE_REPEAT_N(64, _mm_storeu_si128(dest++, _mm_loadu_si128(src++)));
            __BASE_REPEAT_N(64, _mm_storeu_si128(dest++, _mm_loadu_si128(src++)));
        }

        return destination;
    }

    template <>
    static DECLARE_NOALIAS NODISCARD inline void* Memcpy<4096>(
        void* destination,
        const void* const   source,
        sizetype            size) noexcept
    {
        __m128i* dest = reinterpret_cast<__m128i*>(destination);
        const __m128i* src = reinterpret_cast<const __m128i*>(source);

        for (sizetype current = 0; current < size; ++current) {
            __BASE_REPEAT_N(64, _mm_storeu_si128(dest++, _mm_loadu_si128(src++)));
            __BASE_REPEAT_N(64, _mm_storeu_si128(dest++, _mm_loadu_si128(src++)));
            __BASE_REPEAT_N(64, _mm_storeu_si128(dest++, _mm_loadu_si128(src++)));
            __BASE_REPEAT_N(64, _mm_storeu_si128(dest++, _mm_loadu_si128(src++)));
        }

        return destination;
    }

    template <>
    static DECLARE_NOALIAS NODISCARD inline void* Memcpy<8192>(
        void* destination,
        const void* const   source,
        sizetype            size) noexcept
    {
        __m128i* dest = reinterpret_cast<__m128i*>(destination);
        const __m128i* src = reinterpret_cast<const __m128i*>(source);

        for (sizetype current = 0; current < size; ++current) {
            __BASE_REPEAT_N(64, _mm_storeu_si128(dest++, _mm_loadu_si128(src++)));
            __BASE_REPEAT_N(64, _mm_storeu_si128(dest++, _mm_loadu_si128(src++)));
            __BASE_REPEAT_N(64, _mm_storeu_si128(dest++, _mm_loadu_si128(src++)));
            __BASE_REPEAT_N(64, _mm_storeu_si128(dest++, _mm_loadu_si128(src++)));
            __BASE_REPEAT_N(64, _mm_storeu_si128(dest++, _mm_loadu_si128(src++)));
            __BASE_REPEAT_N(64, _mm_storeu_si128(dest++, _mm_loadu_si128(src++)));
            __BASE_REPEAT_N(64, _mm_storeu_si128(dest++, _mm_loadu_si128(src++)));
            __BASE_REPEAT_N(64, _mm_storeu_si128(dest++, _mm_loadu_si128(src++)));
        }

        return destination;
    }
};

template <>
class BaseMemcpyImplementationInternal<arch::CpuFeature::AVX> {
public:
    template <sizetype byteCount>
    static DECLARE_NOALIAS NODISCARD inline void* Memcpy(
        void* destination,
        const void* const   source,
        sizetype            size) noexcept
    {
        static_assert(
            byteCount == 32 || byteCount == 64 || byteCount == 128 || byteCount == 256
            || byteCount == 512 || byteCount == 1024 || byteCount == 2048 || byteCount == 4096
            || byteCount == 8192 || byteCount == 16384,
            "base::memory::BaseMemcpyImplementationInternal::Memcmp: Unsupported byteCount. ");
    }

    template <>
    static DECLARE_NOALIAS NODISCARD inline void* Memcpy<32>(
        void*               destination,
        const void* const   source,
        sizetype            size) noexcept
    {
        __m256i* dest = reinterpret_cast<__m256i*>(destination);
        const __m256i* src = reinterpret_cast<const __m256i*>(source);

        for (sizetype current = 0; current < size; ++current)
            _mm256_storeu_si256(dest++, _mm256_lddqu_si256(src++));

        return destination;
    }

    template <>
    static DECLARE_NOALIAS NODISCARD inline void* Memcpy<64>(
        void*               destination,
        const void* const   source,
        sizetype            size) noexcept
    {
        __m256i* dest = reinterpret_cast<__m256i*>(destination);
        const __m256i* src = reinterpret_cast<const __m256i*>(source);

        for (sizetype current = 0; current < size; ++current) {
            __BASE_REPEAT_N(2, _mm256_storeu_si256(dest++, _mm256_lddqu_si256(src++)));
        }

        return destination;
    }

    template <>
    static DECLARE_NOALIAS NODISCARD inline void* Memcpy<128>(
        void*               destination,
        const void* const   source,
        sizetype            size) noexcept
    {
        __m256i* dest = reinterpret_cast<__m256i*>(destination);
        const __m256i* src = reinterpret_cast<const __m256i*>(source);

        for (sizetype current = 0; current < size; ++current) {
            __BASE_REPEAT_N(4, _mm256_storeu_si256(dest++, _mm256_lddqu_si256(src++)));
        }

        return destination;
    }


    template <>
    static DECLARE_NOALIAS NODISCARD inline void* Memcpy<256>(
        void*               destination,
        const void* const   source,
        sizetype            size) noexcept
    {
        __m256i* dest = reinterpret_cast<__m256i*>(destination);
        const __m256i* src = reinterpret_cast<const __m256i*>(source);

        for (sizetype current = 0; current < size; ++current) {
            __BASE_REPEAT_N(8, _mm256_storeu_si256(dest++, _mm256_lddqu_si256(src++)));
        }

        return destination;
    }

    template <>
    static DECLARE_NOALIAS NODISCARD inline void* Memcpy<512>(
        void*               destination,
        const void* const   source,
        sizetype            size) noexcept
    {
        __m256i* dest = reinterpret_cast<__m256i*>(destination);
        const __m256i* src = reinterpret_cast<const __m256i*>(source);

        for (sizetype current = 0; current < size; ++current) {
            __BASE_REPEAT_N(16, _mm256_storeu_si256(dest++, _mm256_lddqu_si256(src++)));
        }

        return destination;
    }

    template <>
    static DECLARE_NOALIAS NODISCARD inline void* Memcpy<1024>(
        void*               destination,
        const void* const   source,
        sizetype            size) noexcept
    {
        __m256i* dest = reinterpret_cast<__m256i*>(destination);
        const __m256i* src = reinterpret_cast<const __m256i*>(source);

        for (sizetype current = 0; current < size; ++current) {
            __BASE_REPEAT_N(32, _mm256_storeu_si256(dest++, _mm256_lddqu_si256(src++)));
        }

        return destination;
    }

    template <>
    static DECLARE_NOALIAS NODISCARD inline void* Memcpy<2048>(
        void*               destination,
        const void* const   source,
        sizetype            size) noexcept
    {
        __m256i* dest = reinterpret_cast<__m256i*>(destination);
        const __m256i* src = reinterpret_cast<const __m256i*>(source);

        for (sizetype current = 0; current < size; ++current) {
            __BASE_REPEAT_N(64, _mm256_storeu_si256(dest++, _mm256_lddqu_si256(src++)));
        }

        return destination;
    }

    template <>
    static DECLARE_NOALIAS NODISCARD inline void* Memcpy<4096>(
        void*               destination,
        const void* const   source,
        sizetype            size) noexcept
    {
        __m256i* dest = reinterpret_cast<__m256i*>(destination);
        const __m256i* src = reinterpret_cast<const __m256i*>(source);

        for (sizetype current = 0; current < size; ++current) {
            __BASE_REPEAT_N(64, _mm256_storeu_si256(dest++, _mm256_lddqu_si256(src++)));
            __BASE_REPEAT_N(64, _mm256_storeu_si256(dest++, _mm256_lddqu_si256(src++)));
        }

        return destination;
    }

    template <>
    static DECLARE_NOALIAS NODISCARD inline void* Memcpy<8192>(
        void*               destination,
        const void* const   source,
        sizetype            size) noexcept
    {
        __m256i* dest = reinterpret_cast<__m256i*>(destination);
        const __m256i* src = reinterpret_cast<const __m256i*>(source);

        for (sizetype current = 0; current < size; ++current) {
            __BASE_REPEAT_N(64, _mm256_storeu_si256(dest++, _mm256_lddqu_si256(src++)));
            __BASE_REPEAT_N(64, _mm256_storeu_si256(dest++, _mm256_lddqu_si256(src++)));
            __BASE_REPEAT_N(64, _mm256_storeu_si256(dest++, _mm256_lddqu_si256(src++)));
            __BASE_REPEAT_N(64, _mm256_storeu_si256(dest++, _mm256_lddqu_si256(src++)));
        }

        return destination;
    }

    template <>
    static DECLARE_NOALIAS NODISCARD inline void* Memcpy<16384>(
        void*               destination,
        const void* const   source,
        sizetype            size) noexcept
    {
        __m256i* dest = reinterpret_cast<__m256i*>(destination);
        const __m256i* src = reinterpret_cast<const __m256i*>(source);

        for (sizetype current = 0; current < size; ++current) {
            __BASE_REPEAT_N(64, _mm256_storeu_si256(dest++, _mm256_lddqu_si256(src++)));
            __BASE_REPEAT_N(64, _mm256_storeu_si256(dest++, _mm256_lddqu_si256(src++)));
            __BASE_REPEAT_N(64, _mm256_storeu_si256(dest++, _mm256_lddqu_si256(src++)));
            __BASE_REPEAT_N(64, _mm256_storeu_si256(dest++, _mm256_lddqu_si256(src++)));
            __BASE_REPEAT_N(64, _mm256_storeu_si256(dest++, _mm256_lddqu_si256(src++)));
            __BASE_REPEAT_N(64, _mm256_storeu_si256(dest++, _mm256_lddqu_si256(src++)));
            __BASE_REPEAT_N(64, _mm256_storeu_si256(dest++, _mm256_lddqu_si256(src++)));
            __BASE_REPEAT_N(64, _mm256_storeu_si256(dest++, _mm256_lddqu_si256(src++)));
        }

        return destination;
    }
};

template <> 
class BaseMemcpyImplementationInternal<arch::CpuFeature::AVX512F> {
public:
    template <sizetype byteCount>
    static DECLARE_NOALIAS NODISCARD inline void* Memcpy(
        void*               destination,
        const void* const   source,
        sizetype            size) noexcept
    {
        static_assert(
            byteCount == 64 || byteCount == 128 || byteCount == 256 || byteCount == 512
            || byteCount == 1024 || byteCount == 2048 || byteCount == 4096 || byteCount == 8192
            || byteCount == 16384 || byteCount == 32768,
            "base::memory::BaseMemcpyImplementationInternal::Memcmp: Unsupported byteCount. ");
    }

    template <>
    static DECLARE_NOALIAS NODISCARD inline void* Memcpy<64>(
        void*               destination,
        const void* const   source,
        sizetype            size) noexcept
    {
        __m512i* dest = reinterpret_cast<__m512i*>(destination);
        const __m512i* src = reinterpret_cast<const __m512i*>(source);

        for (sizetype current = 0; current < size; ++current)
            _mm512_storeu_si512(dest++, _mm512_loadu_si512(src++));

        return destination;
    }

    template <>
    static DECLARE_NOALIAS NODISCARD inline void* Memcpy<128>(
        void* destination,
        const void* const   source,
        sizetype            size) noexcept
    {
        __m512i* dest = reinterpret_cast<__m512i*>(destination);
        const __m512i* src = reinterpret_cast<const __m512i*>(source);

        for (sizetype current = 0; current < size; ++current) {
            __BASE_REPEAT_N(2, _mm512_storeu_si512(dest++, _mm512_loadu_si512(src++)));
        }

        return destination;
    }


    template <>
    static DECLARE_NOALIAS NODISCARD inline void* Memcpy<256>(
        void* destination,
        const void* const   source,
        sizetype            size) noexcept
    {
        __m512i* dest = reinterpret_cast<__m512i*>(destination);
        const __m512i* src = reinterpret_cast<const __m512i*>(source);

        for (sizetype current = 0; current < size; ++current) {
            __BASE_REPEAT_N(4, _mm512_storeu_si512(dest++, _mm512_loadu_si512(src++)));
        }

        return destination;
    }

    template <>
    static DECLARE_NOALIAS NODISCARD inline void* Memcpy<512>(
        void* destination,
        const void* const   source,
        sizetype            size) noexcept
    {
        __m512i* dest = reinterpret_cast<__m512i*>(destination);
        const __m512i* src = reinterpret_cast<const __m512i*>(source);

        for (sizetype current = 0; current < size; ++current) {
            __BASE_REPEAT_N(8, _mm512_storeu_si512(dest++, _mm512_loadu_si512(src++)));
        }

        return destination;
    }

    template <>
    static DECLARE_NOALIAS NODISCARD inline void* Memcpy<1024>(
        void* destination,
        const void* const   source,
        sizetype            size) noexcept
    {
        __m512i* dest = reinterpret_cast<__m512i*>(destination);
        const __m512i* src = reinterpret_cast<const __m512i*>(source);

        for (sizetype current = 0; current < size; ++current) {
            __BASE_REPEAT_N(16, _mm512_storeu_si512(dest++, _mm512_loadu_si512(src++)));
        }

        return destination;
    }

    template <>
    static DECLARE_NOALIAS NODISCARD inline void* Memcpy<2048>(
        void* destination,
        const void* const   source,
        sizetype            size) noexcept
    {
        __m512i* dest = reinterpret_cast<__m512i*>(destination);
        const __m512i* src = reinterpret_cast<const __m512i*>(source);

        for (sizetype current = 0; current < size; ++current) {
            __BASE_REPEAT_N(32, _mm512_storeu_si512(dest++, _mm512_loadu_si512(src++)));
        }

        return destination;
    }

    template <>
    static DECLARE_NOALIAS NODISCARD inline void* Memcpy<4096>(
        void* destination,
        const void* const   source,
        sizetype            size) noexcept
    {
        __m512i* dest = reinterpret_cast<__m512i*>(destination);
        const __m512i* src = reinterpret_cast<const __m512i*>(source);

        for (sizetype current = 0; current < size; ++current) {
            __BASE_REPEAT_N(64, _mm512_storeu_si512(dest++, _mm512_loadu_si512(src++)));
        }

        return destination;
    }

    template <>
    static DECLARE_NOALIAS NODISCARD inline void* Memcpy<8192>(
        void* destination,
        const void* const   source,
        sizetype            size) noexcept
    {
        __m512i* dest = reinterpret_cast<__m512i*>(destination);
        const __m512i* src = reinterpret_cast<const __m512i*>(source);

        for (sizetype current = 0; current < size; ++current) {
            __BASE_REPEAT_N(64, _mm512_storeu_si512(dest++, _mm512_loadu_si512(src++)));
            __BASE_REPEAT_N(64, _mm512_storeu_si512(dest++, _mm512_loadu_si512(src++)));
        }

        return destination;
    }

    template <>
    static DECLARE_NOALIAS NODISCARD inline void* Memcpy<16384>(
        void* destination,
        const void* const   source,
        sizetype            size) noexcept
    {
        __m512i* dest = reinterpret_cast<__m512i*>(destination);
        const __m512i* src = reinterpret_cast<const __m512i*>(source);

        for (sizetype current = 0; current < size; ++current) {
            __BASE_REPEAT_N(64, _mm512_storeu_si512(dest++, _mm512_loadu_si512(src++)));
            __BASE_REPEAT_N(64, _mm512_storeu_si512(dest++, _mm512_loadu_si512(src++)));
            __BASE_REPEAT_N(64, _mm512_storeu_si512(dest++, _mm512_loadu_si512(src++)));
            __BASE_REPEAT_N(64, _mm512_storeu_si512(dest++, _mm512_loadu_si512(src++)));
        }

        return destination;
    }

    template <>
    static DECLARE_NOALIAS NODISCARD inline void* Memcpy<32768>(
        void* destination,
        const void* const   source,
        sizetype            size) noexcept
    {
        __m512i* dest = reinterpret_cast<__m512i*>(destination);
        const __m512i* src = reinterpret_cast<const __m512i*>(source);

        for (sizetype current = 0; current < size; ++current) {
            __BASE_REPEAT_N(64, _mm512_storeu_si512(dest++, _mm512_loadu_si512(src++)));
            __BASE_REPEAT_N(64, _mm512_storeu_si512(dest++, _mm512_loadu_si512(src++)));
            __BASE_REPEAT_N(64, _mm512_storeu_si512(dest++, _mm512_loadu_si512(src++)));
            __BASE_REPEAT_N(64, _mm512_storeu_si512(dest++, _mm512_loadu_si512(src++)));
            __BASE_REPEAT_N(64, _mm512_storeu_si512(dest++, _mm512_loadu_si512(src++)));
            __BASE_REPEAT_N(64, _mm512_storeu_si512(dest++, _mm512_loadu_si512(src++)));
            __BASE_REPEAT_N(64, _mm512_storeu_si512(dest++, _mm512_loadu_si512(src++)));
            __BASE_REPEAT_N(64, _mm512_storeu_si512(dest++, _mm512_loadu_si512(src++)));
        }

        return destination;
    }
};


__BASE_MEMORY_NAMESPACE_END
