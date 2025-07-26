#pragma once 

#include <base/core/arch/CpuFeature.h>


__BASE_MEMORY_NAMESPACE_BEGIN

#ifndef __BASE_DEFINE_DEFAULT_MEMCPY
#define __BASE_DEFINE_DEFAULT_MEMCPY(...)                           \
    template <sizetype byteCount, bool aligned>                     \
    static DECLARE_NOALIAS NODISCARD inline void* Memcpy(           \
        void*               destination,                            \
        const void* const   source,                                 \
        sizetype            size) noexcept                          \
    {                                                               \
        static_assert(                                              \
            base::arch::Contains<byteCount, __VA_ARGS__>::value,    \
            "base::memory::Memcpy: Unsupported byteCount. "         \
    }   
#endif // __BASE_DEFINE_DEFAULT_MEMCPY


#ifndef __BASE_DEFINE_MEMCPY
#define __BASE_DEFINE_MEMCPY(bytesCount, aligned, copyType, copyCommand)            \
    template <>                                                                     \
    static DECLARE_NOALIAS NODISCARD inline void* Memcpy<bytesCount, aligned>(      \
        void*               destination,                                            \
        const void* const   source,                                                 \
        sizetype            size) noexcept                                          \
    {                                                                               \
        copyType* dest          = reinterpret_cast<copyType*>(destination);         \
        const copyType* src     = reinterpret_cast<const copyType*>(source);        \
        for (sizetype current = 0; current < size; ++current) { copyCommand; }      \
        return destination;                                                         \
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

#if defined(PROCESSOR_X86_64)
    __BASE_DEFINE_DEFAULT_MEMCPY(1, 2, 4, 8)
#else 
    __BASE_DEFINE_DEFAULT_MEMCPY(1, 2, 4)
#endif 

    __BASE_DEFINE_MEMCPY(1, false, char, BASE_ECHO(dest[current] == src[current]; ++dest; ++src));
    __BASE_DEFINE_MEMCPY(2, false, char, BASE_ECHO(dest[current] == src[current]; ++dest; ++src));
    __BASE_DEFINE_MEMCPY(4, false, char, BASE_ECHO(dest[current] == src[current]; ++dest; ++src));

#if defined(PROCESSOR_X86_64)
    __BASE_DEFINE_MEMCPY(8, false, char, BASE_ECHO(dest[current] == src[current]; ++dest; ++src));
#endif
};

template <>
class BaseMemcpyImplementationInternal<arch::CpuFeature::SSE> {
public:
    __BASE_DEFINE_DEFAULT_MEMCPY(16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192)

    // Unaligned

    __BASE_DEFINE_MEMCPY(16,    false, __m128i, BASE_ECHO(                                         _mm_storeu_si128(dest++, _mm_loadu_si128(src++))))
    __BASE_DEFINE_MEMCPY(32,    false, __m128i, BASE_ECHO(__BASE_REPEAT_N(2,                       _mm_storeu_si128(dest++, _mm_loadu_si128(src++)))))
    __BASE_DEFINE_MEMCPY(64,    false, __m128i, BASE_ECHO(__BASE_REPEAT_N(4,                       _mm_storeu_si128(dest++, _mm_loadu_si128(src++)))))
    __BASE_DEFINE_MEMCPY(128,   false, __m128i, BASE_ECHO(__BASE_REPEAT_N(8,                       _mm_storeu_si128(dest++, _mm_loadu_si128(src++)))))
    __BASE_DEFINE_MEMCPY(256,   false, __m128i, BASE_ECHO(__BASE_REPEAT_N(16,                      _mm_storeu_si128(dest++, _mm_loadu_si128(src++)))))
    __BASE_DEFINE_MEMCPY(512,   false, __m128i, BASE_ECHO(__BASE_REPEAT_N(32,                      _mm_storeu_si128(dest++, _mm_loadu_si128(src++)))))
    __BASE_DEFINE_MEMCPY(1024,  false, __m128i, BASE_ECHO(__BASE_REPEAT_N(64,                      _mm_storeu_si128(dest++, _mm_loadu_si128(src++)))))
    __BASE_DEFINE_MEMCPY(2048,  false, __m128i, BASE_ECHO(__BASE_REPEAT_N(2, __BASE_REPEAT_N(64,   _mm_storeu_si128(dest++, _mm_loadu_si128(src++))))))
    __BASE_DEFINE_MEMCPY(4096,  false, __m128i, BASE_ECHO(__BASE_REPEAT_N(4, __BASE_REPEAT_N(64,   _mm_storeu_si128(dest++, _mm_loadu_si128(src++))))))
    __BASE_DEFINE_MEMCPY(8192,  false, __m128i, BASE_ECHO(__BASE_REPEAT_N(8, __BASE_REPEAT_N(64,   _mm_storeu_si128(dest++, _mm_loadu_si128(src++))))))

    // Aligned

    __BASE_DEFINE_MEMCPY(16,    true, __m128i, BASE_ECHO(                                         _mm_store_si128(dest++, _mm_load_si128(src++))))
    __BASE_DEFINE_MEMCPY(32,    true, __m128i, BASE_ECHO(__BASE_REPEAT_N(2,                       _mm_store_si128(dest++, _mm_load_si128(src++)))))
    __BASE_DEFINE_MEMCPY(64,    true, __m128i, BASE_ECHO(__BASE_REPEAT_N(4,                       _mm_store_si128(dest++, _mm_load_si128(src++)))))
    __BASE_DEFINE_MEMCPY(128,   true, __m128i, BASE_ECHO(__BASE_REPEAT_N(8,                       _mm_store_si128(dest++, _mm_load_si128(src++)))))
    __BASE_DEFINE_MEMCPY(256,   true, __m128i, BASE_ECHO(__BASE_REPEAT_N(16,                      _mm_store_si128(dest++, _mm_load_si128(src++)))))
    __BASE_DEFINE_MEMCPY(512,   true, __m128i, BASE_ECHO(__BASE_REPEAT_N(32,                      _mm_store_si128(dest++, _mm_load_si128(src++)))))
    __BASE_DEFINE_MEMCPY(1024,  true, __m128i, BASE_ECHO(__BASE_REPEAT_N(64,                      _mm_store_si128(dest++, _mm_load_si128(src++)))))
    __BASE_DEFINE_MEMCPY(2048,  true, __m128i, BASE_ECHO(__BASE_REPEAT_N(2, __BASE_REPEAT_N(64,   _mm_store_si128(dest++, _mm_load_si128(src++))))))
    __BASE_DEFINE_MEMCPY(4096,  true, __m128i, BASE_ECHO(__BASE_REPEAT_N(4, __BASE_REPEAT_N(64,   _mm_store_si128(dest++, _mm_load_si128(src++))))))
    __BASE_DEFINE_MEMCPY(8192,  true, __m128i, BASE_ECHO(__BASE_REPEAT_N(8, __BASE_REPEAT_N(64,   _mm_store_si128(dest++, _mm_load_si128(src++))))))
};

template <>
class BaseMemcpyImplementationInternal<arch::CpuFeature::AVX> {
public:
    __BASE_DEFINE_DEFAULT_MEMCPY(32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384)
       
    // Unaligned

    __BASE_DEFINE_MEMCPY(32,    false, __m256i, BASE_ECHO(                                         _mm256_storeu_si256(dest++, _mm256_lddqu_si256(src++))))
    __BASE_DEFINE_MEMCPY(64,    false, __m256i, BASE_ECHO(__BASE_REPEAT_N(2,                       _mm256_storeu_si256(dest++, _mm256_lddqu_si256(src++)))))
    __BASE_DEFINE_MEMCPY(128,   false, __m256i, BASE_ECHO(__BASE_REPEAT_N(4,                       _mm256_storeu_si256(dest++, _mm256_lddqu_si256(src++)))))
    __BASE_DEFINE_MEMCPY(256,   false, __m256i, BASE_ECHO(__BASE_REPEAT_N(8,                       _mm256_storeu_si256(dest++, _mm256_lddqu_si256(src++)))))
    __BASE_DEFINE_MEMCPY(512,   false, __m256i, BASE_ECHO(__BASE_REPEAT_N(16,                      _mm256_storeu_si256(dest++, _mm256_lddqu_si256(src++)))))
    __BASE_DEFINE_MEMCPY(1024,  false, __m256i, BASE_ECHO(__BASE_REPEAT_N(32,                      _mm256_storeu_si256(dest++, _mm256_lddqu_si256(src++)))))
    __BASE_DEFINE_MEMCPY(2048,  false, __m256i, BASE_ECHO(__BASE_REPEAT_N(64,                      _mm256_storeu_si256(dest++, _mm256_lddqu_si256(src++)))))
    __BASE_DEFINE_MEMCPY(4096,  false, __m256i, BASE_ECHO(__BASE_REPEAT_N(2, __BASE_REPEAT_N(64,   _mm256_storeu_si256(dest++, _mm256_lddqu_si256(src++))))))
    __BASE_DEFINE_MEMCPY(8192,  false, __m256i, BASE_ECHO(__BASE_REPEAT_N(4, __BASE_REPEAT_N(64,   _mm256_storeu_si256(dest++, _mm256_lddqu_si256(src++))))))
    __BASE_DEFINE_MEMCPY(16384, false, __m256i, BASE_ECHO(__BASE_REPEAT_N(8, __BASE_REPEAT_N(64,   _mm256_storeu_si256(dest++, _mm256_lddqu_si256(src++))))))

    // Aligned

    __BASE_DEFINE_MEMCPY(32,    true, __m256i, BASE_ECHO(                                           _mm256_store_si256(dest++, _mm256_load_si256(src++))))
    __BASE_DEFINE_MEMCPY(64,    true, __m256i, BASE_ECHO(__BASE_REPEAT_N(2,                         _mm256_store_si256(dest++, _mm256_load_si256(src++)))))
    __BASE_DEFINE_MEMCPY(128,   true, __m256i, BASE_ECHO(__BASE_REPEAT_N(4,                         _mm256_store_si256(dest++, _mm256_load_si256(src++)))))
    __BASE_DEFINE_MEMCPY(256,   true, __m256i, BASE_ECHO(__BASE_REPEAT_N(8,                         _mm256_store_si256(dest++, _mm256_load_si256(src++)))))
    __BASE_DEFINE_MEMCPY(512,   true, __m256i, BASE_ECHO(__BASE_REPEAT_N(16,                        _mm256_store_si256(dest++, _mm256_load_si256(src++)))))
    __BASE_DEFINE_MEMCPY(1024,  true, __m256i, BASE_ECHO(__BASE_REPEAT_N(32,                        _mm256_store_si256(dest++, _mm256_load_si256(src++)))))
    __BASE_DEFINE_MEMCPY(2048,  true, __m256i, BASE_ECHO(__BASE_REPEAT_N(64,                        _mm256_store_si256(dest++, _mm256_load_si256(src++)))))
    __BASE_DEFINE_MEMCPY(4096,  true, __m256i, BASE_ECHO(__BASE_REPEAT_N(2, __BASE_REPEAT_N(64,     _mm256_store_si256(dest++, _mm256_load_si256(src++))))))
    __BASE_DEFINE_MEMCPY(8192,  true, __m256i, BASE_ECHO(__BASE_REPEAT_N(4, __BASE_REPEAT_N(64,     _mm256_store_si256(dest++, _mm256_load_si256(src++))))))
    __BASE_DEFINE_MEMCPY(16384, true, __m256i, BASE_ECHO(__BASE_REPEAT_N(8, __BASE_REPEAT_N(64,     _mm256_store_si256(dest++, _mm256_load_si256(src++))))))
};

template <> 
class BaseMemcpyImplementationInternal<arch::CpuFeature::AVX512F> {
public:
    __BASE_DEFINE_DEFAULT_MEMCPY(64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768)

    // Unaligned

    __BASE_DEFINE_MEMCPY(64,    false, __m512i, BASE_ECHO(                                         _mm512_storeu_si512(dest++, _mm512_loadu_si512(src++))))
    __BASE_DEFINE_MEMCPY(128,   false, __m512i, BASE_ECHO(__BASE_REPEAT_N(2,                       _mm512_storeu_si512(dest++, _mm512_loadu_si512(src++)))))
    __BASE_DEFINE_MEMCPY(256,   false, __m512i, BASE_ECHO(__BASE_REPEAT_N(4,                       _mm512_storeu_si512(dest++, _mm512_loadu_si512(src++)))))
    __BASE_DEFINE_MEMCPY(512,   false, __m512i, BASE_ECHO(__BASE_REPEAT_N(8,                       _mm512_storeu_si512(dest++, _mm512_loadu_si512(src++)))))
    __BASE_DEFINE_MEMCPY(1024,  false, __m512i, BASE_ECHO(__BASE_REPEAT_N(16,                      _mm512_storeu_si512(dest++, _mm512_loadu_si512(src++)))))
    __BASE_DEFINE_MEMCPY(2048,  false, __m512i, BASE_ECHO(__BASE_REPEAT_N(32,                      _mm512_storeu_si512(dest++, _mm512_loadu_si512(src++)))))
    __BASE_DEFINE_MEMCPY(4096,  false, __m512i, BASE_ECHO(__BASE_REPEAT_N(64,                      _mm512_storeu_si512(dest++, _mm512_loadu_si512(src++)))))
    __BASE_DEFINE_MEMCPY(8192,  false, __m512i, BASE_ECHO(__BASE_REPEAT_N(2, __BASE_REPEAT_N(64,   _mm512_storeu_si512(dest++, _mm512_loadu_si512(src++))))))
    __BASE_DEFINE_MEMCPY(16384, false, __m512i, BASE_ECHO(__BASE_REPEAT_N(4, __BASE_REPEAT_N(64,   _mm512_storeu_si512(dest++, _mm512_loadu_si512(src++))))))
    __BASE_DEFINE_MEMCPY(32768, false, __m512i, BASE_ECHO(__BASE_REPEAT_N(8, __BASE_REPEAT_N(64,   _mm512_storeu_si512(dest++, _mm512_loadu_si512(src++))))))

    // Aligned

    __BASE_DEFINE_MEMCPY(64,    true, __m512i, BASE_ECHO(                                         _mm512_store_si512(dest++, _mm512_load_si512(src++))))
    __BASE_DEFINE_MEMCPY(128,   true, __m512i, BASE_ECHO(__BASE_REPEAT_N(2,                       _mm512_store_si512(dest++, _mm512_load_si512(src++)))))
    __BASE_DEFINE_MEMCPY(256,   true, __m512i, BASE_ECHO(__BASE_REPEAT_N(4,                       _mm512_store_si512(dest++, _mm512_load_si512(src++)))))
    __BASE_DEFINE_MEMCPY(512,   true, __m512i, BASE_ECHO(__BASE_REPEAT_N(8,                       _mm512_store_si512(dest++, _mm512_load_si512(src++)))))
    __BASE_DEFINE_MEMCPY(1024,  true, __m512i, BASE_ECHO(__BASE_REPEAT_N(16,                      _mm512_store_si512(dest++, _mm512_load_si512(src++)))))
    __BASE_DEFINE_MEMCPY(2048,  true, __m512i, BASE_ECHO(__BASE_REPEAT_N(32,                      _mm512_store_si512(dest++, _mm512_load_si512(src++)))))
    __BASE_DEFINE_MEMCPY(4096,  true, __m512i, BASE_ECHO(__BASE_REPEAT_N(64,                      _mm512_store_si512(dest++, _mm512_load_si512(src++)))))
    __BASE_DEFINE_MEMCPY(8192,  true, __m512i, BASE_ECHO(__BASE_REPEAT_N(2, __BASE_REPEAT_N(64,   _mm512_store_si512(dest++, _mm512_load_si512(src++))))))
    __BASE_DEFINE_MEMCPY(16384, true, __m512i, BASE_ECHO(__BASE_REPEAT_N(4, __BASE_REPEAT_N(64,   _mm512_store_si512(dest++, _mm512_load_si512(src++))))))
    __BASE_DEFINE_MEMCPY(32768, true, __m512i, BASE_ECHO(__BASE_REPEAT_N(8, __BASE_REPEAT_N(64,   _mm512_store_si512(dest++, _mm512_load_si512(src++))))))
};


__BASE_MEMORY_NAMESPACE_END
