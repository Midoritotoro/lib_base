#pragma once 

#include <base/core/compatibility/CompilerDetection.h>
#include <base/core/arch/SystemDetection.h>

#include <base/core/arch/ProcessorDetection.h>


#if !defined(base_unaligned)
#  if defined(base_processor_x86_64) || defined(base_processor_arm) || (base_processor_arm == 8) // x64 ARM
#    if defined(base_os_windows) && defined(base_cpp_msvc)
#      define base_unaligned __unaligned
#    else
#      define base_unaligned
#    endif // defined(base_os_windows) && defined(base_cpp_msvc)
#  else 
#    define base_unaligned
#  endif // defined(base_processor_x86_64) || defined(base_processor_arm) || (base_processor_arm == 8)
#endif // !defined(base_unaligned)


#if !defined(base_aligned_type)
#  if defined(base_cpp_gnu)
#    define base_aligned_type(alignment) __attribute__((aligned(alignment)))
#  elif defined(base_cpp_msvc) || defined(base_cpp_clang) 
#    define base_aligned_type(alignment) __declspec(align(alignment))
#  elif base_has_cxx11
#    define base_aligned_type(alignment) alignas(alignment)
#  endif // defined(base_cpp_gnu) || defined(base_cpp_msvc) || defined(base_cpp_clang) || base_has_cxx11
#endif // !defined(base_aligned_type)