#pragma once

#include <base/system/SimdHelpers.h>
#include <base/Assert.h>

#include <base/Flags.h>

#include <base/system/SystemStaticCodeAnalysis.h>
#include <base/system/SystemString.h>


namespace base::system {
    enum FileOpenMode : uchar {
        Read = 0x01, // "r"
        Write = 0x02, // "w"
        Append = 0x04, // "a"
        ReadEx = 0x10, // "r+"
        WriteEx = 0x20, // "w+"
        AppendEx = 0x40 // "a+"
#if defined(OS_WIN)
        ,
        // Так как в cstdlib fopen() флаг "b" игнорируется в системах POSIX,
        // то и использовать его там смысла нет. В Windows флаг "b" отключает обработку '\n' и '\x1A'.
        Binary = 0x80 // "b"
#endif
    };

    enum FilePosition : uchar {
        FileBegin = 0x01,
        FileEnd = 0x02
    };

    DECLARE_FLAGS(FileOpenModes, FileOpenMode);
    DECLARE_FLAGS(FilePositions, FilePosition);

    struct FileFilter {
        base_string nameContains;
        sizetype minimumSize = 0;
    };
} // namespace base::system



#if defined(OS_MAC) || defined(OS_LINUX)
    #include <unistd.h>
    #include <fcntl.h> // Для fcntl
#elif defined(OS_WIN)
    #include <io.h> // Для _get_osfhandle
#endif

#include <limits>  // Для PATH_MAX



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


#if defined(__GNUC__) || defined(__clang__)
    #define always_inline						inline __attribute__((always_inline))
#elif defined(CPP_MSVC)
    #define always_inline						__forceinline
#else
    #define always_inline						inline
#endif


#ifdef LIB_BASE_SYSTEM_NO_FAILURE
    #define SystemAssert						AssertReturn
#else
    #define SystemAssert(cond, mes, retval)		AssertLog(cond, mes)
#endif

