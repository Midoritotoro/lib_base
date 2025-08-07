#pragma once 


#if !defined(base_has_cxx11)
#  if __cplusplus >= 201103L
#    define base_has_cxx11 1
#  else
#    define base_has_cxx11 0
#  endif // __cplusplus >= 201103L
#endif // !defined(base_has_cxx11)


#if !defined(base_has_cxx14)
#  if __cplusplus >= 201402L
#    define base_has_cxx14 1
#  else
#    define base_has_cxx14 0
#  endif // __cplusplus >= 201402L
#endif // !defined(base_has_cxx14)


#if !defined(base_has_cxx17)
#  if __cplusplus >= 201703L
#    define base_has_cxx17 1
#  else
#    define base_has_cxx17 0
#  endif // __cplusplus >= 201703L
#endif // !defined(base_has_cxx17)


#if !defined(base_has_cxx20)
#  if base_has_cxx17 && __cplusplus >= 202002L
#    define base_has_cxx20 1
#  else
#    define base_has_cxx20 0
#  endif // base_has_cxx17 && __cplusplus >= 202002L
#endif // !defined(base_has_cxx20)


#if !defined(base_has_cxx23)
#  if base_has_cxx20 && __cplusplus > 202002L
#    define base_has_cxx23 1
#  else
#    define base_has_cxx23 0
#  endif // base_has_cxx20 && __cplusplus > 202002L
#endif // !defined(base_has_cxx23)