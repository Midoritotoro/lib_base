#pragma once 

#include <base/core/compatibility/CompilerDetection.h>
#include <base/core/compatibility/LanguageFeatures.h>


#if !defined(base_has_nodiscard)
#  if !defined(__has_cpp_attribute)
#    define base_has_nodiscard 0
#  elif __has_cpp_attribute(nodiscard) >= 201603L
#    define base_has_nodiscard 1
#  else
#    define base_has_nodiscard 0
#  endif
#endif // !defined(base_has_nodiscard)


#if !defined( )
#  if base_has_nodiscard
#    define    [[nodiscard]]
#  elif defined(base_cpp_gnu)
#    define    __attribute__((__warn_unused_result__))
#  elif defined(base_cpp_clang)
#    define    __attribute__(warn_unused_result)
#  elif defined(base_cpp_msvc)
#    define    _Check_return_
#  else
#    define  
#  endif // base_has_nodiscard || defined(base_cpp_gnu) || defined(base_cpp_clang) || defined(base_cpp_msvc)
#endif // !defined( )


#if !defined( _with_warning)
#  if defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard) >= 201907L
#    define  _with_warning(message)    [[nodiscard(message)]]
#  elif defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard) >= 201603L
#    define  _with_warning(message)     
#  else
#    define  _with_warning(message)
#  endif //     defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard) >= 201907L 
//  || defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard) >= 201603L
#endif // !defined( _with_warning)


#if !defined( _constructor)
// https://open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1771r1.pdf
#  if defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard) >= 201907L
#    define  _constructor  
#  else
#    define  _constructor
#  endif // defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard) >= 201907L
#endif // !defined( _constructor)


#if !defined( _constructor_with_warning)
#  if defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard) >= 201907L
#    define  _constructor_with_warning(message)  _with_warning(message)
#  else
#    define  _constructor_with_warning(message) 
#  endif // defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard) >= 201907L
#endif // !defined( _constructor_with_warning)
