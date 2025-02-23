#pragma once

#include <base/system/SystemDetection.h>
#include <base/system/CompilerDetection.h>

#include <base/system/KeywordSupport.h>


#ifndef LIB_BASE_USE_COMPILER_ALIGNMENT 
#  define LIB_BASE_USE_COMPILER_ALIGNMENT
#endif

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
#  if defined (LIB_BASE_USE_COMPILER_ALIGNMENT)
#    if defined(CPP_GNU) || defined(CPP_CLANG)
#      define SSE2_ALIGNAS(size)	__attribute__((aligned(size))) 
#    else
#      define SSE2_ALIGNAS			alignas
#    endif
#  else
#    define SSE2_ALIGNAS	 		alignas
#  endif
#else
#  define SSE2_ALIGNAS(size)
#  define LIB_BASE_ENABLE_sse2 -1
#endif

#if defined(PROCESSOR_X86) && defined(__SSE3__)
#  define LIB_BASE_ENABLE_sse3 1
#  if defined (LIB_BASE_USE_COMPILER_ALIGNMENT)
#    if defined(CPP_GNU) || defined(CPP_CLANG)
#      define SSE3_ALIGNAS(size)	__attribute__((aligned(size))) 
#    else
#      define SSE3_ALIGNAS			alignas
#    endif
#  else
#    define SSE3_ALIGNAS	 		alignas
#  endif
#else
#  define SSE3_ALIGNAS(size)
#  define LIB_BASE_ENABLE_sse3 -1
#endif

#if defined(PROCESSOR_X86) && defined(__SSSE3__)
#  define LIB_BASE_ENABLE_ssse3 1
#  if defined (LIB_BASE_USE_COMPILER_ALIGNMENT)
#    if defined(CPP_GNU) || defined(CPP_CLANG)
#      define SSSE3_ALIGNAS(size)	__attribute__((aligned(size)))
#    else
#      define SSSE3_ALIGNAS			alignas
#    endif
#  else
#    define SSSE3_ALIGNAS	 		alignas
#  endif
#else
#  define SSSE3_ALIGNAS(size)
#  define LIB_BASE_ENABLE_ssse3 -1
#endif

#if defined(PROCESSOR_X86) && defined(__SSE4_1__)
#  define LIB_BASE_ENABLE_sse4_1 1
#  if defined (LIB_BASE_USE_COMPILER_ALIGNMENT)
#    if defined(CPP_GNU) || defined(CPP_CLANG)
#      define SSE4_1_ALIGNAS(size)	__attribute__((aligned(size)))
#    else
#      define SSE4_1_ALIGNAS		alignas
#    endif
#  else
#    define SSE4_1_ALIGNAS	 	    alignas
#  endif
#else
#  define SSE4_1_ALIGNAS(size)
#  define LIB_BASE_ENABLE_sse4_1 -1
#endif

#if defined(PROCESSOR_X86) && defined(__SSE4_2__)
#  define LIB_BASE_ENABLE_sse4_2 1
#  if defined (LIB_BASE_USE_COMPILER_ALIGNMENT)
#    if defined(CPP_GNU) || defined(CPP_CLANG)
#      define SSE4_2_ALIGNAS(size)	__attribute__((aligned(size)))
#    else
#      define SSE4_2_ALIGNAS		alignas
#    endif
#  else
#    define SSE4_2_ALIGNAS	 		alignas
#  endif
#else
#  define SSE4_2_ALIGNAS(size)
#  define LIB_BASE_ENABLE_sse4_2 -1
#endif

#if defined(PROCESSOR_X86) && defined(__AVX__)
#  define LIB_BASE_ENABLE_avx 1
#  if defined (LIB_BASE_USE_COMPILER_ALIGNMENT)
#    if defined(CPP_GNU) || defined(CPP_CLANG)
#      define AVX_ALIGNAS(size)		__attribute__((aligned(size)))
#    else
#      define AVX_ALIGNAS			alignas
#    endif
#  else
#    define AVX_ALIGNAS	 			alignas
#  endif
#else
#  define AVX_ALIGNAS(size)
#  define LIB_BASE_ENABLE_avx -1
#endif

#if defined(PROCESSOR_X86) && defined(__AVX2__)
#  define LIB_BASE_ENABLE_avx2 1
#  if defined (LIB_BASE_USE_COMPILER_ALIGNMENT)
#    if defined(CPP_GNU) || defined(CPP_CLANG)
#      define AVX2_ALIGNAS(size)	__attribute__((aligned(size)))
#    else
#      define AVX2_ALIGNAS			alignas
#    endif
#  else
#    define AVX2_ALIGNAS	 		alignas
#  endif
#else
#  define AVX2_ALIGNAS(size)
#  define LIB_BASE_ENABLE_avx2 -1
#endif

#ifndef VECTORCALL
	#define VECTORCALL
#endif