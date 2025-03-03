#pragma once

#include <base/system/SystemDetection.h>
#include <base/system/CompilerDetection.h>

#include <base/system/KeywordSupport.h>

#include <qconfig.h>


#ifndef LIB_BASE_USE_COMPILER_ALIGNMENT 
#  define LIB_BASE_USE_COMPILER_ALIGNMENT
#endif

#define __LIB_BASE_ENABLE_IMPL(feature) (((1/LIB_BASE_ENABLE_##feature) == 1))
#define LIB_BASE_ENABLE(feature) __LIB_BASE_ENABLE_IMPL(feature)

#if defined(PROCESSOR_X86) && defined(CPP_MSVC)

#  if (defined(_M_X64) || _M_IX86_FP >= 2)
#    define __SSE__ 1
#    define __SSE2__ 1
#  endif
// ������� � __has_include
#  if (defined(_M_AVX) || defined(__AVX__) || __has_include(<nmmintrin.h>)) // SSE4.2)
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
// MSVC ���������� __AVX2__ � /arch:AVX2
#    define __F16C__                        1
#    define __RDRND__                       1

#    define __FMA__                         1

#    define __BMI__                         1
#    define __BMI2__                        1

#    define __MOVBE__                       1
#    define __LZCNT__                       1
#  endif
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

#ifdef PROCESSOR_X86
#if LIB_BASE_ENABLE(sse2) || LIB_BASE_ENABLE(sse3)											\
	|| LIB_BASE_ENABLE(ssse3) || LIB_BASE_ENABLE(sse4_1) || LIB_BASE_ENABLE(sse4_2)			\
	|| LIB_BASE_ENABLE(avx) || LIB_BASE_ENABLE(avx2)										
#  if defined (LIB_BASE_USE_COMPILER_ALIGNMENT)
#    if defined(CPP_GNU) || defined(CPP_CLANG)
#      define SIMD_ALIGNAS(size)	__attribute__((aligned(size)))
#    else
#      define SIMD_ALIGNAS			alignas
#    endif
#  else
#    define SIMD_ALIGNAS	 		alignas
#  endif
#endif
#endif

#ifndef VECTORCALL
	#define VECTORCALL
#endif

#ifdef LIB_BASE_ENABLE_sse4_2
#  ifndef BASE_SIMD_SSE4_2_ALIGNMENT
#    define BASE_SIMD_SSE4_2_ALIGNMENT	sizeof(__m128i)
#  endif
#else 
#  define BASE_SIMD_SSE4_2_ALIGNMENT	0
#endif

#ifdef LIB_BASE_ENABLE_sse4_1
#  ifndef BASE_SIMD_SSE4_1_ALIGNMENT
#    define BASE_SIMD_SSE4_1_ALIGNMENT	sizeof(__m128i)
#  endif
#else 
#  define BASE_SIMD_SSE4_1_ALIGNMENT	0
#endif

#ifdef LIB_BASE_ENABLE_ssse3
#  ifndef BASE_SIMD_SSSE3_ALIGNMENT
#    define BASE_SIMD_SSSE3_ALIGNMENT	sizeof(__m128i)
#  endif
#else 
#  define BASE_SIMD_SSSE3_ALIGNMENT		0
#endif

#ifdef LIB_BASE_ENABLE_sse2
#  ifndef BASE_SIMD_SSE2_ALIGNMENT
#    define BASE_SIMD_SSE2_ALIGNMENT	sizeof(__m128i)
#  endif
#else 
#  define BASE_SIMD_SSE2_ALIGNMENT		0
#endif
