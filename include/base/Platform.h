#pragma once

#if defined(_WIN32)
#if !defined(_UNICODE)
#define _UNICODE
#endif
#if !defined(UNICODE)
#define UNICODE
#endif
#endif

#include "Windows.h"
#include "ProcessorDetection.h"

#include <cstdint>

#ifdef LIB_BASE_STATIC
#if defined(LIB_BASE)
#undef LIB_BASE
#endif
#define LIB_BASE
#endif

#if defined (_WIN32)
#define aligned_free(ptr)                   _aligned_free(ptr)
#else
#define aligned_free(ptr)                   free(ptr)
#endif

#define CLOCK_FREQ INT64_C(1000000)
#if (CLOCK_FREQ % 1000) == 0
#define TICK_FROM_MS(ms)  ((CLOCK_FREQ / INT64_C(1000)) * (ms))
#define MS_FROM_TICK(vtk) ((vtk) / (CLOCK_FREQ / INT64_C(1000)))
#elif (1000 % CLOCK_FREQ) == 0
#define TICK_FROM_MS(ms)  ((ms)  / (INT64_C(1000) / CLOCK_FREQ))
#define MS_FROM_TICK(vtk) ((vtk) * (INT64_C(1000) / CLOCK_FREQ))
#else /* Округленное переполненное преобразование */
#define TICK_FROM_MS(ms)  (CLOCK_FREQ * (ms) / 1000)
#define MS_FROM_TICK(vtk) ((vtk) * 1000 / CLOCK_FREQ)
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
#elif defined(_MSC_VER)
#define likely(p)                           (!!(p))
#define unlikely(p)                         (!!(p))
#define unreachable()                       (__assume(0))
#else
#define likely(p)                           (!!(p))
#define unlikely(p)                         (!!(p))
#define unreachable()                       ((void)0)
#endif

#define always_inline						__forceinline
#define unused(x)                           ((void)(x))
