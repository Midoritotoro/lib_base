#pragma once 

#if defined(_MSC_VER)
#  define base_cpp_msvc			(_MSC_VER)
#  define base_cpp_msvc_only	base_cpp_msvc

#  if defined(__clang__)
#    undef base_cpp_msvc_only
#    define base_cpp_clang ((__clang_major__ * 100) + __clang_minor__)
#    define base_cpp_clang_only base_cpp_clang
#  endif // defined(__clang__)
#

#elif defined(__GNUC__)
#  define base_cpp_gnu          (__GNUC__ * 100 + __GNUC_MINOR__)

#  if defined(__MINGW32__)
#    define base_cpp_mingw
#  endif

#  if defined(__clang__)
#    if defined(__apple_build_version__)

#      if __apple_build_version__   >= 14030022 // Xcode 14.3
#        define base_cpp_clang 1500

#      elif __apple_build_version__ >= 14000029 // Xcode 14.0
#        define base_cpp_clang 1400

#      elif __apple_build_version__ >= 13160021 // Xcode 13.3
#        define base_cpp_clang 1300

#      elif __apple_build_version__ >= 13000029 // Xcode 13.0
#        define base_cpp_clang 1200

#      elif __apple_build_version__ >= 12050022 // Xcode 12.5
#        define base_cpp_clang 1110

#      elif __apple_build_version__ >= 12000032 // Xcode 12.0
#        define base_cpp_clang 1000

#      elif __apple_build_version__ >= 11030032 // Xcode 11.4
#        define base_cpp_clang 900

#      elif __apple_build_version__ >= 11000033 // Xcode 11.0
#        define base_cpp_clang 800

#      else
#        error ""
#      endif // __apple_build_version__

#    else
#      define base_cpp_clang ((__clang_major__ * 100) + __clang_minor__)
#    endif // defined(__apple_build_version__)

#    define base_cpp_clang_only base_cpp_clang


#    if !defined(__has_extension)
#      define __has_extension __has_feature
#    endif // !defined(__has_extension)
#  else

#    define base_cpp_gnu_only base_cpp_gnu
#  endif // defined(__clang__)

#endif // defined(_MSC_VER) || defined(__GNUC__)