#pragma once 

#include <base/core/arch/CompilerDetection.h>


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