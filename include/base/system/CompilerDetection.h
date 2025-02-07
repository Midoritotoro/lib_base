#pragma once

#include <base/system/ProcessorDetection.h>
#include <base/system/SystemDetection.h>

#if defined(_MSC_VER)
#  define CPP_MSVC (_MSC_VER)
#  define CPP_MSVC_NET
#  define CPP_MSVC_ONLY CPP_MSVC
#  ifdef __clang__
#    undef CPP_MSVC_ONLY
#    define CPP_CLANG ((__clang_major__ * 100) + __clang_minor__)
#    define CPP_CLANG_ONLY CPP_CLANG
#  endif
#  define ASSUME_IMPL(expr) __assume(expr)
#  define UNREACHABLE_IMPL() __assume(0)
#  define DECL_EXPORT __declspec(dllexport)
#  define DECL_IMPORT __declspec(dllimport)
#  define COMPILER_COMPLAINS_ABOUT_RETURN_AFTER_UNREACHABLE
#elif defined(__GNUC__)
#  define CPP_GNU          (__GNUC__ * 100 + __GNUC_MINOR__)
#  if defined(__MINGW32__)
#    define CPP_MINGW
#  endif
#  if defined(__clang__)
#    if defined(__apple_build_version__)
#      if __apple_build_version__   >= 14030022 // Xcode 14.3
#        define CPP_CLANG 1500
#      elif __apple_build_version__ >= 14000029 // Xcode 14.0
#        define CPP_CLANG 1400
#      elif __apple_build_version__ >= 13160021 // Xcode 13.3
#        define CPP_CLANG 1300
#      elif __apple_build_version__ >= 13000029 // Xcode 13.0
#        define CPP_CLANG 1200
#      elif __apple_build_version__ >= 12050022 // Xcode 12.5
#        define CPP_CLANG 1110
#      elif __apple_build_version__ >= 12000032 // Xcode 12.0
#        define CPP_CLANG 1000
#      elif __apple_build_version__ >= 11030032 // Xcode 11.4
#        define CPP_CLANG 900
#      elif __apple_build_version__ >= 11000033 // Xcode 11.0
#        define CPP_CLANG 800
#      else
#        error ""
#      endif
#    else

#      define CPP_CLANG ((__clang_major__ * 100) + __clang_minor__)
#    endif
#    define CPP_CLANG_ONLY CPP_CLANG
#    if __has_builtin(__builtin_assume)
#      define ASSUME_IMPL(expr)   __builtin_assume(expr)
#    else
#      define ASSUME_IMPL(expr)  if (expr){} else __builtin_unreachable()
#    endif
#    define UNREACHABLE_IMPL() __builtin_unreachable()
#    if !defined(__has_extension)
#      
#      define __has_extension __has_feature
#    endif
#  else

#    define CPP_GNU_ONLY CPP_GNU
#    if CPP_GNU >= 405
#      define ASSUME_IMPL(expr)  if (expr){} else __builtin_unreachable()
#      define UNREACHABLE_IMPL() __builtin_unreachable()
#      define DECL_DEPRECATED_X(text) __attribute__ ((__deprecated__(text)))
#    endif
#  endif

#  ifdef OS_WIN
#    define DECL_EXPORT     __declspec(dllexport)
#    define DECL_IMPORT     __declspec(dllimport)
#  else
#    define DECL_EXPORT_OVERRIDABLE __attribute__((visibility("default"), weak))
#    ifdef USE_PROTECTED_VISIBILITY
#      define DECL_EXPORT     __attribute__((visibility("protected")))
#    else
#      define DECL_EXPORT     __attribute__((visibility("default")))
#    endif
#    define DECL_IMPORT     __attribute__((visibility("default")))
#    define DECL_HIDDEN     __attribute__((visibility("hidden")))
#  endif

# else 
# endif

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
# if __has_include(<version>)
#  include <version>
# endif
# endif

#define DO_PRAGMA(text)                      _Pragma(#text)
#if defined(CPP_MSVC) && !defined(CPP_CLANG)
#  undef DO_PRAGMA                           /* not needed */
#  define WARNING_PUSH                       __pragma(warning(push))
#  define WARNING_POP                        __pragma(warning(pop))
#  define WARNING_DISABLE_MSVC(number)       __pragma(warning(disable: number))
#  define WARNING_DISABLE_INTEL(number)
#  define WARNING_DISABLE_CLANG(text)
#  define WARNING_DISABLE_GCC(text)
#  define WARNING_DISABLE_DEPRECATED         WARNING_DISABLE_MSVC(4996)
#  define WARNING_DISABLE_FLOAT_COMPARE
#  define WARNING_DISABLE_INVALID_OFFSETOF
#elif defined(CPP_CLANG)
#  define WARNING_PUSH                       DO_PRAGMA(clang diagnostic push)
#  define WARNING_POP                        DO_PRAGMA(clang diagnostic pop)
#  define WARNING_DISABLE_CLANG(text)        DO_PRAGMA(clang diagnostic ignored text)
#  define WARNING_DISABLE_GCC(text)
#  define WARNING_DISABLE_INTEL(number)
#  define WARNING_DISABLE_MSVC(number)
#  define WARNING_DISABLE_DEPRECATED         WARNING_DISABLE_CLANG("-Wdeprecated-declarations")
#  define WARNING_DISABLE_FLOAT_COMPARE      WARNING_DISABLE_CLANG("-Wfloat-equal")
#  define WARNING_DISABLE_INVALID_OFFSETOF   WARNING_DISABLE_CLANG("-Winvalid-offsetof")
#elif defined(CPP_GNU) && (__GNUC__ * 100 + __GNUC_MINOR__ >= 406)
#  define WARNING_PUSH                       DO_PRAGMA(GCC diagnostic push)
#  define WARNING_POP                        DO_PRAGMA(GCC diagnostic pop)
#  define WARNING_DISABLE_GCC(text)          DO_PRAGMA(GCC diagnostic ignored text)
#  define WARNING_DISABLE_CLANG(text)
#  define WARNING_DISABLE_INTEL(number)
#  define WARNING_DISABLE_MSVC(number)
#  define WARNING_DISABLE_DEPRECATED         WARNING_DISABLE_GCC("-Wdeprecated-declarations")
#  define WARNING_DISABLE_FLOAT_COMPARE      WARNING_DISABLE_GCC("-Wfloat-equal")
#  define WARNING_DISABLE_INVALID_OFFSETOF   WARNING_DISABLE_GCC("-Winvalid-offsetof")
#else       // ��� ��������� �����������, GCC < 4.6 � MSVC < 2008
#  define WARNING_DISABLE_GCC(text)
#  define WARNING_PUSH
#  define WARNING_POP
#  define WARNING_DISABLE_INTEL(number)
#  define WARNING_DISABLE_MSVC(number)
#  define WARNING_DISABLE_CLANG(text)
#  define WARNING_DISABLE_GCC(text)
#  define WARNING_DISABLE_DEPRECATED
#  define WARNING_DISABLE_FLOAT_COMPARE
#  define WARNING_DISABLE_INVALID_OFFSETOF
#endif


# ifdef __cplusplus
#if !defined(PROCESSOR_X86)
#  undef COMPILER_SUPPORTS_SSE2
#  undef COMPILER_SUPPORTS_SSE3
#  undef COMPILER_SUPPORTS_SSSE3
#  undef COMPILER_SUPPORTS_SSE4_1
#  undef COMPILER_SUPPORTS_SSE4_2
#  undef COMPILER_SUPPORTS_AVX
#  undef COMPILER_SUPPORTS_AVX2
#  undef COMPILER_SUPPORTS_F16C
#endif


#ifdef CPP_MSVC
#  define never_inline __declspec(noinline)
#  define always_inline __forceinline
#elif defined(CPP_GNU)
#  define never_inline __attribute__((noinline))
#  define always_inline inline __attribute__((always_inline))
#else
#  define never_inline
#  define always_inline inline
#endif

#if !defined(CPP_WARNINGS)
#  define NO_WARNINGS
#endif
#if defined(NO_WARNINGS)
#  if defined(CPP_MSVC)
WARNING_DISABLE_MSVC(4828) /* ���� �������� ����, ������������ �� �������� xx, ������� �������� ������������ � ������� �������� ���������. */
WARNING_DISABLE_MSVC(4251) /* ����� 'type' ������ ����� dll-���������, ������� ����� �������������� ��������� ������ 'type2' */
WARNING_DISABLE_MSVC(4244) /* �������������� �� 'type1' � 'type2', ��������� ������ ������ */
WARNING_DISABLE_MSVC(4275) /* ������������� ����� ������, ��������� �� DLL-����������, ������������ � �������� �������� ��� �������������� ����� ������ DLL-���������� */
WARNING_DISABLE_MSVC(4514) /* ������� ���������� �������, �� ������� ��� ������ */
WARNING_DISABLE_MSVC(4800) /* 'type' : �������������� ���������� �������� bool 'true' ��� 'false' (�������������� � ������������������) */
WARNING_DISABLE_MSVC(4097) /* typedef-��� 'identifier1' ������������ ��� ������� ����� ������ 'identifier2' */
WARNING_DISABLE_MSVC(4706) /* ���������� � �������� ��������� */
WARNING_DISABLE_MSVC(4355) /* 'this' : ������������ � ������ ��������������� ������� ��������� */
WARNING_DISABLE_MSVC(4710) /* ������� �� �������� */
WARNING_DISABLE_MSVC(4530) /* ������������ ���������� ���������� C++, �� ��������� �������� �� ��������. ������� /EHsc */
WARNING_DISABLE_MSVC(4006) 
#  elif defined(CPP_BOR)
#    pragma option -w-inl
#    pragma option -w-aus
#    pragma warn -inl
#    pragma warn -pia
#    pragma warn -ccc
#    pragma warn -rch
#    pragma warn -sig
#  endif
#endif

# endif // __cplusplus