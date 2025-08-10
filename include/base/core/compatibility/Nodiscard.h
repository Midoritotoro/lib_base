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


#if !defined(base_nodiscard)
#  if base_has_nodiscard
#    define base_nodiscard  [[nodiscard]]
#  elif defined(base_cpp_gnu)
#    define base_nodiscard  __attribute__((__warn_unused_result__))
#  elif defined(base_cpp_clang)
#    define base_nodiscard  __attribute__(warn_unused_result)
#  elif defined(base_cpp_msvc)
#    define base_nodiscard  _Check_return_
#  else
#    define base_nodiscard
#  endif // base_has_nodiscard || defined(base_cpp_gnu) || defined(base_cpp_clang) || defined(base_cpp_msvc)
#endif // !defined(base_nodiscard)


#if !defined(base_nodiscard_with_warning)
#  if defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard) >= 201907L
#    define base_nodiscard_with_warning(message)    [[nodiscard(message)]]
#  elif defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard) >= 201603L
#    define base_nodiscard_with_warning(message)    base_nodiscard
#  else
#    define base_nodiscard_with_warning(message)
#  endif //     defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard) >= 201907L 
//  || defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard) >= 201603L
#endif // !defined(base_nodiscard_with_warning)


#if !defined(base_nodiscard_constructor)
// https://open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1771r1.pdf
#  if defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard) >= 201907L
#    define base_nodiscard_constructor base_nodiscard
#  else
#    define base_nodiscard_constructor
#  endif // defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard) >= 201907L
#endif // !defined(base_nodiscard_constructor)


#if !defined(base_nodiscard_constructor_with_warning)
#  if defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard) >= 201907L
#    define base_nodiscard_constructor_with_warning(message) base_nodiscard_with_warning(message)
#  else
#    define base_nodiscard_constructor_with_warning(message) 
#  endif // defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard) >= 201907L
#endif // !defined(base_nodiscard_constructor_with_warning)