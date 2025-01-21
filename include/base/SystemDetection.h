#pragma once

#if defined(__APPLE__) && (defined(__GNUC__) || defined(__xlC__) || defined(__xlc__))
#  include <TargetConditionals.h>
#  define OS_APPLE
#  if defined(TARGET_OS_MAC) && TARGET_OS_MAC
#    define OS_DARWIN
#    define OS_BSD4
#    if defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
#      define QT_PLATFORM_UIKIT
#      if defined(TARGET_OS_WATCH) && TARGET_OS_WATCH
#        define OS_WATCHOS
#      elif defined(TARGET_OS_TV) && TARGET_OS_TV
#        define OS_TVOS
#      else
#        // TARGET_OS_IOS is only available in newer SDKs,
#        // so assume any other iOS-based platform is iOS for now
#        define OS_IOS
#      endif
#    else
#      // TARGET_OS_OSX is only available in newer SDKs,
#      // so assume any non iOS-based platform is macOS for now
#      define OS_MACOS
#    endif
#  else
#    error ""
#  endif
#elif defined(__WEBOS__)
#  define OS_WEBOS
#  define OS_LINUX
#elif defined(__ANDROID__) || defined(ANDROID)
#  define OS_ANDROID
#  define OS_LINUX
#elif defined(__CYGWIN__)
#  define OS_CYGWIN
#elif !defined(SAG_COM) && (!defined(WINAPI_FAMILY) || WINAPI_FAMILY==WINAPI_FAMILY_DESKTOP_APP) && (defined(WIN64) || defined(_WIN64) || defined(__WIN64__))
#  define OS_WIN32
#  define OS_WIN64
#elif !defined(SAG_COM) && (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
#    define OS_WIN32
#elif defined(__sun) || defined(sun)
#  define OS_SOLARIS
#elif defined(hpux) || defined(__hpux)
#  define OS_HPUX
#elif defined(__EMSCRIPTEN__)
#  define OS_WASM
#elif defined(__linux__) || defined(__linux)
#  define OS_LINUX
#elif defined(__FreeBSD__) || defined(__DragonFly__) || defined(__FreeBSD_kernel__)
#  ifndef __FreeBSD_kernel__
#    define OS_FREEBSD
#  endif
#  define OS_FREEBSD_KERNEL
#  define OS_BSD4
#elif defined(__NetBSD__)
#  define OS_NETBSD
#  define OS_BSD4
#elif defined(__OpenBSD__)
#  define OS_OPENBSD
#  define OS_BSD4
#elif defined(__INTERIX)
#  define OS_INTERIX
#  define OS_BSD4
#elif defined(_AIX)
#  define OS_AIX
#elif defined(__Lynx__)
#  define OS_LYNX
#elif defined(__GNU__)
#  define OS_HURD
#elif defined(__QNXNTO__)
#  define OS_QNX
#elif defined(__INTEGRITY)
#  define OS_INTEGRITY
#elif defined(__rtems__)
#  define OS_RTEMS
#elif defined(__vxworks)
#  define OS_VXWORKS
#elif defined(__HAIKU__)
#  define OS_HAIKU
#elif defined(__MAKEDEPEND__)
#else
#  error ""
#endif

#if defined(OS_WIN32) || defined(OS_WIN64)
#  define OS_WINDOWS
#  define OS_WIN
// On Windows, pointers to dllimport'ed variables are not constant expressions,
// so to keep to certain initializations (like QMetaObject) constexpr, we need
// to use functions instead.
#  define QT_NO_DATA_RELOCATION
#endif

#if defined(OS_WIN)
#  undef OS_UNIX
#elif !defined(OS_UNIX)
#  define OS_UNIX
#endif

// Compatibility synonyms
#ifdef OS_DARWIN
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wunknown-pragmas"
#  define OS_MAC // FIXME: Deprecate
#  ifdef __LP64__
#    define OS_DARWIN64
#    pragma clang deprecated(OS_DARWIN64, "use OS_DARWIN and QT_POINTER_SIZE/PROCESSOR_* instead")
#    define OS_MAC64
#    pragma clang deprecated(OS_MAC64, "use OS_DARWIN and QT_POINTER_SIZE/PROCESSOR_* instead")
#  else
#    define OS_DARWIN32
#    pragma clang deprecated(OS_DARWIN32, "use OS_DARWIN and QT_POINTER_SIZE/PROCESSOR_* instead")
#    define OS_MAC32
#    pragma clang deprecated(OS_MAC32, "use OS_DARWIN and QT_POINTER_SIZE/PROCESSOR_* instead")
#  endif
#  ifdef OS_MACOS
#    define OS_MACX
#    pragma clang deprecated(OS_MACX, "use OS_MACOS instead")
#    define OS_OSX
#    pragma clang deprecated(OS_OSX, "use OS_MACOS instead")
#  endif
#  pragma clang diagnostic pop
#endif

#ifdef OS_DARWIN
#  include <Availability.h>
#  include <AvailabilityMacros.h>

#  define QT_DARWIN_PLATFORM_SDK_EQUAL_OR_ABOVE(macos, ios, tvos, watchos) \
    ((defined(__MAC_OS_X_VERSION_MAX_ALLOWED) && macos != __MAC_NA && __MAC_OS_X_VERSION_MAX_ALLOWED >= macos) || \
     (defined(__IPHONE_OS_VERSION_MAX_ALLOWED) && ios != __IPHONE_NA && __IPHONE_OS_VERSION_MAX_ALLOWED >= ios) || \
     (defined(__TV_OS_VERSION_MAX_ALLOWED) && tvos != __TVOS_NA && __TV_OS_VERSION_MAX_ALLOWED >= tvos) || \
     (defined(__WATCH_OS_VERSION_MAX_ALLOWED) && watchos != __WATCHOS_NA && __WATCH_OS_VERSION_MAX_ALLOWED >= watchos))

#  define QT_DARWIN_DEPLOYMENT_TARGET_BELOW(macos, ios, tvos, watchos) \
    ((defined(__MAC_OS_X_VERSION_MIN_REQUIRED) && macos != __MAC_NA && __MAC_OS_X_VERSION_MIN_REQUIRED < macos) || \
     (defined(__IPHONE_OS_VERSION_MIN_REQUIRED) && ios != __IPHONE_NA && __IPHONE_OS_VERSION_MIN_REQUIRED < ios) || \
     (defined(__TV_OS_VERSION_MIN_REQUIRED) && tvos != __TVOS_NA && __TV_OS_VERSION_MIN_REQUIRED < tvos) || \
     (defined(__WATCH_OS_VERSION_MIN_REQUIRED) && watchos != __WATCHOS_NA && __WATCH_OS_VERSION_MIN_REQUIRED < watchos))

#  define QT_MACOS_IOS_PLATFORM_SDK_EQUAL_OR_ABOVE(macos, ios) \
      QT_DARWIN_PLATFORM_SDK_EQUAL_OR_ABOVE(macos, ios, __TVOS_NA, __WATCHOS_NA)
#  define QT_MACOS_PLATFORM_SDK_EQUAL_OR_ABOVE(macos) \
      QT_DARWIN_PLATFORM_SDK_EQUAL_OR_ABOVE(macos, __IPHONE_NA, __TVOS_NA, __WATCHOS_NA)
#  define QT_IOS_PLATFORM_SDK_EQUAL_OR_ABOVE(ios) \
      QT_DARWIN_PLATFORM_SDK_EQUAL_OR_ABOVE(__MAC_NA, ios, __TVOS_NA, __WATCHOS_NA)
#  define QT_TVOS_PLATFORM_SDK_EQUAL_OR_ABOVE(tvos) \
      QT_DARWIN_PLATFORM_SDK_EQUAL_OR_ABOVE(__MAC_NA, __IPHONE_NA, tvos, __WATCHOS_NA)
#  define QT_WATCHOS_PLATFORM_SDK_EQUAL_OR_ABOVE(watchos) \
      QT_DARWIN_PLATFORM_SDK_EQUAL_OR_ABOVE(__MAC_NA, __IPHONE_NA, __TVOS_NA, watchos)

#  define QT_MACOS_IOS_DEPLOYMENT_TARGET_BELOW(macos, ios) \
      QT_DARWIN_DEPLOYMENT_TARGET_BELOW(macos, ios, __TVOS_NA, __WATCHOS_NA)
#  define QT_MACOS_DEPLOYMENT_TARGET_BELOW(macos) \
      QT_DARWIN_DEPLOYMENT_TARGET_BELOW(macos, __IPHONE_NA, __TVOS_NA, __WATCHOS_NA)
#  define QT_IOS_DEPLOYMENT_TARGET_BELOW(ios) \
      QT_DARWIN_DEPLOYMENT_TARGET_BELOW(__MAC_NA, ios, __TVOS_NA, __WATCHOS_NA)
#  define QT_TVOS_DEPLOYMENT_TARGET_BELOW(tvos) \
      QT_DARWIN_DEPLOYMENT_TARGET_BELOW(__MAC_NA, __IPHONE_NA, tvos, __WATCHOS_NA)
#  define QT_WATCHOS_DEPLOYMENT_TARGET_BELOW(watchos) \
      QT_DARWIN_DEPLOYMENT_TARGET_BELOW(__MAC_NA, __IPHONE_NA, __TVOS_NA, watchos)

#else // !OS_DARWIN

#define QT_DARWIN_PLATFORM_SDK_EQUAL_OR_ABOVE(macos, ios, tvos, watchos) (0)
#define QT_MACOS_IOS_PLATFORM_SDK_EQUAL_OR_ABOVE(macos, ios) (0)
#define QT_MACOS_PLATFORM_SDK_EQUAL_OR_ABOVE(macos) (0)
#define QT_IOS_PLATFORM_SDK_EQUAL_OR_ABOVE(ios) (0)
#define QT_TVOS_PLATFORM_SDK_EQUAL_OR_ABOVE(tvos) (0)
#define QT_WATCHOS_PLATFORM_SDK_EQUAL_OR_ABOVE(watchos) (0)

#endif // OS_DARWIN

#ifdef __LSB_VERSION__
#  if __LSB_VERSION__ < 40
#    error "This version of the Linux Standard Base is unsupported"
#  endif
#ifndef QT_LINUXBASE
#  define QT_LINUXBASE
#endif
#endif

#if defined (__ELF__)
#  define OF_ELF
#endif
#if defined (__MACH__) && defined (__APPLE__)
#  define OF_MACH_O
#endif
