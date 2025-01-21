#pragma once

#include <base/ProcessorDetection.h>

#if defined(_MSC_VER)
#  define Q_CC_MSVC (_MSC_VER)
#  define Q_CC_MSVC_NET
#  define Q_CC_MSVC_ONLY Q_CC_MSVC
#  ifdef __clang__
#    undef Q_CC_MSVC_ONLY
#    define Q_CC_CLANG ((__clang_major__ * 100) + __clang_minor__)
#    define Q_CC_CLANG_ONLY Q_CC_CLANG
#  endif
#  define Q_OUTOFLINE_TEMPLATE inline
#  define Q_COMPILER_MANGLES_RETURN_TYPE
#  define Q_COMPILER_MANGLES_ACCESS_SPECIFIER
#  define Q_FUNC_INFO __FUNCSIG__
#  define Q_ASSUME_IMPL(expr) __assume(expr)
#  define Q_UNREACHABLE_IMPL() __assume(0)
#  define Q_DECL_EXPORT __declspec(dllexport)
#  define Q_DECL_IMPORT __declspec(dllimport)
#  if _MSC_VER < 1938 // stdext is deprecated since VS 2022 17.8
#    define QT_MAKE_CHECKED_ARRAY_ITERATOR(x, N) stdext::make_checked_array_iterator(x, size_t(N)) // Since _MSC_VER >= 1500
#  endif
#  define Q_COMPILER_COMPLAINS_ABOUT_RETURN_AFTER_UNREACHABLE

#elif defined(__GNUC__)
#  define Q_CC_GNU          (__GNUC__ * 100 + __GNUC_MINOR__)
#  if defined(__MINGW32__)
#    define Q_CC_MINGW
#  endif
#  if defined(__clang__)
#    if defined(__apple_build_version__)
#      if __apple_build_version__   >= 14030022 // Xcode 14.3
#        define Q_CC_CLANG 1500
#      elif __apple_build_version__ >= 14000029 // Xcode 14.0
#        define Q_CC_CLANG 1400
#      elif __apple_build_version__ >= 13160021 // Xcode 13.3
#        define Q_CC_CLANG 1300
#      elif __apple_build_version__ >= 13000029 // Xcode 13.0
#        define Q_CC_CLANG 1200
#      elif __apple_build_version__ >= 12050022 // Xcode 12.5
#        define Q_CC_CLANG 1110
#      elif __apple_build_version__ >= 12000032 // Xcode 12.0
#        define Q_CC_CLANG 1000
#      elif __apple_build_version__ >= 11030032 // Xcode 11.4
#        define Q_CC_CLANG 900
#      elif __apple_build_version__ >= 11000033 // Xcode 11.0
#        define Q_CC_CLANG 800
#      else
#        error "Unsupported Apple Clang version"
#      endif
#    else
       // Non-Apple Clang, so we trust the versions reported
#      define Q_CC_CLANG ((__clang_major__ * 100) + __clang_minor__)
#    endif
#    define Q_CC_CLANG_ONLY Q_CC_CLANG
#    if __has_builtin(__builtin_assume)
#      define Q_ASSUME_IMPL(expr)   __builtin_assume(expr)
#    else
#      define Q_ASSUME_IMPL(expr)  if (expr){} else __builtin_unreachable()
#    endif
#    define Q_UNREACHABLE_IMPL() __builtin_unreachable()
#    if !defined(__has_extension)
#      /* Compatibility with older Clang versions */
#      define __has_extension __has_feature
#    endif
#  else

#    define Q_CC_GNU_ONLY Q_CC_GNU
#    if Q_CC_GNU >= 405
#      define Q_ASSUME_IMPL(expr)  if (expr){} else __builtin_unreachable()
#      define Q_UNREACHABLE_IMPL() __builtin_unreachable()
#      define Q_DECL_DEPRECATED_X(text) __attribute__ ((__deprecated__(text)))
#    endif
#  endif

#  ifdef Q_OS_WIN
#    define Q_DECL_EXPORT     __declspec(dllexport)
#    define Q_DECL_IMPORT     __declspec(dllimport)
#  else
#    define Q_DECL_EXPORT_OVERRIDABLE __attribute__((visibility("default"), weak))
#    ifdef QT_USE_PROTECTED_VISIBILITY
#      define Q_DECL_EXPORT     __attribute__((visibility("protected")))
#    else
#      define Q_DECL_EXPORT     __attribute__((visibility("default")))
#    endif
#    define Q_DECL_IMPORT     __attribute__((visibility("default")))
#    define Q_DECL_HIDDEN     __attribute__((visibility("hidden")))
#  endif

#  define Q_FUNC_INFO       __PRETTY_FUNCTION__
#  define Q_TYPEOF(expr)    __typeof__(expr)
#  define Q_DECL_DEPRECATED __attribute__ ((__deprecated__))
#  define Q_DECL_UNUSED     __attribute__((__unused__))
#  define Q_LIKELY(expr)    __builtin_expect(!!(expr), true)
#  define Q_UNLIKELY(expr)  __builtin_expect(!!(expr), false)
#  define Q_NORETURN        __attribute__((__noreturn__))
#  define Q_REQUIRED_RESULT __attribute__ ((__warn_unused_result__))
#  define Q_DECL_PURE_FUNCTION __attribute__((pure))
#  define Q_DECL_CONST_FUNCTION __attribute__((const))
#  define Q_DECL_COLD_FUNCTION __attribute__((cold))
#  if !defined(QT_MOC_CPP)
#    define Q_PACKED __attribute__ ((__packed__))
#    ifndef __ARM_EABI__
#      define QT_NO_ARM_EABI
#    endif
#  endif
#  if Q_CC_GNU >= 403 && !defined(Q_CC_CLANG)
#      define Q_ALLOC_SIZE(x) __attribute__((alloc_size(x)))
#  endif

#ifndef __has_builtin
#  define __has_builtin(x)             0
#endif
#ifndef __has_feature
#  define __has_feature(x)             0
#endif
#ifndef __has_attribute
#  define __has_attribute(x)           0
#endif
#ifndef __has_cpp_attribute
#  define __has_cpp_attribute(x)       0
#endif
#ifndef __has_include
#  define __has_include(x)             0
#endif
#ifndef __has_include_next
#  define __has_include_next(x)        0
#endif


#ifdef __cplusplus
# if __has_include(<version>) /* remove this check once Integrity, QNX have caught up */
#  include <version>
# endif
#endif

#define QT_DO_PRAGMA(text)                      _Pragma(#text)
#if defined(Q_CC_MSVC) && !defined(Q_CC_CLANG)
#  undef QT_DO_PRAGMA                           /* not needed */
#  define QT_WARNING_PUSH                       __pragma(warning(push))
#  define QT_WARNING_POP                        __pragma(warning(pop))
#  define QT_WARNING_DISABLE_MSVC(number)       __pragma(warning(disable: number))
#  define QT_WARNING_DISABLE_INTEL(number)
#  define QT_WARNING_DISABLE_CLANG(text)
#  define QT_WARNING_DISABLE_GCC(text)
#  define QT_WARNING_DISABLE_DEPRECATED         QT_WARNING_DISABLE_MSVC(4996)
#  define QT_WARNING_DISABLE_FLOAT_COMPARE
#  define QT_WARNING_DISABLE_INVALID_OFFSETOF
#elif defined(Q_CC_CLANG)
#  define QT_WARNING_PUSH                       QT_DO_PRAGMA(clang diagnostic push)
#  define QT_WARNING_POP                        QT_DO_PRAGMA(clang diagnostic pop)
#  define QT_WARNING_DISABLE_CLANG(text)        QT_DO_PRAGMA(clang diagnostic ignored text)
#  define QT_WARNING_DISABLE_GCC(text)
#  define QT_WARNING_DISABLE_INTEL(number)
#  define QT_WARNING_DISABLE_MSVC(number)
#  define QT_WARNING_DISABLE_DEPRECATED         QT_WARNING_DISABLE_CLANG("-Wdeprecated-declarations")
#  define QT_WARNING_DISABLE_FLOAT_COMPARE      QT_WARNING_DISABLE_CLANG("-Wfloat-equal")
#  define QT_WARNING_DISABLE_INVALID_OFFSETOF   QT_WARNING_DISABLE_CLANG("-Winvalid-offsetof")
#elif defined(Q_CC_GNU) && (__GNUC__ * 100 + __GNUC_MINOR__ >= 406)
#  define QT_WARNING_PUSH                       QT_DO_PRAGMA(GCC diagnostic push)
#  define QT_WARNING_POP                        QT_DO_PRAGMA(GCC diagnostic pop)
#  define QT_WARNING_DISABLE_GCC(text)          QT_DO_PRAGMA(GCC diagnostic ignored text)
#  define QT_WARNING_DISABLE_CLANG(text)
#  define QT_WARNING_DISABLE_INTEL(number)
#  define QT_WARNING_DISABLE_MSVC(number)
#  define QT_WARNING_DISABLE_DEPRECATED         QT_WARNING_DISABLE_GCC("-Wdeprecated-declarations")
#  define QT_WARNING_DISABLE_FLOAT_COMPARE      QT_WARNING_DISABLE_GCC("-Wfloat-equal")
#  define QT_WARNING_DISABLE_INVALID_OFFSETOF   QT_WARNING_DISABLE_GCC("-Winvalid-offsetof")
#else       // All other compilers, GCC < 4.6 and MSVC < 2008
#  define QT_WARNING_DISABLE_GCC(text)
#  define QT_WARNING_PUSH
#  define QT_WARNING_POP
#  define QT_WARNING_DISABLE_INTEL(number)
#  define QT_WARNING_DISABLE_MSVC(number)
#  define QT_WARNING_DISABLE_CLANG(text)
#  define QT_WARNING_DISABLE_GCC(text)
#  define QT_WARNING_DISABLE_DEPRECATED
#  define QT_WARNING_DISABLE_FLOAT_COMPARE
#  define QT_WARNING_DISABLE_INVALID_OFFSETOF
#endif

#if !defined(Q_PROCESSOR_X86)
#  undef COMPILER_SUPPORTS_SSE2
#  undef COMPILER_SUPPORTS_SSE3
#  undef COMPILER_SUPPORTS_SSSE3
#  undef COMPILER_SUPPORTS_SSE4_1
#  undef COMPILER_SUPPORTS_SSE4_2
#  undef COMPILER_SUPPORTS_AVX
#  undef COMPILER_SUPPORTS_AVX2
#  undef COMPILER_SUPPORTS_F16C
#endif


#ifdef q_cc_msvc
#  define q_never_inline __declspec(noinline)
#  define q_always_inline __forceinline
#elif defined(q_cc_gnu)
#  define q_never_inline __attribute__((noinline))
#  define q_always_inline inline __attribute__((always_inline))
#else
#  define q_never_inline
#  define q_always_inline inline
#endif

#if !defined(QT_CC_WARNINGS)
#  define QT_NO_WARNINGS
#endif
#if defined(QT_NO_WARNINGS)
#  if defined(Q_CC_MSVC)
QT_WARNING_DISABLE_MSVC(4251) /* class 'type' needs to have dll-interface to be used by clients of class 'type2' */
QT_WARNING_DISABLE_MSVC(4244) /* conversion from 'type1' to 'type2', possible loss of data */
QT_WARNING_DISABLE_MSVC(4275) /* non - DLL-interface classkey 'identifier' used as base for DLL-interface classkey 'identifier' */
QT_WARNING_DISABLE_MSVC(4514) /* unreferenced inline function has been removed */
QT_WARNING_DISABLE_MSVC(4800) /* 'type' : forcing value to bool 'true' or 'false' (performance warning) */
QT_WARNING_DISABLE_MSVC(4097) /* typedef-name 'identifier1' used as synonym for class-name 'identifier2' */
QT_WARNING_DISABLE_MSVC(4706) /* assignment within conditional expression */
QT_WARNING_DISABLE_MSVC(4355) /* 'this' : used in base member initializer list */
QT_WARNING_DISABLE_MSVC(4710) /* function not inlined */
QT_WARNING_DISABLE_MSVC(4530) /* C++ exception handler used, but unwind semantics are not enabled. Specify /EHsc */
#  elif defined(Q_CC_BOR)
#    pragma option -w-inl
#    pragma option -w-aus
#    pragma warn -inl
#    pragma warn -pia
#    pragma warn -ccc
#    pragma warn -rch
#    pragma warn -sig
#  endif
#endif

#endif // __cplusplus