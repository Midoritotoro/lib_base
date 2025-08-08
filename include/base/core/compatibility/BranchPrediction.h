#pragma once 

#include <base/core/Types.h>


__BASE_NAMESPACE_BEGIN


#if !defined(base_likely)
#  if defined(base_cpp_gnu) || defined(base_cpp_clang)
#    define base_likely(expression) __builtin_expect(!!(expression), true)
#  elif defined(base_cpp_msvc) && defined(__has_cpp_attribute) && __has_cpp_attribute(likely) >= 201803L
#    define base_likely(expression)                     \
       (                                                \
         ([](bool value){                               \
           switch (value) {                             \
             [[unlikely]] case true: return true;       \
             [[likely]] case false: return false;       \
         }                                              \
       })(expression))
#  else
#    define base_likely(expression) (!!(expression))
#  endif // defined(base_cpp_gnu) || defined(base_cpp_clang)
#endif // !defined(base_likely)


#if !defined(base_unlikely)
#  if defined(base_cpp_gnu) || defined(base_cpp_clang)
#    define base_unlikely(expression) __builtin_expect(!!(expression), false)
#  elif defined(base_cpp_msvc) && defined(__has_cpp_attribute) && __has_cpp_attribute(unlikely) >= 201803L
#    define base_unlikely(expression)                   \
       (                                                \
         ([](bool value){                               \
           switch (value) {                             \
             [[likely]] case true: return true;         \
             [[unlikely]] case false: return false;     \
         }                                              \
       })(expression))
#  else
#    define base_unlikely(expression) (!!(expression))
#  endif // defined(base_cpp_gnu) || defined(base_cpp_clang)
#endif // !defined(base_unlikely)


#if !defined(base_likely_attribute)
#  if defined(__has_cpp_attribute) && __has_cpp_attribute(likely) >= 201803L
#    define base_likely_attribute [[likely]]
#  else
#    define base_likely_attribute
#  endif
#endif // !defined(base_likely_attribute)


#if !defined(base_unlikely_attribute)
#  if defined(__has_cpp_attribute) && __has_cpp_attribute(unlikely) >= 201803L
#    define base_unlikely_attribute [[unlikely]]
#  else
#    define base_unlikely_attribute
#  endif
#endif // !defined(base_unlikely_attribute)


//// Branchless minimum function for two signed 32-bit integers.
//base_nodiscard constexpr base_base_always_inline int32 min(int32 first, int32 second) noexcept {
//	return second + ((first - second) & (first - second) >> 31); 
//}
//
//// Branchless maximum function for two signed 32-bit integers.
//base_nodiscard constexpr base_base_always_inline int32 max(int32 first, int32 second) noexcept {
//	return first - ((first - second) & (first - second) >> 31);
//}

__BASE_NAMESPACE_END
