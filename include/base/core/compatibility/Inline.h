#pragma once 

#include <base/core/compatibility/CompilerDetection.h>


#if !defined(base_never_inline)
#  if defined(base_cpp_msvc) || defined(base_cpp_clang)
#     define base_never_inline __declspec(noinline)
#  elif defined(base_cpp_gnu) 
#    define base_never_inline __attribute__((noinline))
#  else 
#    define base_never_inline 
#  endif // defined(base_cpp_msvc) || defined(base_cpp_clang) || defined(base_cpp_gnu)
#endif // !defined(base_never_inline)


#if !defined(base_always_inline)
#  if defined(base_cpp_msvc) || defined(base_cpp_clang)
#    define base_always_inline __forceinline
#  elif defined(base_cpp_gnu)
#    define base_always_inline inline __attribute__((base_always_inline))
#  else 
#    define base_always_inline inline
#  endif // defined(base_cpp_msvc) || defined(base_cpp_clang) || defined(base_cpp_gnu)
#endif // !defined(base_always_inline)


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