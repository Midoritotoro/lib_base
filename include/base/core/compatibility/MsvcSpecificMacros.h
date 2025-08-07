#pragma once 

#include <base/core/arch/CompilerDetection.h>


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
