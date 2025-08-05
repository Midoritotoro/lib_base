#pragma once

#if defined(__APPLE__) && (defined(__GNUC__) || defined(__xlC__) || defined(__xlc__))
#  define base_os_apple
#  if defined(TARGET_OS_MAC) && TARGET_OS_MAC
#    define base_os_darwin
#    define base_os_bsd4
#    if defined(OS_IPHONE) && TARGET_OS_IPHONE
#    else
#      define base_os_mac
#    endif
#  endif
#elif defined(__CYGWIN__)
#  define base_os_cygwin
#elif !defined(SAG_COM) && (!defined(WINAPI_FAMILY) || WINAPI_FAMILY==WINAPI_FAMILY_DESKTOP_APP) && (defined(WIN64) || defined(_WIN64) || defined(__WIN64__))
#  define base_os_win32
#  define base_os_win64
#elif !defined(SAG_COM) && (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
#  define base_os_win32
#elif defined(__linux__) || defined(__linux)
#  define base_os_linux

#elif defined(__Lynx__)
#  define base_os_lynx

#elif defined(__GNU__)
#  define base_os_hurd

#elif defined(__FreeBSD__)
#  define base_os_freebsd

#elif defined(__NetBSD__)
#  define base_os_netbsd

#elif defined(__OpenBSD__)
#  define base_os_openbsd

#elif defined(__DragonFly__)
#  define base_os_dragonfly

#elif defined(__linux__)
#  define base_os_linux

#elif defined(__native_client__)
#  define base_os_nacl

#elif defined(__EMSCRIPTEN__)
#  define base_os_emscripten

#elif defined(__rtems__)
#  define base_os_rtems

#elif defined(__Fuchsia__)
#  define base_os_fuchsia

#elif defined (__SVR4) && defined (__sun)
#  define base_os_solaris

#elif defined(__QNX__)
#  define base_os_qnx

#elif defined(__MVS__)
#  define base_os_zos

#elif defined(__hexagon__)
#  define base_os_qurt

#else
#  error ""
#endif

#if defined(base_os_win32) || defined(base_os_win64)
#  define base_os_windows
#  define base_os_win
#endif

#if defined(base_os_windows)
#  undef base_os_unix
#elif !defined(base_os_unix)
#  define base_os_unix
#endif

