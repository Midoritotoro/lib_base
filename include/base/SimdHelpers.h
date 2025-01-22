#pragma once

#include <base/SystemDetection.h>
#include <base/CompilerDetection.h>

#include <qsimd.h>


#define LIB_BASE_ENABLE(feature) (1/LIB_BASE_ENABLE_##feature == 1)

#if defined(PROCESSOR_ARM) && defined(__ARM_NEON) || defined(__ARM_NEON__)
#  include <arm_neon.h>
#  define LIB_BASE_ENABLE_neon 1
#else
#  define LIB_BASE_ENABLE_neon -1
#endif

#if defined(PROCESSOR_MIPS) && (defined(__MIPS_DSP__) || (defined(__mips_dsp) && defined(PROCESSOR_MIPS_32)))
#  define LIB_BASE_ENABLE_mips_dsp 1
#else
#  define LIB_BASE_ENABLE_mips_dsp -1
#endif

#if defined(PROCESSOR_MIPS) && (defined(__MIPS_DSPR2__) || (defined(__mips_dspr2) && defined(PROCESSOR_MIPS_32)))
#  define LIB_BASE_ENABLE_mips_dspr2 1
#else
#  define LIB_BASE_ENABLE_mips_dspr2 -1
#endif

#if defined(PROCESSOR_X86) && defined(CPP_MSVC)
 // MSVC doesn't define __SSE2__, so do it ourselves
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
#    ifndef __AVX__
#      define __AVX__                       1
#    endif
#  endif
#  ifdef __SSE2__
#    define VECTORCALL __vectorcall
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
// Starting with /arch:AVX512, MSVC defines all the macros
# elif defined(PROCESSOR_X86) && defined(CPP_GCC)
# if __builtin_cpu_supports("sse")
#	define __SSE__
# elif __builtin_cpu_supports("sse2")
#	define __SSE2__
# elif __builtin_cpu_supports("sse3")
#	define __SSE3__
# elif __builtin_cpu_supports("sse4.1")
#	define __SSE4_1__
# elif __builtin_cpu_supports("sse4.2")
#	define __SSE4_2__
# elif __builtin_cpu_supports("avx")
#	define __AVX__
# elif __builtin_cpu_supports("avx2")
#	define __AVX2__
# endif
# endif


#if defined(PROCESSOR_X86) && defined(__SSE2__)
#  include <immintrin.h>
#  define LIB_BASE_ENABLE_sse2 1
#else
#  define LIB_BASE_ENABLE_sse2 -1
#endif

#if defined(PROCESSOR_X86) && defined(__SSE3__)
#  define LIB_BASE_ENABLE_sse3 1
#else
#  define LIB_BASE_ENABLE_sse3 -1
#endif

#if defined(PROCESSOR_X86) && defined(__SSSE3__)
#  define LIB_BASE_ENABLE_ssse3 1
#else
#  define LIB_BASE_ENABLE_ssse3 -1
#endif

#if defined(PROCESSOR_X86) && defined(__SSE4_1__)
#  define LIB_BASE_ENABLE_sse4_1 1
#else
#  define LIB_BASE_ENABLE_sse4_1 -1
#endif

#if defined(PROCESSOR_X86) && defined(__SSE4_2__)
#  define LIB_BASE_ENABLE_sse4_2 1
#else
#  define LIB_BASE_ENABLE_sse4_2 -1
#endif

#if defined(PROCESSOR_X86) && defined(__AVX__)
#  define LIB_BASE_ENABLE_avx 1
#else
#  define LIB_BASE_ENABLE_avx -1
#endif

#if defined(PROCESSOR_X86) && defined(__AVX2__)
#  define LIB_BASE_ENABLE_avx2 1
#else
#  define LIB_BASE_ENABLE_avx2 -1
#endif


#ifndef VECTORCALL
#define VECTORCALL
#endif