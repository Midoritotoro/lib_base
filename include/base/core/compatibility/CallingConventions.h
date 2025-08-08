#pragma once 

#include <base/core/compatibility/CompilerDetection.h>
#include <base/core/arch/ProcessorDetection.h>

#include <base/core/arch/SimdHelpers.h>


#if !defined(base_fastcall)
#  if defined(base_processor_x86_32)
#    if defined(base_cpp_gnu) || defined(base_cpp_clang)
#      define base_fastcall __attribute__((regparm(3)))
#    elif defined(base_cpp_msvc)
#      define base_fastcall __fastcall
#    else
#      define base_fastcall
#    endif // defined(base_cpp_gnu) || defined(base_cpp_msvc) || defined(base_cpp_clang)
#  else
#    define base_fastcall
#  endif // defined(base_processor_x86_32)
#endif // !defined(base_fastcall)


#if !defined(base_stdcall)
#  if defined(base_cpp_msvc) || defined(base_cpp_clang)
#    define base_stdcall            __stdcall
#  elif defined(base_cpp_gnu)
#    define base_stdcall            __attribute__((__stdcall__))
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


#if !defined(base_vectorcall)
#  if defined(base_cpp_msvc) && defined(base_processor_x86) && defined(__SSE2__)
#    define base_vectorcall __vectorcall
#  else
#    define base_vectorcall
#  endif
#endif // !defined(base_vectorcall)
