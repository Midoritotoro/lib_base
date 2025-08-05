#pragma once

#include <base/core/arch/ProcessorDetection.h>
#include <base/core/arch/SystemDetection.h>

#if defined(_MSC_VER)
#  define base_cpp_msvc (_MSC_VER)
#  define base_cpp_msvc_net

#  define base_cpp_msvc_only base_cpp_msvc

#  if defined(__clang__)
#    undef base_cpp_msvc_only
#    define base_cpp_clang ((__clang_major__ * 100) + __clang_minor__)
#    define base_cpp_clang_only base_cpp_clang
#  endif // __clang__

#  define base_compiler_mangles_return_type
#  define base_compiler_mangles_access_specifier
#
#  define base_func_info __FUNCSIG__
#  define base_unreachable_impl() __assume(0)



#  define base_compiler_complains_about_return_after_unreachable
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
#      endif
#    else
#      define base_cpp_clang ((__clang_major__ * 100) + __clang_minor__)
#    endif

#    define base_cpp_clang_only base_cpp_clang

#    if __has_builtin(__builtin_assume)
#      define base_assume_impl(expr)   __builtin_assume(expr)
#    else
#      define base_assume_impl(expr)  if (expr){} else __builtin_unreachable()
#    endif

#    define base_unreachable_impl() __builtin_unreachable()

#    if !defined(__has_extension)
#      define __has_extension __has_feature
#    endif
#  else

#    define base_cpp_gnu_only base_cpp_gnu
#    if base_cpp_gnu >= 405
#      define base_assume_impl(expr)  if (expr){} else __builtin_unreachable()
#      define base_unreachable_impl() __builtin_unreachable()
#      define base_decl_deprecated_x(text) __attribute__ ((__deprecated__(text)))
#    endif
#  endif

#  if defined(base_os_windows)
#    define base_decl_import     __declspec(dllimport)
#  else
#    define base_decl_export_overridable    __attribute__((visibility("default"), weak))
#  endif

#  define base_func_info       __PRETTY_FUNCTION__

#  define base_typeof(expr)    __typeof__(expr)
#  define base_decl_deprecated __attribute__ ((__deprecated__))

#  define base_likely(expr)    __builtin_expect(!!(expr), true)

#  define base_unlikely(expr)  __builtin_expect(!!(expr), false)
#  define base_required_result __attribute__ ((__warn_unused_result__))
#  define base_decl_pure_function __attribute__((pure))

#  define base_decl_const_function __attribute__((const))
#  define base_decl_cold_function __attribute__((cold))

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
#      define base_compiler_alignas
#      define base_compiler_alignof
#    endif

#    if __has_feature(cxx_atomic) && __has_include(<atomic>)
#     define base_compiler_atomics
#    endif

#    if __has_feature(cxx_attributes)
#      define base_compiler_attributes
#    endif

#    if __has_feature(cxx_auto_type)
#      define base_compiler_auto_function
#      define base_compiler_auto_type
#    endif

#    if __has_feature(cxx_strong_enums)
#      define base_compiler_class_enum
#    endif

#    if __has_feature(cxx_constexpr) && CPP_CLANG > 302 /* CLANG 3.2 has bad/partial support */
#      define base_compiler_constexpr
#    endif

#    if __has_feature(cxx_decltype) /* && __has_feature(cxx_decltype_incomplete_return_types) */
#      define base_compiler_decltype
#    endif

#    if __has_feature(cxx_defaulted_functions)
#      define base_compiler_default_members
#    endif

#    if __has_feature(cxx_deleted_functions)
#      define base_compiler_delete_members
#    endif

#    if __has_feature(cxx_delegating_constructors)
#      define base_compiler_delegating_constructors
#    endif

#    if __has_feature(cxx_explicit_conversions)
#      define base_compiler_explicit_conversions
#    endif

#    if __has_feature(cxx_override_control)
#      define base_compiler_explicit_overrides
#    endif

#    if __has_feature(cxx_inheriting_constructors)
#      define base_compiler_inheriting_constructors
#    endif

#    if __has_feature(cxx_generalized_initializers)
#      define base_compiler_initializer_lists
#      define base_compiler_uniform_init /* both covered by this feature macro, according to docs */
#    endif

#    if __has_feature(cxx_lambdas)
#      define base_compiler_lambda
#    endif

#    if __has_feature(cxx_noexcept)
#      define base_compiler_noexcept
#    endif

#    if __has_feature(cxx_nonstatic_member_init)
#      define base_compiler_nonstatic_member_init
#    endif

#    if __has_feature(cxx_nullptr)
#      define base_compiler_nullptr
#    endif

#    if __has_feature(cxx_range_for)
#      define base_compiler_range_for
#    endif

#    if __has_feature(cxx_raw_string_literals)
#      define base_compiler_raw_strings
#    endif

#    if __has_feature(cxx_reference_qualified_functions)
#      define base_compiler_ref_qualifiers
#    endif

#    if __has_feature(cxx_rvalue_references)
#      define base_compiler_rvalue_refs
#    endif

#    if __has_feature(cxx_static_assert)
#      define base_compiler_static_assert
#    endif

#    if __has_feature(cxx_alias_templates)
#      define base_compiler_template_alias
#    endif

#    if __has_feature(cxx_thread_local)
#      if !defined(__FreeBSD__) /* FreeBSD clang fails on __cxa_thread_atexit */
#        define base_compiler_thread_local
#      endif
#    endif

#    if __has_feature(cxx_user_literals)
#      define base_compiler_udl
#    endif

#    if __has_feature(cxx_unicode_literals)
#      define base_compiler_unicode_strings
#    endif

#    if __has_feature(cxx_unrestricted_unions)
#      define base_compiler_unrestricted_unions
#    endif

#    if __has_feature(cxx_variadic_templates)
#      define base_compiler_variadic_templates
#    endif

#  endif // (defined(__cplusplus) && __cplusplus >= 201103L) || defined(__GXX_EXPERIMENTAL_CXX0X__)

#define base_do_pragma(text)                      _Pragma(#text)
#if defined(base_cpp_msvc) && !defined(base_cpp_clang)
#  undef base_do_pragma                           /* not needed */
#  define base_warning_push                         __pragma(warning(push))
#  define base_warning_pop                          __pragma(warning(pop))
#  define base_disable_warning_msvc(number)         __pragma(warning(disable: number))
#  define base_disable_warning_intel(number)
#  define base_disable_warning_clang(text)
#  define base_disable_warning_gcc(text)
#  define base_disable_warning_deprecated           base_disable_warning_msvc(4996)
#  define base_disable_warning_float_compare
#  define base_disable_warning_invalid_offsetof
#elif defined(base_cpp_clang)
#  define base_warning_push                         base_do_pragma(clang diagnostic push)
#  define base_warning_pop                          base_do_pragma(clang diagnostic pop)
#  define base_disable_warning_clang(text)          base_do_pragma(clang diagnostic ignored text)
#  define base_disable_warning_gcc(text)
#  define base_disable_warning_intel(number)
#  define base_disable_warning_msvc(number)
#  define base_disable_warning_deprecated           base_disable_warning_clang("-Wdeprecated-declarations")
#  define base_disable_warning_float_compare        base_disable_warning_clang("-Wfloat-equal")
#  define base_disable_warning_invalid_offsetof     base_disable_warning_clang("-Winvalid-offsetof")
#elif defined(base_cpp_gnu) && (__GNUC__ * 100 + __GNUC_MINOR__ >= 406)
#  define base_warning_push                         base_do_pragma(GCC diagnostic push)
#  define base_warning_pop                          base_do_pragma(GCC diagnostic pop)
#  define base_disable_warning_gcc(text)            base_do_pragma(GCC diagnostic ignored text)
#  define base_disable_warning_clang(text)
#  define base_disable_warning_intel(number)
#  define base_disable_warning_msvc(number)
#  define base_disable_warning_deprecated           base_warning_disable_gcc("-Wdeprecated-declarations")
#  define base_disable_warning_float_compare        base_warning_disable_gcc("-Wfloat-equal")
#  define base_disable_warning_invalid_offsetof     base_warning_disable_gcc("-Winvalid-offsetof")
#else       // Все остальные компиляторы, GCC < 4.6 и MSVC < 2008
#  define base_warning_push
#  define base_warning_pop
#  define base_disable_warning_intel(number)
#  define base_disable_warning_msvc(number)
#  define base_disable_warning_clang(text)
#  define base_disable_warning_gcc(text)
#  define base_disable_warning_deprecated
#  define base_disable_warning_float_compare
#  define base_disable_warning_invalid_offsetof
#endif

#if defined(base_cpp_msvc)

#define base_deprecated_warning(message)                                            \
    __pragma(warning(push))                                                         \
    __pragma(warning(disable: 4996))                                                \
    __pragma(message (__FILE__ "(" __LINE__ ") : warning C4996: " message))         \
    __pragma(warning(pop))

#elif defined(base_cpp_clang)

#define base_deprecated_warning(message)                                            \
    base_do_pragma("clang diagnostic push")                                         \
    base_do_pragma("clang diagnostic warning \"-Wdeprecated-declarations\"")        \
    base_do_pragma("clang diagnostic ignored \"-Wunused-but-set-variable\"")        \
    base_do_pragma("message \"" __FILE__ "(" __LINE__ ") : warning: " message "\"") \
    base_do_pragma("clang diagnostic pop")

#elif defined(base_cpp_gnu)

#define base_deprecated_warning(message)                                            \
    base_do_pragma("GCC diagnostic push")                                           \
    base_do_pragma("GCC diagnostic warning \"-Wdeprecated-declarations\"")          \
    base_do_pragma("message \"" __FILE__ "(" __LINE__ ") : warning: " message "\"") \ 
    base_do_pragma("GCC diagnostic pop")

#else

#define base_deprecated_warning(message)

#endif

#define base_cast_ignore_align(body) base_warning_push base_disable_warning_gcc("-Wcast-align") body base_warning_pop

#define base_offsetof(Class, member)                        \
    []() -> size_t {                                        \
        base_warning_push WARNING_DISABLE_INVALID_OFFSETOF  \
        return offsetof(Class, member);                     \
        base_warning_pop                                    \
    }()


# ifdef __cplusplus

#if !defined(PROCESSOR_X86)
#  undef lib_base_enable_sse2
#  undef lib_base_enable_sse3
#  undef lib_base_enable_ssse3
#  undef lib_base_enable_sse4_1
#  undef lib_base_enable_sse4_2
#  undef lib_base_enable_avx
#  undef lib_base_enable_avx2
#  undef lib_base_enable_f16c // ????????
#endif


#if !defined(base_cpp_warnings)
#  define base_no_warnings
#endif
#if defined(base_no_warnings)
#  if defined(base_cpp_msvc)
base_disable_warning_msvc(4828) /* Файл содержит знак, начинающийся со смещения 0xX, который является недопустимым в текущей исходной кодировке. */
base_disable_warning_msvc(4251) /* класс 'type' должен иметь dll-интерфейс, который будет использоваться клиентами класса 'type2' */
base_disable_warning_msvc(4244) /* преобразование из 'type1' в 'type2', возможная потеря данных */
base_disable_warning_msvc(4275) /* идентификатор ключа класса, отличного от DLL-интерфейса, используемый в качестве базового для идентификатора ключа класса DLL-интерфейса */
base_disable_warning_msvc(4514) /* удалена встроенная функция, на которую нет ссылок */
base_disable_warning_msvc(4800) /* 'type' : принудительное присвоение значения bool 'true' или 'false' (предупреждение о производительности) */
base_disable_warning_msvc(4097) /* typedef-имя 'identifier1' используется как синоним имени класса 'identifier2' */
base_disable_warning_msvc(4706) /* присвоение в условном выражении */
base_disable_warning_msvc(4355) /* 'this' : используется в списке инициализаторов базовых элементов */
base_disable_warning_msvc(4710) /* функция не встроена */
base_disable_warning_msvc(4530) /* Используется обработчик исключений C++, но семантика размотки не включена. Укажите /EHsc */
base_disable_warning_msvc(4006)
#endif
#endif

# endif // __cplusplus
