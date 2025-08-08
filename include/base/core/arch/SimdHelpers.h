#pragma once

#include <base/core/compatibility/Compatibility.h>

#include <vector>
#include <base/core/Types.h>

#include <base/core/arch/CpuFeature.h>

#if !defined(_M_ARM64EC)
#include <intrin.h>
#endif

#ifndef lib_base_use_compiler_alignment 
#  define lib_base_use_compiler_alignment
#endif

#define __lib_base_enable_impl(feature) (((1/lib_base_enable_##feature) == 1))
#define lib_base_enable(feature) __lib_base_enable_impl(feature)

#if defined(base_processor_x86) && defined(base_cpp_msvc)

#  if (defined(_M_X64) || _M_IX86_FP >= 2)
#    define __SSE__ 1
#    define __SSE2__ 1
#  endif

#  if (defined(_M_AVX) || defined(__AVX__))
#    define __SSE3__                        1

#    define __SSSE3__                       1
#    define __SSE4_1__                      1

#    define __SSE4_2__                      1
#    define __POPCNT__                      1

#    if !defined(__AVX__)
#      define __AVX__                       1
#    endif

#  endif
#  ifdef __AVX2__
// MSVC defines __AVX2__ with /arch:AVX2
#    define __F16C__                        1
#    define __RDRND__                       1

#    define __FMA__                         1

#    define __BMI__                         1
#    define __BMI2__                        1

#    define __MOVBE__                       1
#    define __LZCNT__                       1
#  endif
# endif


#if defined(base_processor_x86) && defined(__SSE2__)
#  include <emmintrin.h>
#  define LIB_BASE_HAS_SIMD_SUPPORT 1
#  define lib_base_enable_sse2 1
#else
#  define lib_base_enable_sse2 -1
#endif // defined(PROCESSOR_X86) && defined(__SSE2__)


#if defined(base_processor_x86) && defined(__SSE3__)
#  define LIB_BASE_HAS_SIMD_SUPPORT 1
#  define lib_base_enable_sse3 1
#else
#  define lib_base_enable_sse3 -1
#endif // defined(PROCESSOR_X86) && defined(__SSE3__)


#if defined(base_processor_x86) && defined(__SSSE3__)
#  define LIB_BASE_HAS_SIMD_SUPPORT 1
#  define lib_base_enable_ssse3 1
#else
#  define lib_base_enable_ssse3 -1
#endif // defined(PROCESSOR_X86) && defined(__SSSE3__)


#if defined(base_processor_x86) && defined(__SSE4_1__)
#  define LIB_BASE_HAS_SIMD_SUPPORT 1
#  define lib_base_enable_sse4_1 1
#else
#  define lib_base_enable_sse4_1 -1
#endif // defined(PROCESSOR_X86) && defined(__SSE4_1__)


#if defined(base_processor_x86) && defined(__SSE4_2__)
#  define LIB_BASE_HAS_SIMD_SUPPORT 1
#  define lib_base_enable_sse4_2 1
#else
#  define lib_base_enable_sse4_2 -1
#endif // defined(PROCESSOR_X86) && defined(__SSE4_2__)


#if defined(base_processor_x86) && defined(__AVX__)
#  define LIB_BASE_HAS_SIMD_SUPPORT 1
#  define lib_base_enable_avx 1
#else
#  define lib_base_enable_avx -1
#endif // defined(PROCESSOR_X86) && defined(__AVX__)


#if defined(base_processor_x86) && defined(__AVX2__)
#  define LIB_BASE_HAS_SIMD_SUPPORT 1
#  define lib_base_enable_avx2 1
#else
#  define lib_base_enable_avx2 -1
#endif // defined(PROCESSOR_X86) && defined(__AVX2__)

#if !defined(lib_base_has_simd_support)
#  define lib_base_has_simd_support 0
#endif

#if lib_base_has_simd_support

__BASE_ARCH_NAMESPACE_BEGIN

using xmmint    = __m128i;
using ymmint    = __m256i;
using zmmint    = __m512i;

using xmmdouble = __m128d;
using ymmdouble = __m256d;
using zmmdouble = __m512d;

using xmmfloat  = __m128;
using ymmfloat  = __m256;
using zmmfloat  = __m512;

// Size limit (in bytes) before switching to non-temporal/streaming loads & stores
// Applies to: memmove, memset, and memcpy
#if !defined(base_simd_cache_size_limit)
#  define base_simd_cache_size_limit 3*1024*1024 // 3 MB
#endif // base_simd_cache_size_limit

__BASE_ARCH_NAMESPACE_END

#endif // lib_base_has_simd_support
