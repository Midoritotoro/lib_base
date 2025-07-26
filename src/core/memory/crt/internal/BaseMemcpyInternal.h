#pragma once 

#include <base/core/arch/CpuFeature.h>


__BASE_MEMORY_NAMESPACE_BEGIN

#ifndef __BASE_DEFINE_MEMCPY
#define __BASE_DEFINE_MEMCPY(bytesCount, copyType, copyCommand)                     \
    template <>                                                                     \
    static DECLARE_NOALIAS NODISCARD inline void* Memcpy<bytesCount>(               \
        void*               destination,                                            \
        const void* const   source,                                                 \
        sizetype            size) noexcept                                          \
        {                                                                           \
            copyType* dest          = reinterpret_cast<copyType*>(destination);     \
            const copyType* src     = reinterpret_cast<const copyType*>(source);    \
            for (sizetype current = 0; current < size; ++current) { copyCommand; }  \
            return destination;                                                     \
        }
#endif // __BASE_DEFINE_MEMCPY

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
            byteCount == 1 || byteCount == 2 || byteCount == 4
#if defined(PROCESSOR_X86_64)
            || byteCount == 8
#endif
            , "base::memory::BaseMemcpyImplementationInternal::Memcmp: Unsupported byteCount. ");
    }

    __BASE_DEFINE_MEMCPY(1, char, BASE_ECHO(dest[current] == src[current]; ++dest; ++src));
    __BASE_DEFINE_MEMCPY(2, char, BASE_ECHO(dest[current] == src[current]; ++dest; ++src));
    __BASE_DEFINE_MEMCPY(4, char, BASE_ECHO(dest[current] == src[current]; ++dest; ++src));

#if defined(PROCESSOR_X86_64)
    __BASE_DEFINE_MEMCPY(8, char, BASE_ECHO(dest[current] == src[current]; ++dest; ++src));
#endif
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

    __BASE_DEFINE_MEMCPY(16,    __m128i, BASE_ECHO(                                         _mm_storeu_si128(dest++, _mm_loadu_si128(src++))))
    __BASE_DEFINE_MEMCPY(32,    __m128i, BASE_ECHO(__BASE_REPEAT_N(2,                       _mm_storeu_si128(dest++, _mm_loadu_si128(src++)))))
    __BASE_DEFINE_MEMCPY(64,    __m128i, BASE_ECHO(__BASE_REPEAT_N(4,                       _mm_storeu_si128(dest++, _mm_loadu_si128(src++)))))
    __BASE_DEFINE_MEMCPY(128,   __m128i, BASE_ECHO(__BASE_REPEAT_N(8,                       _mm_storeu_si128(dest++, _mm_loadu_si128(src++)))))
    __BASE_DEFINE_MEMCPY(256,   __m128i, BASE_ECHO(__BASE_REPEAT_N(16,                      _mm_storeu_si128(dest++, _mm_loadu_si128(src++)))))
    __BASE_DEFINE_MEMCPY(512,   __m128i, BASE_ECHO(__BASE_REPEAT_N(32,                      _mm_storeu_si128(dest++, _mm_loadu_si128(src++)))))
    __BASE_DEFINE_MEMCPY(1024,  __m128i, BASE_ECHO(__BASE_REPEAT_N(64,                      _mm_storeu_si128(dest++, _mm_loadu_si128(src++)))))
    __BASE_DEFINE_MEMCPY(2048,  __m128i, BASE_ECHO(__BASE_REPEAT_N(2, __BASE_REPEAT_N(64,   _mm_storeu_si128(dest++, _mm_loadu_si128(src++))))))
    __BASE_DEFINE_MEMCPY(4096,  __m128i, BASE_ECHO(__BASE_REPEAT_N(4, __BASE_REPEAT_N(64,   _mm_storeu_si128(dest++, _mm_loadu_si128(src++))))))
    __BASE_DEFINE_MEMCPY(8192,  __m128i, BASE_ECHO(__BASE_REPEAT_N(8, __BASE_REPEAT_N(64,   _mm_storeu_si128(dest++, _mm_loadu_si128(src++))))))
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

    __BASE_DEFINE_MEMCPY(32,    __m256i, BASE_ECHO(                                         _mm256_storeu_si256(dest++, _mm256_lddqu_si256(src++))))
    __BASE_DEFINE_MEMCPY(64,    __m256i, BASE_ECHO(__BASE_REPEAT_N(2,                       _mm256_storeu_si256(dest++, _mm256_lddqu_si256(src++)))))
    __BASE_DEFINE_MEMCPY(128,   __m256i, BASE_ECHO(__BASE_REPEAT_N(4,                       _mm256_storeu_si256(dest++, _mm256_lddqu_si256(src++)))))
    __BASE_DEFINE_MEMCPY(256,   __m256i, BASE_ECHO(__BASE_REPEAT_N(8,                       _mm256_storeu_si256(dest++, _mm256_lddqu_si256(src++)))))
    __BASE_DEFINE_MEMCPY(512,   __m256i, BASE_ECHO(__BASE_REPEAT_N(16,                      _mm256_storeu_si256(dest++, _mm256_lddqu_si256(src++)))))
    __BASE_DEFINE_MEMCPY(1024,  __m256i, BASE_ECHO(__BASE_REPEAT_N(32,                      _mm256_storeu_si256(dest++, _mm256_lddqu_si256(src++)))))
    __BASE_DEFINE_MEMCPY(2048,  __m256i, BASE_ECHO(__BASE_REPEAT_N(64,                      _mm256_storeu_si256(dest++, _mm256_lddqu_si256(src++)))))
    __BASE_DEFINE_MEMCPY(4096,  __m256i, BASE_ECHO(__BASE_REPEAT_N(2, __BASE_REPEAT_N(64,   _mm256_storeu_si256(dest++, _mm256_lddqu_si256(src++))))))
    __BASE_DEFINE_MEMCPY(8192,  __m256i, BASE_ECHO(__BASE_REPEAT_N(4, __BASE_REPEAT_N(64,   _mm256_storeu_si256(dest++, _mm256_lddqu_si256(src++))))))
    __BASE_DEFINE_MEMCPY(16384, __m256i, BASE_ECHO(__BASE_REPEAT_N(8, __BASE_REPEAT_N(64,   _mm256_storeu_si256(dest++, _mm256_lddqu_si256(src++))))))
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

    __BASE_DEFINE_MEMCPY(64, __m512i, BASE_ECHO(_mm512_storeu_si512(dest++, _mm512_loadu_si512(src++))))
    __BASE_DEFINE_MEMCPY(128, __m512i, BASE_ECHO(__BASE_REPEAT_N(2, _mm512_storeu_si512(dest++, _mm512_loadu_si512(src++)))))
    __BASE_DEFINE_MEMCPY(256, __m512i, BASE_ECHO(__BASE_REPEAT_N(4, _mm512_storeu_si512(dest++, _mm512_loadu_si512(src++)))))
    __BASE_DEFINE_MEMCPY(512, __m512i, BASE_ECHO(__BASE_REPEAT_N(8, _mm512_storeu_si512(dest++, _mm512_loadu_si512(src++)))))
    __BASE_DEFINE_MEMCPY(1024, __m512i, BASE_ECHO(__BASE_REPEAT_N(16, _mm512_storeu_si512(dest++, _mm512_loadu_si512(src++)))))
    __BASE_DEFINE_MEMCPY(2048, __m512i, BASE_ECHO(__BASE_REPEAT_N(32, _mm512_storeu_si512(dest++, _mm512_loadu_si512(src++)))))
    __BASE_DEFINE_MEMCPY(4096, __m512i, BASE_ECHO(__BASE_REPEAT_N(64, _mm512_storeu_si512(dest++, _mm512_loadu_si512(src++)))))
    __BASE_DEFINE_MEMCPY(8192, __m512i, BASE_ECHO(__BASE_REPEAT_N(2, __BASE_REPEAT_N(64, _mm512_storeu_si512(dest++, _mm512_loadu_si512(src++))))))
    __BASE_DEFINE_MEMCPY(16384, __m512i, BASE_ECHO(__BASE_REPEAT_N(4, __BASE_REPEAT_N(64, _mm512_storeu_si512(dest++, _mm512_loadu_si512(src++))))))
    __BASE_DEFINE_MEMCPY(32768, __m512i, BASE_ECHO(__BASE_REPEAT_N(8, __BASE_REPEAT_N(64, _mm512_storeu_si512(dest++, _mm512_loadu_si512(src++))))))
};


__BASE_MEMORY_NAMESPACE_END
