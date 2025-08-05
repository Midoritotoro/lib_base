#pragma once 

#include <base/core/arch/CompilerDetection.h>


// Clang attributes
// https://clang.llvm.org/docs/AttributeReference.html#always-inline-force-inline

// Msvc attributes
// https://learn.microsoft.com/en-us/cpp/cpp/declspec?view=msvc-170
// Msvc SAL
// https://learn.microsoft.com/en-us/cpp/code-quality/using-sal-annotations-to-reduce-c-cpp-code-defects?view=msvc-170

// Gcc attributes
// https://ohse.de/uwe/articles/gcc-attributes.html and https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html


base_disable_warning_msvc(4067)

// #define base_use_protected_visibility

#ifdef __cplusplus 


#if defined(base_cpp_msvc) || defined(base_cpp_clang)
#  include <stdnoreturn.h> 
#endif // defined(base_cpp_msvc) || defined(base_cpp_clang)

#if defined(__cpp_conditional_explicit)
#  if !defined(base_implicit)
#    define base_implicit explicit(false)
#  else
#    define base_implicit
#  endif // base_implicit
#endif // defined(__cpp_conditional_explicit)


#if !defined(base_fastcall)
#  if defined(PROCESSOR_X86_32)
#    if defined(base_cpp_gnu) || defined(base_cpp_clang)
#      define base_fastcall __attribute__((regparm(3)))
#    elif defined(base_cpp_msvc)
#      define base_fastcall __fastcall
#    else
#      define base_fastcall
#    endif // defined(base_cpp_gnu) || defined(base_cpp_msvc) || defined(base_cpp_clang)
#  else
#    define base_fastcall
#  endif // defined(PROCESSOR_X86_32)
#endif // !defined(base_fastcall)


#if __has_cpp_attribute(clang::fallthrough)
#    define base_fallthrough() [[clang::fallthrough]]
#elif __has_cpp_attribute(gnu::fallthrough)
#    define base_fallthrough() [[gnu::fallthrough]]
#elif __has_cpp_attribute(fallthrough)
#  define base_fallthrough() [[fallthrough]]
#endif // __has_cpp_attribute(clang::fallthrough) || __has_cpp_attribute(gnu::fallthrough) || __has_cpp_attribute(fallthrough)


#if !defined(base_fallthrough)
#  if defined(base_cpp_gnu)
#    define base_fallthrough() __attribute__((base_fallthrough))
#  else
#    define base_fallthrough() (void)0
#  endif // base_cpp_gnu
#endif // !defined(base_fallthrough)


#if !defined(base_stdcall)
#  if defined(base_cpp_msvc) || defined(base_cpp_clang)
#    define base_stdcall         __stdcall
#  elif defined(base_cpp_gnu)
#    define base_stdcall        __attribute__((__stdcall__))
#  else
#    define base_stdcall        
#  endif // defined(base_cpp_msvc) || defined(base_cpp_clang) || defined(base_cpp_gnu)
#endif // !defined(base_stdcall)

#if !defined(base_cdecl)
#  if defined(base_cpp_msvc) || defined(base_cpp_clang)
#    define base_cdecl          __cdecl
#  elif defined(base_cpp_gnu)
#    define base_cdecl          __attribute__((__cdecl__))
#  else
#    define base_cdecl        
#  endif // defined(base_cpp_msvc) || defined(base_cpp_clang) || defined(base_cpp_gnu)
#endif // !defined(base_cdecl)


#if !defined(base_noreturn)
#  if defined(base_cpp_gnu)
#    define base_noreturn        __attribute__((__noreturn__))
#  elif defined(base_cpp_msvc) || defined(base_cpp_clang)
#    define base_noreturn       __declspec(noreturn)
#  else 
#    define base_noreturn       
#  endif // defined(base_cpp_gnu) || defined(base_cpp_msvc) || defined(base_cpp_clang)
#endif // !defined(base_noreturn)


#if !defined(base_decl_export)
#  if defined(base_cpp_msvc) || defined(base_cpp_clang)
#    define base_decl_export            __declspec(dllexport)
#  elif defined(base_cpp_gnu)
#    if defined(base_os_windows)
#      define base_decl_export          __declspec(dllexport)
#    else
#      if defined(base_use_protected_visibility)
#         define base_decl_export         __attribute__((visibility("protected")))
#      else
#         define base_decl_export         __attribute__((visibility("default")))
#      endif // defined(base_use_protected_visibility)
#    endif // defined(base_os_windows)
#  else
#    define base_decl_export    
#  endif // defined(base_cpp_gnu) || defined(base_cpp_msvc) || defined(base_cpp_clang)
#endif // !defined(base_decl_export)


#if !defined(base_decl_export_overridable)
#  if defined(base_cpp_msvc)
#    define base_decl_export_overridable 
#  elif defined(base_cpp_gnu) && !defined(base_os_windows)
#    define base_decl_export_overridable __attribute__((visibility("default"), weak)) 
#  else
#    define base_decl_export_overridable
#  endif // defined(base_cpp_msvc) || (defined(base_cpp_gnu) && !defined(base_os_windows))
#endif // !defined(base_decl_export_overridable)


#if !defined(base_decl_import)
#  if defined(base_cpp_msvc) || defined(base_cpp_clang)
#    define base_decl_import        __declspec(dllimport)
#  elif defined(base_cpp_gnu)
#    define base_decl_import        __attribute__((visibility("default")))
#  else
#    define base_decl_import
#  endif // defined(base_cpp_msvc) || defined(base_cpp_clang) || defined(base_cpp_gnu)
#endif // !defined(base_decl_import)


#if !defined(base_decl_hidden)
#  if defined(base_cpp_gnu)
#    define base_decl_hidden        __attribute__((visibility("hidden")))
#  elif defined(base_cpp_clang)
#    define base_decl_hidden        __attribute__((__visibility__("hidden")))
#  else
#    define base_decl_hidden                
#  endif // defined(base_cpp_gnu) || defined(base_cpp_clang)
#endif // !defined(base_decl_hidden)


#if !defined(base_decl_unused)
#  if defined(base_cpp_gnu)
#    define base_decl_unused        __attribute__((__unused__))
#  elif defined(base_cpp_clang)
#    define base_decl_unused        __attribute__((unused))
#  else
#    define base_decl_unused    
#  endif // defined(base_cpp_gnu) || defined(base_cpp_clang)
#endif // !defined(base_decl_unused)


#if !defined(base_func_info)
#  if defined(base_cpp_msvc)
#    define base_func_info          __FUNCSIG__
#  elif defined(base_cpp_gnu)
#    define base_func_info          __PRETTY_FUNCTION__
#  else
#    define base_func_info          __FILE__ ":" stringify(__LINE__)
#  endif // defined(base_cpp_msvc) || defined(base_cpp_gnu)
#endif // !defined(base_func_info)


#if !defined(base_decl_pure_function)
#  if defined(base_cpp_gnu) || defined(base_cpp_clang)
#    define base_decl_pure_function __attribute__((pure))
#  else
#    define base_decl_pure_function
#  endif // defined(base_cpp_gnu) || defined(base_cpp_clang)
#endif // !defined(base_decl_pure_function)


#if !defined(base_decl_const_function)
#  if defined(base_cpp_gnu) || defined(base_cpp_clang)
#    define base_decl_const_function __attribute__((const))
#  elif defined(base_cpp_msvc)
#    define base_decl_const_function __declspec(noalias)
#  else
#    define base_decl_const_function
#  endif // defined(base_cpp_gnu) || defined(base_cpp_clang) || defined(base_cpp_msvc)
#endif // !defined(base_decl_const_function)


#if !defined(base_decl_cold_function)
#  if defined(base_cpp_gnu) || defined(base_cpp_clang)
#    define base_decl_cold_function __attribute__((cold))
#  else 
#    define base_decl_cold_function
#  endif // defined(base_cpp_gnu) || defined(base_cpp_clang)
#endif // !defined(base_decl_cold_function)


#if !defined(base_attribute_format_printf)
#  if defined(base_cpp_gnu) && !defined(__INSURE__)
#    if defined(base_cpp_mingw) && !defined(base_cpp_clang)
#      define base_attribute_format_printf(A, B) __attribute__((format(gnu_printf, (A), (B))))
#    else
#      define base_attribute_format_printf(A, B) __attribute__((format(printf, (A), (B))))
#    endif // defined(base_cpp_mingw) && !defined(base_cpp_clang)
#  else
#    define base_attribute_format_printf(A, B)
#  endif // defined(base_cpp_gnu) && !defined(__INSURE__)
#endif // !defined(base_attribute_format_printf)


#if !defined(base_never_inline)
#  if defined(base_cpp_msvc) || defined(base_cpp_clang)
#     define never_inline __declspec(noinline)
#  elif defined(base_cpp_gnu) 
#    define never_inline __attribute__((noinline))
#  else 
#    define never_inline 
#  endif // defined(base_cpp_msvc) || defined(base_cpp_clang) || defined(base_cpp_gnu)
#endif // !defined(base_never_inline)


#if !defined(base_always_inline)
#  if defined(base_cpp_msvc) || defined(base_cpp_clang)
#    define base_always_inline __forceinline
#  elif defined(base_cpp_gnu)
#    define base_always_inline inline __attribute__((always_inline))
#  else 
#    define base_always_inline inline
#  endif // defined(base_cpp_msvc) || defined(base_cpp_clang) || defined(base_cpp_gnu)
#endif // !defined(base_always_inline)


#if !defined(base_has_nodiscard)
#  if !defined(__has_cpp_attribute)
#    define base_has_nodiscard 0
#  elif __has_cpp_attribute(nodiscard) >= 201603L
#    define base_has_nodiscard 1
#  else
#    define base_has_nodiscard 0
#  endif
#endif // !defined(base_has_nodiscard)


#if !defined(base_nodiscard)
#  if base_has_nodiscard
#    define base_nodiscard  [[nodiscard]]
#  elif defined(base_cpp_gnu)
#    define base_nodiscard  __attribute__((__warn_unused_result__))
#  elif defined(base_cpp_clang)
#    define base_nodiscard  __attribute__(warn_unused_result)
#  elif defined(base_cpp_msvc)
#    define base_nodiscard  _Check_return_
#  else
#    define base_nodiscard
#  endif // base_has_nodiscard || defined(base_cpp_gnu) || defined(base_cpp_clang) || defined(base_cpp_msvc)
#endif // !defined(base_nodiscard)


#if !defined(base_likely_attribute)
#  if defined(__has_cpp_attribute) && __has_cpp_attribute(likely) >= 201803L
#    define base_likely_attribute [[likely]]
#  else
#    define base_likely_attribute
#  endif
#endif // !defined(base_likely_attribute)


#if !defined(base_unlikely_attribute)
#  if defined(__has_cpp_attribute) && __has_cpp_attribute(unlikely) >= 201803L
#    define base_unlikely_attribute [[unlikely]]
#  else
#    define base_unlikely_attribute
#  endif
#endif // !defined(base_unlikely_attribute)


#ifndef __has_cpp_attribute && !defined(maybe_unused_attribute)
#  define maybe_unused_attribute
#elif __has_cpp_attribute(maybe_unused) >= 201603L && !defined(maybe_unused_attribute)
#  define maybe_unused_attribute   [[maybe_unused]]
#else

#ifndef maybe_unused_attribute
#  define maybe_unused_attribute
#endif

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


#ifndef unused
#  define unused(x)                             ((void)(x))
#endif 

#ifndef UNUSED
#  define UNUSED                                unused
#endif

#if defined (CPP_GNU) || defined (CPP_CLANG)

#ifndef LIKELY
#  define LIKELY(p)                             __builtin_expect(!!(p), 1)
#endif

#ifndef UNLIKELY
#  define UNLIKELY(p)                           __builtin_expect(!!(p), 0)
#endif

#ifndef UNREACHABLE
#  define UNREACHABLE()                         __builtin_unreachable()
#endif

#elif defined(base_cpp_msvc)

#ifndef LIKELY
#  define LIKELY(p)                             (!!(p))
#endif

#ifndef UNLIKELY
#  define UNLIKELY(p)                           (!!(p))
#endif

#ifndef UNREACHABLE
#  define UNREACHABLE()                         (__assume(0))
#endif

#else

#ifndef LIKELY
#  define LIKELY(p)                             (!!(p))
#endif

#ifndef UNLIKELY
#  define UNLIKELY(p)                           (!!(p))
#endif

#ifndef UNREACHABLE
#  define UNREACHABLE()                         ((void)0)
#endif

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

#if defined(base_cpp_msvc) && !defined(declare_memory_allocator)
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

#ifndef BASE_HAS_CXX23
#  if BASE_HAS_CXX20 && __cplusplus > 202002L
#    define BASE_HAS_CXX23 1
#  else
#    define BASE_HAS_CXX23 0
#  endif
#endif

#ifndef constexpr_cxx20
#  if BASE_HAS_CXX20
#    define constexpr_cxx20 constexpr
#  else
#    define constexpr_cxx20 
#  endif
#endif

#ifndef CONSTEXPR_CXX20
#  define CONSTEXPR_CXX20 constexpr_cxx20
#endif

#ifndef base_restrict
#  if defined(base_cpp_msvc)
#    define base_restrict  __declspec(restrict)
#  elif defined(CPP_GNU) || defined (CPP_CLANG)
#    define base_restrict  __restrict__ 
#  endif 
#endif

#ifndef BASE_RESTRICT
#  define BASE_RESTRICT base_restrict
#endif

#ifdef CPP_CLANG
#  define clang_constexpr_cxx20 constexpr_cxx20
#else
#  define clang_constexpr_cxx20 
#endif

#ifndef CLANG_CONSTEXPR_CXX20
#  define CLANG_CONSTEXPR_CXX20 clang_constexpr_cxx20
#endif

#if defined(OS_WIN) && defined(base_cpp_msvc)
#  define BASE_GUARDOVERFLOW __declspec(guard(overflow))
#else
#  define BASE_GUARDOVERFLOW 
#endif


// The "noalias" attribute tells the compiler optimizer that pointers going into these hand-vectorized algorithms
// won't be stored beyond the lifetime of the function, and that the function will only reference arrays denoted by
// those pointers. The optimizer also assumes in that case that a pointer parameter is not returned to the caller via
// the return value, so functions using "noalias" must usually return void. This attribute is valuable because these
// functions are in native code objects that the compiler cannot analyze. In the absence of the noalias attribute, the
// compiler has to assume that the denoted arrays are "globally address taken", and that any later calls to
// unanalyzable routines may modify those arrays.

#ifndef SIZEOF_TO_BITS
#  define SIZEOF_TO_BITS(_Type) (sizeof(_Type) * 8)
#endif 

#ifndef sizeof_to_bits
#  define sizeof_to_bits        SIZEOF_TO_BITS
#endif

#if defined(PROCESSOR_X86_64) \
    || defined(PROCESSOR_ARM) \
    || (PROCESSOR_ARM == 8) // x64 ARM

#  if defined(OS_WIN) && defined(base_cpp_msvc)
      #define BASE_UNALIGNED __unaligned
#  else
      #define BASE_UNALIGNED
#  endif

#else 
#  define BASE_UNALIGNED
#endif

#ifdef base_cpp_msvc

#  ifndef BASE_MSVC_COMSTEXPR
#    ifdef _MSVC_CONSTEXPR_ATTRIBUTE
#      define BASE_MSVC_COMSTEXPR [[msvc::constexpr]]
#    else
#      define BASE_MSVC_COMSTEXPR
#    endif
#  endif

#else 

#  define BASE_MSVC_COMSTEXPR 

#endif

#ifdef CPP_MSVC 

#  define BASE_DECLARE_GPU      restrict(amp,cpu)
#  define BASE_DECLARE_GPU_ONLY restrict(amp)
#  define BASE_DECLARE_CPU_ONLY restrict(cpu)

#else 

#  define BASE_DECLARE_GPU     
#  define BASE_DECLARE_GPU_ONLY 
#  define BASE_DECLARE_CPU_ONLY

#endif

/* GCC */

#if defined(CPP_GNU) || defined(CPP_CLANG)

#  ifndef BASE_ATTRIBUTE_MAY_ALIAS
#    define BASE_ATTRIBUTE_MAY_ALIAS __attribute__((__may_alias__))
#  endif

#  ifndef BASE_ATTRIBUTE_ALIGNED
#    define BASE_ATTRIBUTE_ALIGNED(size) __attribute__((aligned(size)))
#  endif

#  ifndef BASE_ATTRIBUTE_VECTOR_SIZE
#    define BASE_ATTRIBUTE_VECTOR_SIZE(size) __attribute__((__vector_size__(size)))
#  endif

#endif // !defined(CPP_GNU) && !defined(CPP_CLANG)

#if defined(CPP_GNU) || defined(CPP_CLANG)
#  if !defined(BASE_ATTRIBUTE_VECTOR_SIZE_MAY_ALIAS)
#    define BASE_ATTRIBUTE_VECTOR_SIZE_MAY_ALIAS(size) __attribute__((__vector_size__(size), __may_alias__))
#endif
#else 
#  define BASE_ATTRIBUTE_VECTOR_SIZE_MAY_ALIAS(size) 
#endif

#if defined(CPP_GNU) || defined(CPP_CLANG)
#  if !defined(BASE_ATTRIBUTE_VECTOR_SIZE_ALIGNED)
#    define BASE_ATTRIBUTE_VECTOR_SIZE_ALIGNED(size) __attribute__((__vector_size__(size), __aligned__(size)))
#endif
#else 
#  define BASE_ATTRIBUTE_VECTOR_SIZE_ALIGNED(size) 
#endif

#if defined(CPP_GNU) || defined(CPP_CLANG)
#  if !defined(BASE_ATTRIBUTE_VECTOR_SIZE_ALIGNED_MAY_ALIAS)
#    define BASE_ATTRIBUTE_VECTOR_SIZE_ALIGNED_MAY_ALIAS(size) __attribute__((__vector_size__(size), __may_alias__, __aligned__(size)))
#endif
#else 
#  define BASE_ATTRIBUTE_VECTOR_SIZE_ALIGNED_MAY_ALIAS(size) 
#endif

#if defined(CPP_GNU) || defined(CPP_CLANG)
#  if !defined(BASE_ATTRIBUTE_VECTOR_SIZE_DIFFERENT_ALIGNED_MAY_ALIAS)
#    define BASE_ATTRIBUTE_VECTOR_SIZE_DIFFERENT_ALIGNED_MAY_ALIAS(size, align) __attribute__((__vector_size__(size), __may_alias__, __aligned__(align)))
#endif
#else 
#  define BASE_ATTRIBUTE_VECTOR_SIZE_DIFFERENT_ALIGNED_MAY_ALIAS(size, align) 
#endif

#if defined(CPP_GNU) || defined(CPP_CLANG)
#  if !defined(base_attribute_may_alias_aligned)
#    define base_attribute_may_alias_aligned(size) __attribute__((__may_alias__, __aligned__(size)))
#  endif // base_attribute_may_alias_aligned
#else // !defined(CPP_GNU) && !defined(CPP_CLANG)
#  define base_attribute_may_alias_aligned(size) 
#endif // base_attribute_may_alias_aligned


// Exceptions 

#ifndef base_try_begin
#  define base_try_begin try {
#endif // base_try_begin

#ifndef base_catch
#  define base_catch(x) \
    }             \
    catch (x) {
#endif // base_catch

#ifndef base_catch_all
#  define base_catch_all \
    }              \
    catch (...) {
#endif // base_catch_all

#ifndef base_catch_end 
#  define base_catch_end } 
#endif // base_catch_end

#ifndef base_reraise
#  define base_reraise    throw
#endif // base_reraise

#ifndef base_throw
#  define base_throw(...) throw(__VA_ARGS__)
#endif // base_throw


#ifndef base_asm_extern
#  if defined(__cplusplus)
#    define base_asm_extern extern "C"
#  else 
#    define base_asm_extern extern
#  endif // __cplusplus
#endif // base_asm_extern

// Warnings

#ifndef base_nodiscard_return_raw_ptr
#  define base_nodiscard_return_raw_ptr \
        base_nodiscard_msg("This function allocates memory and returns a raw pointer. " \
            "Discarding the return value will cause a memory leak.")
#endif // base_nodiscard_return_raw_ptr

#ifndef base_nodiscard_thread_ctor
#  define base_nodiscard_thread_ctor \
    base_nodiscard_ctor_msg("Creating a thread object without assigning it to a variable " \
        "may lead to unexpected behavior and resource leaks. Ensure " \
        "the thread is properly managed.")
#endif // base_nodiscard_thread_ctor

#ifndef base_nodiscard_remove_algorithm
#  define base_nodiscard_remove_algorithm \
        base_nodiscard_msg("The 'remove' and 'remove_if' algorithms return the iterator past the last element " \
            "that should be kept. You need to call container.erase(result, container.end()) afterwards. " \
            "In C++20, 'std::erase' and 'std::erase_if' are simpler replacements for these two steps.")
#endif // base_nodiscard_remove_algorithm

#endif // __cplusplus