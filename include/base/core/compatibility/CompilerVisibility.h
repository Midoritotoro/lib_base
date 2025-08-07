#pragma once

#include <base/core/arch/CompilerDetection.h>
#include <base/core/arch/SystemDetection.h>

#if !defined(base_decl_export)
#  if defined(base_cpp_msvc) || defined(base_cpp_clang)
#    define base_decl_export                __declspec(dllexport)
#  elif defined(base_cpp_gnu)
#    if defined(base_os_windows)
#      define base_decl_export              __declspec(dllexport)
#    else
#      if defined(base_use_protected_visibility)
#         define base_decl_export           __attribute__((visibility("protected")))
#      else
#         define base_decl_export           __attribute__((visibility("default")))
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


#if !defined(base_asm_extern)
#  if defined(__cplusplus)
#    define base_asm_extern extern "C"
#  else 
#    define base_asm_extern extern
#  endif // __cplusplus
#endif // !defined(base_asm_extern)

