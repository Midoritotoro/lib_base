#pragma once

#include <base/system/SimdHelpers.h>

#include <base/Types.h>
#include <base/BaseExport.h>

#include <cstdint>
#include <span>


#if defined(OS_WIN)
	#include <base/system/Windows.h>

	#if defined(CPP_MSVC)
		#include <sal.h>
	#endif

	#ifndef UNICODE
		#define UNICODE
	#endif

	#ifndef _UNICODE
		#define _UNICODE
	#endif
	
	#define LIB_BASE_ENABLE_WINDOWS_UNICODE
#endif

// Игнор SAL в случае компилятора, отличного от MSVC
#if defined(OS_WIN) && defined(CPP_MSVC)
	#ifndef _SAL2_In_reads_bytes_
		#define _SAL2_In_reads_bytes_		_In_reads_bytes_
	#endif

	#ifndef _SAL2_Out_writes_bytes_
		#define _SAL2_Out_writes_bytes_		_Out_writes_bytes_
	#endif

	#ifndef _SAL2_In
		#define _SAL2_In_					_In_
	#endif

	#ifndef _SAL2_Out
		#define _SAL2_Out_					_Out_
	#endif

	#ifndef _SAL2_In_z
		#define _SAL2_In_z_					_In_z_
	#endif
#else
	#define _SAL2_In_reads_bytes_(size)		
	#define _SAL2_Out_writes_bytes_(size)	

	#define _SAL2_In_						
	#define _SAL2_Out_						

	#define _SAL2_In_z_						
#endif




#ifdef LIB_BASE_STATIC
#if defined(LIB_BASE)
#undef LIB_BASE
#endif
#define LIB_BASE
#endif

#if defined(OS_WIN)
	#define aligned_malloc(size, alignment)					_aligned_malloc(size, alignment)
#else
	#define aligned_malloc(size, alignment)					malloc(size)
#endif

#if defined (OS_WIN)
	#define aligned_free(ptr)                   _aligned_free(ptr)
#else
	#define aligned_free(ptr)                   free(ptr)
#endif

#define CLOCK_FREQ INT64_C(1000000)
#if (CLOCK_FREQ % 1000) == 0
#define TICK_FROM_MS(ms)  ((CLOCK_FREQ / INT64_C(1000)) * (ms))
#define MS_FROM_TICK(tk) ((tk) / (CLOCK_FREQ / INT64_C(1000)))
#elif (1000 % CLOCK_FREQ) == 0
#define TICK_FROM_MS(ms)  ((ms)  / (INT64_C(1000) / CLOCK_FREQ))
#define MS_FROM_TICK(tk) ((tk) * (INT64_C(1000) / CLOCK_FREQ))
#else
#define TICK_FROM_MS(ms)  (CLOCK_FREQ * (ms) / 1000)
#define MS_FROM_TICK(tk) ((tk) * 1000 / CLOCK_FREQ)
#endif /* CLOCK_FREQ / 1000 */

typedef int64_t msftime_t;
typedef int64_t tick_t;

#define MSFTIME_FROM_SEC(sec)       (INT64_C(10000000) * (sec))  /* Секунды в msftime_t */
#define MSFTIME_FROM_MS(sec)        (INT64_C(10000) * (sec))     /* Миллисекунды в msftime_t */

#if (CLOCK_FREQ % 10000000) == 0
#define TICK_FROM_MSFTIME(msft) ((msft) * (CLOCK_FREQ / INT64_C(10000000))
#define MSFTIME_FROM_TICK(vtk)  ((vtk)  / (CLOCK_FREQ / INT64_C(10000000))
#elif (10000000 % CLOCK_FREQ) == 0
#define TICK_FROM_MSFTIME(msft) ((msft) / (INT64_C(10000000) / CLOCK_FREQ))
#define MSFTIME_FROM_TICK(vtk)  ((vtk)  * (INT64_C(10000000) / CLOCK_FREQ))
#else /* rounded overflowing conversion */
#define TICK_FROM_MSFTIME(msft) (CLOCK_FREQ * (msft) / INT64_C(10000000))
#define MSFTIME_FROM_TICK(vtk)  ((vtk)  * INT64_C(10000000) / CLOCK_FREQ)
#endif /* CLOCK_FREQ / 10000000 */

#if defined (__GNUC__) || defined (__clang__)

	#define likely(p)                           __builtin_expect(!!(p), 1)
	#define unlikely(p)                         __builtin_expect(!!(p), 0)
	#define unreachable()                       __builtin_unreachable()

#elif defined(CPP_MSVC)

	#define likely(p)                           (!!(p))
	#define unlikely(p)                         (!!(p))
	#define unreachable()                       (__assume(0))

#else

	#define likely(p)                           (!!(p))
	#define unlikely(p)                         (!!(p))
	#define unreachable()                       ((void)0)

#endif


#if defined(__GNUC__) || defined(__clang__)
	#define always_inline						inline __attribute__((always_inline))
#elif defined(CPP_MSVC)
	#define always_inline						__forceinline
#else
	#define always_inline						inline
#endif

#define unused(x)								((void)(x))