#pragma once

#include <base/core/arch/CompilerDetection.h>


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


#if !defined(base_noreturn)
#  if defined(base_cpp_gnu)
#    define base_noreturn           __attribute__((__noreturn__))
#  elif defined(base_cpp_msvc) || defined(base_cpp_clang)
#    define base_noreturn           __declspec(noreturn)
#  else 
#    define base_noreturn       
#  endif // defined(base_cpp_gnu) || defined(base_cpp_msvc) || defined(base_cpp_clang)
#endif // !defined(base_noreturn)


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


#if !defined(base_func_info)
#  if defined(base_cpp_msvc)
#    define base_func_info          __FUNCSIG__
#  elif defined(base_cpp_gnu)
#    define base_func_info          __PRETTY_FUNCTION__
#  else
#    define base_func_info          __FILE__ ":" stringify(__LINE__)
#  endif // defined(base_cpp_msvc) || defined(base_cpp_gnu)
#endif // !defined(base_func_info)