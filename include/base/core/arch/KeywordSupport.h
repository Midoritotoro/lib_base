#pragma once 

#include <base/core/arch/CompilerDetection.h>

#ifdef __cplusplus 

#ifndef OUTOFLINE_TEMPLATE
#  define OUTOFLINE_TEMPLATE
#endif

#ifdef __cpp_conditional_explicit
    #define IMPLICIT explicit(false)
#else
    #define IMPLICIT
#endif

#ifdef PROCESSOR_X86_32
#  if defined(CPP_GNU)
#    define FASTCALL __attribute__((regparm(3)))
#  elif defined(CPP_MSVC)
#    define FASTCALL __fastcall
#  else
#    define FASTCALL
#  endif
#else
#  define FASTCALL
#endif

#if __has_cpp_attribute(clang::fallthrough)
#    define FALLTHROUGH() [[clang::fallthrough]]
#elif __has_cpp_attribute(gnu::fallthrough)
#    define FALLTHROUGH() [[gnu::fallthrough]]
#elif __has_cpp_attribute(fallthrough)
#  define FALLTHROUGH() [[fallthrough]]
#endif

#ifndef FALLTHROUGH
#  ifdef CPP_GNU
#    define FALLTHROUGH() __attribute__((fallthrough))
#  else
#    define FALLTHROUGH() (void)0
#  endif
#endif

#if defined(OS_WIN) && defined(CPP_MSVC)
#  define STDCALL       __stdcall
#  define __CDECL       __cdecl
#elif defined(CPP_GNU)
#  define STDCALL       __attribute__((__stdcall__))
#  define CDECL         __attribute__((__cdecl__))
#else 
#  define STDCALL       
#  define __CDECL       
#endif

#ifndef NORETURN
#  define NORETURN
#endif

#ifndef DECL_EXPORT
#  define DECL_EXPORT
#endif

#ifndef DECL_EXPORT_OVERRIDABLE
#  define DECL_EXPORT_OVERRIDABLE DECL_EXPORT
#endif

#ifndef DECL_IMPORT
#  define DECL_IMPORT
#endif

#ifndef DECL_HIDDEN
#  define DECL_HIDDEN
#endif

#ifndef DECL_UNUSED
#  define DECL_UNUSED
#endif

#ifndef DECL_UNUSED_MEMBER
#  define DECL_UNUSED_MEMBER
#endif

#ifndef FUNC_INFO
#  define FUNC_INFO __FILE__ ":" stringify(__LINE__)
#endif

#ifndef DECL_CF_RETURNS_RETAINED
#  define DECL_CF_RETURNS_RETAINED
#endif

#ifndef DECL_NS_RETURNS_AUTORELEASED
#  define DECL_NS_RETURNS_AUTORELEASED
#endif

#ifndef DECL_PURE_FUNCTION
#  define DECL_PURE_FUNCTION
#endif

#ifndef DECL_CONST_FUNCTION
#  define DECL_CONST_FUNCTION DECL_PURE_FUNCTION
#endif

#ifndef DECL_COLD_FUNCTION
#  define DECL_COLD_FUNCTION
#endif

#ifndef WEAK_OVERLOAD
#  define WEAK_OVERLOAD template <typename = void>
#endif

#if defined(CPP_GNU) && !defined(__INSURE__)
#  if defined(CPP_MINGW) && !defined(CPP_CLANG)
#    define ATTRIBUTE_FORMAT_PRINTF(A, B) \
         __attribute__((format(gnu_printf, (A), (B))))
#  else
#    define ATTRIBUTE_FORMAT_PRINTF(A, B) \
         __attribute__((format(printf, (A), (B))))
#  endif
#else
#  define ATTRIBUTE_FORMAT_PRINTF(A, B)
#endif

#ifdef CPP_MSVC
#  define never_inline          __declspec(noinline)
#  define always_inline         __forceinline
#elif defined(CPP_GNU)
#  define never_inline          __attribute__((noinline))
#  define always_inline         inline __attribute__((always_inline))
#else
#  define never_inline
#  define always_inline         inline
#endif

#ifndef HAS_NODISCARD
    #ifndef __has_cpp_attribute
        #define HAS_NODISCARD 0
    #elif __has_cpp_attribute(nodiscard) >= 201603L
        #define HAS_NODISCARD 1
    #else
        #define HAS_NODISCARD 0
    #endif
#endif // HAS_NODISCARD

#if HAS_NODISCARD
#  define NODISCARD [[nodiscard]]
#else
#  define NODISCARD
#endif

#ifndef __has_cpp_attribute
#  define LIKELY_ATTRIBUTE
#  define UNLIKELY_ATTRIBUTE
#elif __has_cpp_attribute(likely) >= 201803L && __has_cpp_attribute(unlikely) >= 201803L
#  define LIKELY_ATTRIBUTE   [[likely]]
#  define UNLIKELY_ATTRIBUTE [[unlikely]]
#elif defined(CPP_CLANG)
#  define LIKELY_ATTRIBUTE   [[__likely__]]
#  define UNLIKELY_ATTRIBUTE [[__unlikely__]]
#else
#  define LIKELY_ATTRIBUTE
#  define UNLIKELY_ATTRIBUTE
#endif


#ifndef __has_cpp_attribute
#  define MAYBE_UNUSED_ATTRIBUTE
#elif __has_cpp_attribute(maybe_unused) >= 201603L
#  define MAYBE_UNUSED_ATTRIBUTE   [[maybe_unused]]
#else
#  define MAYBE_UNUSED_ATTRIBUTE
#endif

#ifndef __has_cpp_attribute
#  define _NODISCARD_MSG(_Msg)
#elif __has_cpp_attribute(nodiscard) >= 201907L
#  define NODISCARD_MSG(_Msg) [[nodiscard(_Msg)]]
#elif __has_cpp_attribute(nodiscard) >= 201603L
#  define NODISCARD_MSG(_Msg) [[nodiscard]]
#else
#  define NODISCARD_MSG(_Msg)
#endif


#ifndef __has_cpp_attribute
#  define NODISCARD_CTOR
#  define NODISCARD_CTOR_MSG(_Msg)
#elif __has_cpp_attribute(nodiscard) >= 201907L
#  define NODISCARD_CTOR           NODISCARD
#  define NODISCARD_CTOR_MSG(_Msg) NODISCARD_MSG(_Msg)
#else
#  define NODISCARD_CTOR
#  define NODISCARD_CTOR_MSG(_Msg)
#endif

#define unused(x)                               ((void)(x))
#define UNUSED(x)                               ((void)(x))

#if defined(OS_WIN)
    #define aligned_malloc(size, alignment)		_aligned_malloc(size, alignment)
#else
    #define aligned_malloc(size, alignment)		malloc(size)
#endif

#if defined (OS_WIN)
    #define aligned_free(ptr)                   _aligned_free(ptr)
#else
    #define aligned_free(ptr)                   free(ptr)
#endif

#if defined (CPP_GNU) || defined (CPP_CLANG)

#  define LIKELY(p)                             __builtin_expect(!!(p), 1)
#  define UNLIKELY(p)                           __builtin_expect(!!(p), 0)
#  define UNREACHABLE()                         __builtin_unreachable()

#elif defined(CPP_MSVC)

#  define LIKELY(p)                             (!!(p))
#  define UNLIKELY(p)                           (!!(p))
#  define UNREACHABLE()                         (__assume(0))

#else

#  define LIKELY(p)                             (!!(p))
#  define UNLIKELY(p)                           (!!(p))
#  define UNREACHABLE()                         ((void)0)

#endif

#if __has_cpp_attribute(gnu::malloc)
#  if HAS_NODISCARD
#    define DECLARE_MALLOCLIKE [[nodiscard, gnu::malloc]]
#  else
#    define DECLARE_MALLOCLIKE [[gnu::malloc]]
#  endif
#else
#  define DECLARE_MALLOCLIKE NODISCARD
#endif

#if defined(CPP_MSVC)
#  define RESTRICT  __declspec(restrict)
#elif defined(CPP_GNU) || defined (CPP_CLANG)
#  define RESTRICT  __restrict__ 
#endif 

#endif // __cplusplus