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

#ifndef STDCALL
#  define STDCALL       __stdcall
#endif

#ifndef __CDECL
#  define __CDECL       __cdecl
#endif

#elif defined(CPP_GNU)

#ifndef STDCALL
#  define STDCALL       __attribute__((__stdcall__))
#endif

#ifndef __CDECL
#  define __CDECL       __attribute__((__cdecl__))
#endif

#else 

#ifndef STDCALL
#  define STDCALL      
#endif

#ifndef __CDECL
#  define __CDECL       
#endif 

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

#if defined(CPP_GNU) && !defined(__INSURE__) && !defined(ATTRIBUTE_FORMAT_PRINTF)
#  if defined(CPP_MINGW) && !defined(CPP_CLANG)
#    define ATTRIBUTE_FORMAT_PRINTF(A, B) \
         __attribute__((format(gnu_printf, (A), (B))))
#  else
#    define ATTRIBUTE_FORMAT_PRINTF(A, B) \
         __attribute__((format(printf, (A), (B))))
#  endif
#else

#ifndef ATTRIBUTE_FORMAT_PRINTF
#  define ATTRIBUTE_FORMAT_PRINTF(A, B)
#endif

#endif

#ifdef CPP_MSVC

#ifndef never_inline
#  define never_inline          __declspec(noinline)
#endif

#ifndef always_inline
#  define always_inline         __forceinline
#endif

#elif defined(CPP_GNU)

#ifndef never_inline
#  define never_inline          __attribute__((noinline))
#endif

#ifndef always_inline
#  define always_inline         inline __attribute__((always_inline))
#endif 

#else
#  define never_inline
#  define always_inline         inline
#endif

#ifndef ALWAYS_INLINE
#  define ALWAYS_INLINE         always_inline
#endif

#ifndef NEVER_INLINE
#  define NEVER_INLINE          never_inline
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
#  ifndef NODISCARD
#    define NODISCARD [[nodiscard]]
#  endif
#else
#  define NODISCARD
#endif

#ifndef __has_cpp_attribute

#ifndef likely_attribute
#  define likely_attribute
#endif

#ifndef unlikely_attribute
#  define unlikely_attribute
#endif

#elif __has_cpp_attribute(likely) >= 201803L && __has_cpp_attribute(unlikely) >= 201803L

#ifndef likely_attribute
#  define likely_attribute   [[likely]]
#endif 

#ifndef unlikely_attribute
#  define unlikely_attribute [[unlikely]]
#endif

#elif defined(CPP_CLANG)

#ifndef likely_attribute
#  define likely_attribute   [[__likely__]]
#endif

#ifndef unlikely_attribute
#  define unlikely_attribute [[__unlikely__]]
#endif 

#else

#ifndef likely_attribute
#  define likely_attribute
#endif 

#ifndef unlikely_attribute
#  define unlikely_attribute
#endif 

#endif

#ifndef LIKELY_ATTRIBUTE
#  define LIKELY_ATTRIBUTE likely_attribute
#endif

#ifndef UNLIKELY_ATTRIBUTE
#  define UNLIKELY_ATTRIBUTE unlikely_attribute
#endif

#ifndef __has_cpp_attribute && !defined(maybe_unused_attribute)
#  define maybe_unused_attribute
#elif __has_cpp_attribute(maybe_unused) >= 201603L && !defined(maybe_unused_attribute)
#  define maybe_unused_attribute   [[maybe_unused]]
#else

#ifndef maybe_unused_attribute
#  define maybe_unused_attribute
#endif

#endif

#ifndef MAYBE_UNUSED_ATTRIBUTE
#  define MAYBE_UNUSED_ATTRIBUTE maybe_unused_attribute
#endif

#ifndef __has_cpp_attribute && !defined(nodiscard_msg)
#  define nodiscard_msg(_Msg)
#elif __has_cpp_attribute(nodiscard) >= 201907L && !defined(nodiscard_msg)
#  define nodiscard_msg(_Msg) [[nodiscard(_Msg)]]
#elif __has_cpp_attribute(nodiscard) >= 201603L && !defined(nodiscard_msg)
#  define nodiscard_msg(_Msg) [[nodiscard]]
#else

#ifndef nodiscard_msg
#  define nodiscard_msg(_Msg)
#endif

#endif

#ifndef NODISCARD_MSG
#  define NODISCARD_MSG nodiscard_msg
#endif

#ifndef __has_cpp_attribute

#ifndef nodiscard_ctor
#  define nodiscard_ctor
#endif

#ifndef nodiscard_ctor_msg
#  define nodiscard_ctor_msg(_Msg)
#endif

#elif __has_cpp_attribute(nodiscard) >= 201907L

#ifndef nodiscard_ctor
#  define nodiscard_ctor           NODISCARD
#endif

#ifndef nodiscard_ctor_msg
#  define nodiscard_ctor_msg(_Msg) NODISCARD_MSG(_Msg)
#endif

#else

#ifndef nodiscard_ctor
#  define nodiscard_ctor
#endif 

#ifndef nodiscard_ctor_msg
#  define nodiscard_ctor_msg(_Msg)
#endif 

#endif


#ifndef NODISCARD_CTOR
#  define NODISCARD_CTOR nodiscard_ctor
#endif

#ifndef NODISCARD_CTOR_MSG
#  define NODISCARD_CTOR_MSG nodiscard_ctor_msg
#endif

#ifndef unused
#  define unused(x)                             ((void)(x))
#endif 

#ifndef UNUSED
#  define UNUSED                                unused
#endif

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

#if defined(CPP_MSVC) && !defined(declare_memory_allocator)
#  define declare_memory_allocator      __declspec(allocator)
#elif defined (CPP_GNU) && !defined(declare_memory_allocator)
#  if CPP_GNU > 310
#    define declare_memory_allocator    __attribute__((malloc))
#  endif
#else 

#ifndef declare_memory_allocator
#  define declare_memory_allocator
#endif 

#endif

#ifndef DECLARE_MEMORY_ALLOCATOR
#  define DECLARE_MEMORY_ALLOCATOR declare_memory_allocator
#endif

#ifndef BASE_HAS_CXX17
#  if __cplusplus >= 201703L
#    define BASE_HAS_CXX17 1
#  else
#    define BASE_HAS_CXX17 0
#  endif
#endif

#ifndef BASE_HAS_CXX20
#  if BASE_HAS_CXX17 && __cplusplus >= 202002L
#    define BASE_HAS_CXX20 1
#  else
#    define BASE_HAS_CXX20 0
#  endif
#endif

#ifndef constexpr_cxx20
#  if BASE_HAS_CXX20
#    define constexpr_cxx20 constexpr
#  else
#  ifndef 
#    define constexpr_cxx20 
#  endif
#endif

#ifndef CONSTEXPR_CXX20
#  define CONSTEXPR_CXX20 constexpr_cxx20
#endif

#ifndef base_restrict
#  if defined(CPP_MSVC)
#    define base_restrict  __declspec(restrict)
#  elif defined(CPP_GNU) || defined (CPP_CLANG)
#    define base_restrict  __restrict__ 
#  endif 
#endif

#ifndef RESTRICT
#  define RESTRICT base_restrict
#endif

#ifndef restrict
#  define restrict base_restrict
#endif

// Warnings

#ifndef NODISCARD_RETURN_RAW_PTR
#  define NODISCARD_RETURN_RAW_PTR \
        NODISCARD_MSG("This function allocates memory and returns a raw pointer. " \
                   "Discarding the return value will cause a memory leak.")
#else 
#  define NODISCARD_RETURN_RAW_PTR 
#endif

#endif // __cplusplus