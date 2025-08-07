#pragma once 

#include <base/core/arch/CompilerDetection.h>


#if !defined(base_maybe_unused_attribute)
#  if defined(__has_cpp_attribute) && __has_cpp_attribute(maybe_unused) >= 201603L
#    define base_maybe_unused_attribute     [[maybe_unused]]
#  elif defined(base_cpp_gnu) || defined(base_cpp_clang)
#    define base_maybe_unused_attribute     __attribute__(unused)
#  endif // defined(__has_cpp_attribute) && __has_cpp_attribute(maybe_unused) >= 201603L
#endif // !defined(base_maybe_unused_attribute)


#if !defined(base_unused)
#  define base_unused(variable) ((void)(variable))
#endif // !defined(base_unused)

#if !defined(base_unreachable)
#  if defined(base_cpp_gnu) || defined(base_cpp_clang)
#    define base_unreachable() __builtin_unreachable()
#  elif defined(base_cpp_msvc)
#    define base_unreachable() (__assume(0))
#  else
#    define base_unreachable() ((void)0)
#  endif // defined(base_cpp_gnu) || defined(base_cpp_clang) || defined(base_cpp_msvc)
#endif // !defined(base_unreachable)

