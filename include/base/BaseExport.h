#ifndef LIB_BASE_EXPORT_H
#define LIB_BASE_EXPORT_H

#ifdef LIB_BASE_STATIC
#  define LIB_BASE_EXPORT
#  define LIB_BASE_NO_EXPORT
#else
#  ifndef LIB_BASE_EXPORT
#    ifdef lib_base_EXPORTS
/* We are building this library */
#      define LIB_BASE_EXPORT 
#    else
/* We are using this library */
#      define LIB_BASE_EXPORT 
#    endif
#  endif

#  ifndef LIB_BASE_NO_EXPORT
#    define LIB_BASE_NO_EXPORT 
#  endif
#endif

#ifndef LIB_BASE_DEPRECATED
#  define LIB_BASE_DEPRECATED __declspec(deprecated)
#endif

#ifndef LIB_BASE_DEPRECATED_EXPORT
#  define LIB_BASE_DEPRECATED_EXPORT LIB_BASE_EXPORT LIB_BASE_DEPRECATED
#endif

#ifndef LIB_BASE_DEPRECATED_NO_EXPORT
#  define LIB_BASE_DEPRECATED_NO_EXPORT LIB_BASE_NO_EXPORT LIB_BASE_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef LIB_BASE_NO_DEPRECATED
#    define LIB_BASE_NO_DEPRECATED
#  endif
#endif

#endif /* LIB_BASE_EXPORT_H */
