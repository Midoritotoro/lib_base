#pragma once 

#include <base/core/arch/CompilerDetection.h>


// Clang attributes
// https://clang.llvm.org/docs/AttributeReference.html#always-inline-force-inline
// Clang builtins
// https://clang.llvm.org/docs/LanguageExtensions.html

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
// The "noalias" attribute tells the compiler optimizer that pointers going into these hand-vectorized algorithms
// won't be stored beyond the lifetime of the function, and that the function will only reference arrays denoted by
// those pointers. The optimizer also assumes in that case that a pointer parameter is not returned to the caller via
// the return value, so functions using "noalias" must usually return void. This attribute is valuable because these
// functions are in native code objects that the compiler cannot analyze. In the absence of the noalias attribute, the
// compiler has to assume that the denoted arrays are "globally address taken", and that any later calls to
// unanalyzable routines may modify those arrays.
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


#if !defined(base_maybe_unused_attribute)
#  if defined(__has_cpp_attribute) && __has_cpp_attribute(maybe_unused) >= 201603L
#    define base_maybe_unused_attribute     [[maybe_unused]]
#  elif defined(base_cpp_gnu) || defined(base_cpp_clang)
#    define base_maybe_unused_attribute     __attribute__(unused)
#  endif // defined(__has_cpp_attribute) && __has_cpp_attribute(maybe_unused) >= 201603L
#endif // !defined(base_maybe_unused_attribute)


#if !defined(base_nodiscard_with_warning)
#  if defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard) >= 201907L
#    define base_nodiscard_with_warning(message)    [[nodiscard(message)]]
#  elif defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard) >= 201603L
#    define base_nodiscard_with_warning(message)    base_nodiscard
#  else
#    define base_nodiscard_with_warning(message)
#  endif //     defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard) >= 201907L 
         //  || defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard) >= 201603L
#endif // !defined(base_nodiscard_with_warning)


#if !defined(base_nodiscard_constructor)
// https://open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1771r1.pdf
#  if defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard) >= 201907L
#    define base_nodiscard_constructor base_nodiscard
#  else
#    define base_nodiscard_constructor
#  endif // defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard) >= 201907L
#endif // !defined(base_nodiscard_constructor)


#if !defined(base_nodiscard_constructor_with_warning)
#  if defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard) >= 201907L
#    define base_nodiscard_constructor_with_warning(message) base_nodiscard_with_warning(message)
#  else
#    define base_nodiscard_constructor_with_warning(message) 
#  endif // defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard) >= 201907L
#endif // !defined(base_nodiscard_constructor_with_warning)


#if !defined(base_unused)
#  define base_unused(variable) ((void)(variable))
#endif // !defined(base_unused)


#if !defined(base_likely)
#  if defined(base_cpp_gnu) || defined(base_cpp_clang)
#    define base_likely(expression) __builtin_expect(!!(expression), true)
#  elif defined(base_cpp_msvc) && defined(__has_cpp_attribute) && __has_cpp_attribute(likely) >= 201803L
#    define base_likely(expression)                     \
       (                                                \
         ([](bool value){                               \
           switch (value) {                             \
             [[unlikely]] case true: return true;       \
             [[likely]] case false: return false;       \
         }                                              \
       })(expression))
#  else
#    define base_likely(expression) (!!(expression))
#  endif // defined(base_cpp_gnu) || defined(base_cpp_clang)
#endif // !defined(base_likely)


#if !defined(base_unlikely)
#  if defined(base_cpp_gnu) || defined(base_cpp_clang)
#    define base_unlikely(expression) __builtin_expect(!!(expression), false)
#  elif defined(base_cpp_msvc) && defined(__has_cpp_attribute) && __has_cpp_attribute(unlikely) >= 201803L
#    define base_unlikely(expression)                   \
       (                                                \
         ([](bool value){                               \
           switch (value) {                             \
             [[likely]] case true: return true;         \
             [[unlikely]] case false: return false;     \
         }                                              \
       })(expression))
#  else
#    define base_unlikely(expression) (!!(expression))
#  endif // defined(base_cpp_gnu) || defined(base_cpp_clang)
#endif // !defined(base_unlikely)


#if !defined(base_unreachable)
#  if defined(base_cpp_gnu) || defined(base_cpp_clang)
#    define base_unreachable() __builtin_unreachable()
#  elif defined(base_cpp_msvc)
#    define base_unreachable() (__assume(0))
#  else
#    define base_unreachable() ((void)0)
#  endif // defined(base_cpp_gnu) || defined(base_cpp_clang) || defined(base_cpp_msvc)
#endif // !defined(base_unreachable)


#if !defined(base_restrict)
#  if defined(base_cpp_msvc) || defined (base_cpp_clang)
#    define base_restrict   __declspec(restrict)
#  elif defined(base_cpp_gnu)
#    define base_restrict   __restrict  
#  endif // defined(base_cpp_msvc) || defined (base_cpp_clang) || defined(base_cpp_gnu)
#endif // !defined(base_restrict)


#if !defined(base_declare_malloc_like)
#  if defined(__has_cpp_attribute) && __has_cpp_attribute(gnu::malloc)
#    if base_has_nodiscard
#      define base_declare_malloc_like [[nodiscard, gnu::malloc]]
#    else
#      define base_declare_malloc_like [[gnu::malloc]]
#    endif // base_has_nodiscard
#  else
#    define base_declare_malloc_like base_nodiscard base_restrict
#  endif // defined(__has_cpp_attribute) && __has_cpp_attribute(gnu::malloc)
#endif // !defined(base_declare_malloc_like)


#if !defined(base_declare_memory_allocator)
#  if defined(base_cpp_msvc) || defined(base_cpp_clang)
#    define base_declare_memory_allocator       __declspec(allocator)
#  elif defined(base_cpp_gnu) && base_cpp_gnu > 310
#    define base_declare_memory_allocator       __attribute__((malloc))
#  else
#    define base_declare_memory_allocator
#  endif // defined(base_cpp_msvc) || defined(base_cpp_clang) || (defined(base_cpp_gnu) && base_cpp_gnu > 310)
#endif // !defined(base_declare_memory_allocator)


#if !defined(base_has_cxx11)
#  if __cplusplus >= 201103L
#    define base_has_cxx11 1
#  else
#    define base_has_cxx11 0
#  endif // __cplusplus >= 201103L
#endif // !defined(base_has_cxx11)


#if !defined(base_has_cxx14)
#  if __cplusplus >= 201402L
#    define base_has_cxx14 1
#  else
#    define base_has_cxx14 0
#  endif // __cplusplus >= 201402L
#endif // !defined(base_has_cxx14)


#if !defined(base_has_cxx17)
#  if __cplusplus >= 201703L
#    define base_has_cxx17 1
#  else
#    define base_has_cxx17 0
#  endif // __cplusplus >= 201703L
#endif // !defined(base_has_cxx17)


#if !defined(base_has_cxx20)
#  if base_has_cxx17 && __cplusplus >= 202002L
#    define base_has_cxx20 1
#  else
#    define base_has_cxx20 0
#  endif // base_has_cxx17 && __cplusplus >= 202002L
#endif // !defined(base_has_cxx20)


#if !defined(base_has_cxx23)
#  if base_has_cxx20 && __cplusplus > 202002L
#    define base_has_cxx23 1
#  else
#    define base_has_cxx23 0
#  endif // base_has_cxx20 && __cplusplus > 202002L
#endif // !defined(base_has_cxx23)


#if !defined(base_constexpr_cxx20)
#  if base_has_cxx20
#    define base_constexpr_cxx20 constexpr
#  else
#    define base_constexpr_cxx20 inline
#  endif // base_has_cxx20
#endif // !defined(base_constexpr_cxx20)


#if !defined(base_clang_constexpr_cxx20)
#  if defined(base_cpp_clang)
#    define base_clang_constexpr_cxx20 constexpr_cxx20
#  else
#    define base_clang_constexpr_cxx20 
#  endif // defined(base_cpp_clang)
#endif // !defined(base_clang_constexpr_cxx20)


#if !defined(base_guard_integer_overflow)
#  if defined(base_os_windows) && defined(base_cpp_msvc)
#    define base_guard_integer_overflow __declspec(guard(overflow))
#  else
#    define base_guard_integer_overflow 
#  endif // defined(base_os_windows) && defined(base_cpp_msvc)
#endif // !defined(base_guard_integer_overflow)


#if !defined(base_sizeof_in_bits)
#  define base_sizeof_in_bits(type) (sizeof(type) * 8)
#endif // !defined(base_sizeof_in_bits)


#if !defined(base_unaligned)
#  if defined(PROCESSOR_X86_64) || defined(PROCESSOR_ARM) || (PROCESSOR_ARM == 8) // x64 ARM
#    if defined(base_os_windows) && defined(base_cpp_msvc)
#      define base_unaligned __unaligned
#    else
#      define base_unaligned
#    endif // defined(base_os_windows) && defined(base_cpp_msvc)
#  else 
#    define base_unaligned
#  endif // defined(PROCESSOR_X86_64) || defined(PROCESSOR_ARM) || (PROCESSOR_ARM == 8)
#endif // !defined(base_unaligned)


#if !defined(base_msvc_constexpr)
#  if defined(base_cpp_msvc) && defined(_MSVC_CONSTEXPR_ATTRIBUTE)
#    define base_msvc_constexpr [[msvc::constexpr]]
#  else
#    define base_msvc_constexpr
#  endif // defined(base_cpp_msvc) && defined(_MSVC_CONSTEXPR_ATTRIBUTE)
#endif // !defined(base_msvc_constexpr)


#if !defined(base_declare_cpu_only)
#  if defined(base_cpp_msvc)
#    define base_declare_cpu_only restrict(cpu)
#  else
#    define base_declare_cpu_only
#  endif // defined(base_cpp_msvc)
#endif // !defined(base_declare_cpu_only)


#if !defined(base_declare_gpu_only)
#  if defined(base_cpp_msvc)
#    define base_declare_gpu_only restrict(amp)
#  else
#    define base_declare_gpu_only
#  endif // defined(base_cpp_msvc)
#endif // !defined(base_declare_gpu_only)


#if !defined(base_declare_gpu)
#  if defined(base_cpp_msvc)
#    define base_declare_gpu restrict(amp,cpu)
#  else
#    define base_declare_gpu
#  endif // defined(base_cpp_msvc)
#endif // !defined(base_declare_gpu)


#if !defined(base_type_may_alias)
#  if (defined(base_cpp_gnu) && base_cpp_gnu < 480) || defined(base_cpp_clang)
#    define base_type_may_alias __attribute__((__may_alias__))
#  else
#    define base_type_may_alias
#  endif // defined(base_cpp_gnu) || defined(base_cpp_clang) 
#endif // !defined(base_type_may_alias)


#if !defined(base_aligned_type)
#  if defined(base_cpp_gnu)
#    define base_aligned_type(alignment) __attribute__((aligned(alignment)))
#  elif defined(base_cpp_msvc) || defined(base_cpp_clang) 
#    define base_aligned_type(alignment) __declspec(align(alignment))
#  elif base_has_cxx11
#    define base_aligned_type(alignment) alignas(alignment)
#  endif // defined(base_cpp_gnu) || defined(base_cpp_msvc) || defined(base_cpp_clang) || base_has_cxx11
#endif // !defined(base_aligned_type)


#if !defined(base_asm_extern)
#  if defined(__cplusplus)
#    define base_asm_extern extern "C"
#  else 
#    define base_asm_extern extern
#  endif // __cplusplus
#endif // !defined(base_asm_extern)


// Warnings


#if !defined(base_nodiscard_return_raw_ptr)
#  define base_nodiscard_return_raw_ptr \
        base_nodiscard_with_warning("This function allocates memory and returns a raw pointer. " \
            "Discarding the return value will cause a memory leak.")
#endif // !defined(base_nodiscard_return_raw_ptr)


#if !defined(base_nodiscard_thread_constructor)
#  define base_nodiscard_thread_constructor \
    base_nodiscard_constructor_with_warning("Creating a thread object without assigning it to a variable " \
        "may lead to unexpected behavior and resource leaks. Ensure " \
        "the thread is properly managed.")
#endif // !defined(base_nodiscard_thread_constructor)


#if !defined(base_nodiscard_remove_algorithm)
#  define base_nodiscard_remove_algorithm \
        base_nodiscard_with_warning("The 'remove' and 'remove_if' algorithms return the iterator past the last element " \
            "that should be kept. You need to call container.erase(result, container.end()) afterwards. " \
            "In C++20, 'std::erase' and 'std::erase_if' are simpler replacements for these two steps.")
#endif // !defined(base_nodiscard_remove_algorithm)


#endif // __cplusplus
