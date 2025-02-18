#pragma once

#include <base/system/SystemString.h>

#include <base/system/SimdHelpers.h>
#include <base/utility/Assert.h>

#include <base/utility/Flags.h>

#include <base/system/SystemStaticCodeAnalysis.h>


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
    #define MSFTIME_FROM_TICK(tk)  ((tk)  / (CLOCK_FREQ / INT64_C(10000000))
#elif (10000000 % CLOCK_FREQ) == 0
    #define TICK_FROM_MSFTIME(msft) ((msft) / (INT64_C(10000000) / CLOCK_FREQ))
    #define MSFTIME_FROM_TICK(tk)  ((tk)  * (INT64_C(10000000) / CLOCK_FREQ))
#else /* rounded overflowing conversion */
    #define TICK_FROM_MSFTIME(msft) (CLOCK_FREQ * (msft) / INT64_C(10000000))
    #define MSFTIME_FROM_TICK(tk)  ((tk)  * INT64_C(10000000) / CLOCK_FREQ)
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

#if __has_cpp_attribute(gnu::malloc)
    #define DECLARE_MALLOCLIKE [[nodiscard, gnu::malloc]]
#else
    #define DECLARE_MALLOCLIKE [[nodiscard]]
#endif

#ifdef LIB_BASE_SYSTEM_NO_FAILURE
    #define SystemAssert						AssertReturn
#else
    #define SystemAssert(cond, mes, retval)		AssertLog(cond, mes)
#endif

