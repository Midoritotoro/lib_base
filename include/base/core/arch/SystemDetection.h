#pragma once

#if defined(__APPLE__) && (defined(__GNUC__) || defined(__xlC__) || defined(__xlc__))
#  define OS_APPLE
#  if defined(TARGET_OS_MAC) && TARGET_OS_MAC
#    define OS_DARWIN
#    define OS_BSD4
#    if defined(OS_IPHONE) && TARGET_OS_IPHONE
#    else
#      define OS_MAC
#    endif
#  endif
#elif defined(__CYGWIN__)
#  define OS_CYGWIN
#elif !defined(SAG_COM) && (!defined(WINAPI_FAMILY) || WINAPI_FAMILY==WINAPI_FAMILY_DESKTOP_APP) && (defined(WIN64) || defined(_WIN64) || defined(__WIN64__))
#  define OS_WIN32
#  define OS_WIN64
#elif !defined(SAG_COM) && (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
#    define OS_WIN32
#elif defined(__linux__) || defined(__linux)
#  define OS_LINUX

#elif defined(__Lynx__)
#  define OS_LYNX

#elif defined(__GNU__)
#  define OS_HURD

#elif defined(__FreeBSD__)
#  define OS_FREEBSD

#elif defined(__NetBSD__)
#  define OS_NETBSD

#elif defined(__OpenBSD__)
#  define OS_OPENBSD

#elif defined(__DragonFly__)
#  define OS_DRAGONFLY

#elif defined(__linux__)
#  define OS_LINUX

#elif defined(__native_client__)
#  define OS_NACL

#elif defined(__EMSCRIPTEN__)
#  define OS_EMSCRIPTEN

#elif defined(__rtems__)
#  define OS_RTEMS

#elif defined(__Fuchsia__)
#  define OS_FUCHSIA

#elif defined (__SVR4) && defined (__sun)
#  define OS_SOLARIS

#elif defined(__QNX__)
#  define OS_QNX

#elif defined(__MVS__)
#  define OS_ZOS

#elif defined(__hexagon__)
#  define OS_QURT

#endif
#else
#  error ""
#endif

#if defined(OS_WIN32) || defined(OS_WIN64)
#  define OS_WINDOWS
#  define OS_WIN
#endif

#if defined(OS_WIN)
#  undef OS_UNIX
#elif !defined(OS_UNIX)
#  define OS_UNIX
#endif

