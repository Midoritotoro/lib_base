#pragma once

#include <base/system/ProcessorDetection.h>
#include <base/system/SystemDetection.h>

#if defined(_MSC_VER)
#  define CPP_MSVC (_MSC_VER)
#  define CPP_MSVC_NET

#  define CPP_MSVC_ONLY CPP_MSVC

#  ifdef __clang__
#    undef CPP_MSVC_ONLY
#    define CPP_CLANG ((__clang_major__ * 100) + __clang_minor__)
#    define CPP_CLANG_ONLY CPP_CLANG
#  endif

#  define OUTOFLINE_TEMPLATE inline

#  define COMPILER_MANGLES_RETURN_TYPE
#  define COMPILER_MANGLES_ACCESS_SPECIFIER
#
#  define FUNC_INFO __FUNCSIG__
#  define UNREACHABLE_IMPL() __assume(0)

#  define DECL_EXPORT __declspec(dllexport)
#  define DECL_IMPORT __declspec(dllimport)

#  define COMPILER_COMPLAINS_ABOUT_RETURN_AFTER_UNREACHABLE
#elif defined(__GNUC__)
#  define CPP_GNU          (__GNUC__ * 100 + __GNUC_MINOR__)
#  if defined(__MINGW32__)
#    define CPP_MINGW
#  endif
#  if defined(__clang__)
#    if defined(__apple_build_version__)
#      if __apple_build_version__   >= 14030022 // Xcode 14.3
#        define CPP_CLANG 1500
#      elif __apple_build_version__ >= 14000029 // Xcode 14.0
#        define CPP_CLANG 1400
#      elif __apple_build_version__ >= 13160021 // Xcode 13.3
#        define CPP_CLANG 1300
#      elif __apple_build_version__ >= 13000029 // Xcode 13.0
#        define CPP_CLANG 1200
#      elif __apple_build_version__ >= 12050022 // Xcode 12.5
#        define CPP_CLANG 1110
#      elif __apple_build_version__ >= 12000032 // Xcode 12.0
#        define CPP_CLANG 1000
#      elif __apple_build_version__ >= 11030032 // Xcode 11.4
#        define CPP_CLANG 900
#      elif __apple_build_version__ >= 11000033 // Xcode 11.0
#        define CPP_CLANG 800
#      else
#        error ""
#      endif
#    else

#      define CPP_CLANG ((__clang_major__ * 100) + __clang_minor__)
#    endif
#    define CPP_CLANG_ONLY CPP_CLANG
#    if __has_builtin(__builtin_assume)
#      define ASSUME_IMPL(expr)   __builtin_assume(expr)
#    else
#      define ASSUME_IMPL(expr)  if (expr){} else __builtin_unreachable()
#    endif
#    define UNREACHABLE_IMPL() __builtin_unreachable()
#    if !defined(__has_extension)
#      
#      define __has_extension __has_feature
#    endif
#  else

#    define CPP_GNU_ONLY CPP_GNU
#    if CPP_GNU >= 405
#      define ASSUME_IMPL(expr)  if (expr){} else __builtin_unreachable()
#      define UNREACHABLE_IMPL() __builtin_unreachable()
#      define DECL_DEPRECATED_X(text) __attribute__ ((__deprecated__(text)))
#    endif
#  endif

#  ifdef OS_WIN
#    define DECL_EXPORT     __declspec(dllexport)
#    define DECL_IMPORT     __declspec(dllimport)
#  else
#    define DECL_EXPORT_OVERRIDABLE __attribute__((visibility("default"), weak))
#    ifdef USE_PROTECTED_VISIBILITY
#      define DECL_EXPORT     __attribute__((visibility("protected")))
#    else
#      define DECL_EXPORT     __attribute__((visibility("default")))
#    endif
#    define DECL_IMPORT     __attribute__((visibility("default")))
#    define DECL_HIDDEN     __attribute__((visibility("hidden")))
#  endif

#  define FUNC_INFO       __PRETTY_FUNCTION__

#  define TYPEOF(expr)    __typeof__(expr)
#  define DECL_DEPRECATED __attribute__ ((__deprecated__))

#  define DECL_UNUSED     __attribute__((__unused__))
#  define LIKELY(expr)    __builtin_expect(!!(expr), true)

#  define UNLIKELY(expr)  __builtin_expect(!!(expr), false)
#  define NORETURN        __attribute__((__noreturn__))

#  define REQUIRED_RESULT __attribute__ ((__warn_unused_result__))
#  define DECL_PURE_FUNCTION __attribute__((pure))

#  define DECL_CONST_FUNCTION __attribute__((const))
#  define DECL_COLD_FUNCTION __attribute__((cold))

# else 
# endif

#ifndef __has_builtin
#  define __has_builtin(x)             0
#endif
#ifndef __has_feature
#  define __has_feature(x)             0
#endif
#ifndef __has_attribute
#  define __has_attribute(x)           0
#endif
#ifndef __has_cpp_attribute
#  define __has_cpp_attribute(x)       0
#endif
#ifndef __has_include
#  define __has_include(x)             0
#endif
#ifndef __has_include_next
#  define __has_include_next(x)        0
#endif

#ifdef __cplusplus
# if __has_include(<version>)
#  include <version>
# endif
# endif


#  if (defined(__cplusplus) && __cplusplus >= 201103L) \
      || defined(__GXX_EXPERIMENTAL_CXX0X__)
/*  http://clang.llvm.org/docs/LanguageExtensions.html#cxx11 */
#    if __has_feature(cxx_alignas)
#      define COMPILER_ALIGNAS
#      define COMPILER_ALIGNOF
#    endif

#    if __has_feature(cxx_atomic) && __has_include(<atomic>)
#     define COMPILER_ATOMICS
#    endif

#    if __has_feature(cxx_attributes)
#      define COMPILER_ATTRIBUTES
#    endif

#    if __has_feature(cxx_auto_type)
#      define COMPILER_AUTO_FUNCTION
#      define COMPILER_AUTO_TYPE
#    endif

#    if __has_feature(cxx_strong_enums)
#      define COMPILER_CLASS_ENUM
#    endif

#    if __has_feature(cxx_constexpr) && Q_CC_CLANG > 302 /* CLANG 3.2 has bad/partial support */
#      define COMPILER_CONSTEXPR
#    endif

#    if __has_feature(cxx_decltype) /* && __has_feature(cxx_decltype_incomplete_return_types) */
#      define COMPILER_DECLTYPE
#    endif

#    if __has_feature(cxx_defaulted_functions)
#      define COMPILER_DEFAULT_MEMBERS
#    endif

#    if __has_feature(cxx_deleted_functions)
#      define COMPILER_DELETE_MEMBERS
#    endif

#    if __has_feature(cxx_delegating_constructors)
#      define COMPILER_DELEGATING_CONSTRUCTORS
#    endif

#    if __has_feature(cxx_explicit_conversions)
#      define COMPILER_EXPLICIT_CONVERSIONS
#    endif

#    if __has_feature(cxx_override_control)
#      define COMPILER_EXPLICIT_OVERRIDES
#    endif

#    if __has_feature(cxx_inheriting_constructors)
#      define COMPILER_INHERITING_CONSTRUCTORS
#    endif

#    if __has_feature(cxx_generalized_initializers)
#      define COMPILER_INITIALIZER_LISTS
#      define COMPILER_UNIFORM_INIT /* both covered by this feature macro, according to docs */
#    endif

#    if __has_feature(cxx_lambdas)
#      define COMPILER_LAMBDA
#    endif

#    if __has_feature(cxx_noexcept)
#      define COMPILER_NOEXCEPT
#    endif

#    if __has_feature(cxx_nonstatic_member_init)
#      define COMPILER_NONSTATIC_MEMBER_INIT
#    endif

#    if __has_feature(cxx_nullptr)
#      define COMPILER_NULLPTR
#    endif

#    if __has_feature(cxx_range_for)
#      define COMPILER_RANGE_FOR
#    endif

#    if __has_feature(cxx_raw_string_literals)
#      define COMPILER_RAW_STRINGS
#    endif

#    if __has_feature(cxx_reference_qualified_functions)
#      define COMPILER_REF_QUALIFIERS
#    endif

#    if __has_feature(cxx_rvalue_references)
#      define COMPILER_RVALUE_REFS
#    endif

#    if __has_feature(cxx_static_assert)
#      define COMPILER_STATIC_ASSERT
#    endif

#    if __has_feature(cxx_alias_templates)
#      define COMPILER_TEMPLATE_ALIAS
#    endif

#    if __has_feature(cxx_thread_local)
#      if !defined(__FreeBSD__) /* FreeBSD clang fails on __cxa_thread_atexit */
#        define COMPILER_THREAD_LOCAL
#      endif
#    endif

#    if __has_feature(cxx_user_literals)
#      define COMPILER_UDL
#    endif

#    if __has_feature(cxx_unicode_literals)
#      define COMPILER_UNICODE_STRINGS
#    endif

#    if __has_feature(cxx_unrestricted_unions)
#      define COMPILER_UNRESTRICTED_UNIONS
#    endif

#    if __has_feature(cxx_variadic_templates)
#      define COMPILER_VARIADIC_TEMPLATES
#    endif

#  endif // (defined(__cplusplus) && __cplusplus >= 201103L) || defined(__GXX_EXPERIMENTAL_CXX0X__)

#define DO_PRAGMA(text)                      _Pragma(#text)
#if defined(CPP_MSVC) && !defined(CPP_CLANG)
#  undef DO_PRAGMA                           /* not needed */
#  define WARNING_PUSH                       __pragma(warning(push))
#  define WARNING_POP                        __pragma(warning(pop))
#  define WARNING_DISABLE_MSVC(number)       __pragma(warning(disable: number))
#  define WARNING_DISABLE_INTEL(number)
#  define WARNING_DISABLE_CLANG(text)
#  define WARNING_DISABLE_GCC(text)
#  define WARNING_DISABLE_DEPRECATED         WARNING_DISABLE_MSVC(4996)
#  define WARNING_DISABLE_FLOAT_COMPARE
#  define WARNING_DISABLE_INVALID_OFFSETOF
#elif defined(CPP_CLANG)
#  define WARNING_PUSH                       DO_PRAGMA(clang diagnostic push)
#  define WARNING_POP                        DO_PRAGMA(clang diagnostic pop)
#  define WARNING_DISABLE_CLANG(text)        DO_PRAGMA(clang diagnostic ignored text)
#  define WARNING_DISABLE_GCC(text)
#  define WARNING_DISABLE_INTEL(number)
#  define WARNING_DISABLE_MSVC(number)
#  define WARNING_DISABLE_DEPRECATED         WARNING_DISABLE_CLANG("-Wdeprecated-declarations")
#  define WARNING_DISABLE_FLOAT_COMPARE      WARNING_DISABLE_CLANG("-Wfloat-equal")
#  define WARNING_DISABLE_INVALID_OFFSETOF   WARNING_DISABLE_CLANG("-Winvalid-offsetof")
#elif defined(CPP_GNU) && (__GNUC__ * 100 + __GNUC_MINOR__ >= 406)
#  define WARNING_PUSH                       DO_PRAGMA(GCC diagnostic push)
#  define WARNING_POP                        DO_PRAGMA(GCC diagnostic pop)
#  define WARNING_DISABLE_GCC(text)          DO_PRAGMA(GCC diagnostic ignored text)
#  define WARNING_DISABLE_CLANG(text)
#  define WARNING_DISABLE_INTEL(number)
#  define WARNING_DISABLE_MSVC(number)
#  define WARNING_DISABLE_DEPRECATED         WARNING_DISABLE_GCC("-Wdeprecated-declarations")
#  define WARNING_DISABLE_FLOAT_COMPARE      WARNING_DISABLE_GCC("-Wfloat-equal")
#  define WARNING_DISABLE_INVALID_OFFSETOF   WARNING_DISABLE_GCC("-Winvalid-offsetof")
#else       // Все остальные компиляторы, GCC < 4.6 и MSVC < 2008
#  define WARNING_DISABLE_GCC(text)
#  define WARNING_PUSH
#  define WARNING_POP
#  define WARNING_DISABLE_INTEL(number)
#  define WARNING_DISABLE_MSVC(number)
#  define WARNING_DISABLE_CLANG(text)
#  define WARNING_DISABLE_GCC(text)
#  define WARNING_DISABLE_DEPRECATED
#  define WARNING_DISABLE_FLOAT_COMPARE
#  define WARNING_DISABLE_INVALID_OFFSETOF
#endif

#define CAST_IGNORE_ALIGN(body) WARNING_PUSH WARNING_DISABLE_GCC("-Wcast-align") body WARNING_POP

#define OFFSETOF(Class, member) \
    []() -> size_t { \
        WARNING_PUSH WARNING_DISABLE_INVALID_OFFSETOF \
        return offsetof(Class, member); \
        WARNING_POP \
    }()


# ifdef __cplusplus

#if !defined(PROCESSOR_X86)
#  undef LIB_BASE_ENABLE_sse2
#  undef LIB_BASE_ENABLE_sse3
#  undef LIB_BASE_ENABLE_ssse3
#  undef LIB_BASE_ENABLE_sse4_1
#  undef LIB_BASE_ENABLE_sse4_2
#  undef LIB_BASE_ENABLE_avx
#  undef LIB_BASE_ENABLE_avx2
#  undef LIB_BASE_ENABLE_f16c // ????????
#endif


#if !defined(CPP_WARNINGS)
#  define NO_WARNINGS
#endif
#if defined(NO_WARNINGS)
#  if defined(CPP_MSVC)
WARNING_DISABLE_MSVC(4828) /* Файл содержит знак, начинающийся со смещения 0xX, который является недопустимым в текущей исходной кодировке. */
WARNING_DISABLE_MSVC(4251) /* класс 'type' должен иметь dll-интерфейс, который будет использоваться клиентами класса 'type2' */
WARNING_DISABLE_MSVC(4244) /* преобразование из 'type1' в 'type2', возможная потеря данных */
WARNING_DISABLE_MSVC(4275) /* идентификатор ключа класса, отличного от DLL-интерфейса, используемый в качестве базового для идентификатора ключа класса DLL-интерфейса */
WARNING_DISABLE_MSVC(4514) /* удалена встроенная функция, на которую нет ссылок */
WARNING_DISABLE_MSVC(4800) /* 'type' : принудительное присвоение значения bool 'true' или 'false' (предупреждение о производительности) */
WARNING_DISABLE_MSVC(4097) /* typedef-имя 'identifier1' используется как синоним имени класса 'identifier2' */
WARNING_DISABLE_MSVC(4706) /* присвоение в условном выражении */
WARNING_DISABLE_MSVC(4355) /* 'this' : используется в списке инициализаторов базовых элементов */
WARNING_DISABLE_MSVC(4710) /* функция не встроена */
WARNING_DISABLE_MSVC(4530) /* Используется обработчик исключений C++, но семантика размотки не включена. Укажите /EHsc */
WARNING_DISABLE_MSVC(4006) 
#endif
#endif

# endif // __cplusplus
