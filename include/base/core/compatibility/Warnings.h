#pragma once 

#include <base/core/compatibility/Nodiscard.h>


// Warnings

#define base_do_pragma(text)                      _Pragma(#text)

#if defined(base_cpp_msvc) && !defined(base_cpp_clang)
#  undef base_do_pragma
#endif // defined(base_cpp_msvc) && !defined(base_cpp_clang)

#if !defined(base_warning_push)
#  if defined(base_cpp_msvc) && !defined(base_cpp_clang)
#    define base_warning_push __pragma(warning(push))
#  elif defined(base_cpp_clang)
#    define base_warning_push base_do_pragma(clang diagnostic push)
#  elif defined(base_cpp_gnu) && (__GNUC__ * 100 + __GNUC_MINOR__ >= 406)
#    define base_warning_push base_do_pragma(GCC diagnostic push)
#  else 
#    define base_warning_push
#  endif // (defined(base_cpp_msvc) && !defined(base_cpp_clang)) || defined(base_cpp_clang)
         // || defined(base_cpp_gnu) && (__GNUC__ * 100 + __GNUC_MINOR__ >= 406)
#endif // !defined(base_warning_push)


#if !defined(base_warning_pop)
#  if defined(base_cpp_msvc) && !defined(base_cpp_clang)
#    define base_warning_pop __pragma(warning(pop))
#  elif defined(base_cpp_clang)
#    define base_warning_pop base_do_pragma(clang diagnostic pop)
#  elif defined(base_cpp_gnu) && (__GNUC__ * 100 + __GNUC_MINOR__ >= 406)
#    define base_warning_pop base_do_pragma(GCC diagnostic pop)
#  else
#    define base_warning_pop
#  endif // (defined(base_cpp_msvc) && !defined(base_cpp_clang)) || defined(base_cpp_clang)
         // || defined(base_cpp_gnu) && (__GNUC__ * 100 + __GNUC_MINOR__ >= 406)
#endif // !defined(base_warning_pop)


#if !defined(base_disable_warning_msvc)
#  if defined(base_cpp_msvc) && !defined(base_cpp_clang)
#    define base_disable_warning_msvc(number) __pragma(warning(disable: number))
#  else
#    define base_disable_warning_msvc(number)
#  endif // defined(base_cpp_msvc) && !defined(base_cpp_clang)
#endif // !defined(base_disable_warning_msvc)


#if !defined(base_disable_warning_clang)
#  if defined(base_cpp_clang)
#    define base_disable_warning_clang(text) base_do_pragma(clang diagnostic ignored text)
#  else
#    define base_disable_warning_clang(text)
#  endif // defined(base_cpp_msvc) && !defined(base_cpp_clang)
#endif // !defined(base_disable_warning_clang)


#if !defined(base_disable_warning_gcc)
#  if defined(base_cpp_gnu) && (__GNUC__ * 100 + __GNUC_MINOR__ >= 406)
#    define base_disable_warning_gcc(text) base_do_pragma(GCC diagnostic ignored text)
#  else
#    define base_disable_warning_gcc(text) 
#  endif // defined(base_cpp_msvc) && !defined(base_cpp_clang)
#endif // !defined(base_disable_warning_gcc)


#if !defined(base_disable_warning_deprecated)
#  if defined(base_cpp_msvc) && !defined(base_cpp_clang)
#    define base_disable_warning_deprecated base_disable_warning_msvc(4996)
#  elif defined(base_cpp_clang)
#    define base_disable_warning_deprecated base_disable_warning_clang("-Wdeprecated-declarations")
#  elif defined(base_cpp_gnu) && (__GNUC__ * 100 + __GNUC_MINOR__ >= 406)
#    define base_disable_warning_deprecated base_warning_disable_gcc("-Wdeprecated-declarations")
#  else
#    define base_disable_warning_deprecated
#  endif // (defined(base_cpp_msvc) && !defined(base_cpp_clang)) || defined(base_cpp_clang)
         // || defined(base_cpp_gnu) && (__GNUC__ * 100 + __GNUC_MINOR__ >= 406)
#endif // !defined(base_disable_warning_deprecated)


#if !defined(base_cpp_warnings)
#  define base_no_warnings
#endif // !defined(base_cpp_warnings)


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
#  endif // defined(base_cpp_msvc)

#endif // defined(base_no_warnings)

#if !defined( _return_raw_ptr)
#  define  _return_raw_ptr \
         _with_warning("This function allocates memory and returns a raw pointer. " \
            "Discarding the return value will cause a memory leak.")
#endif // !defined( _return_raw_ptr)


#if !defined( _thread_constructor)
#  define  _thread_constructor \
     _constructor_with_warning("Creating a thread object without assigning it to a variable " \
        "may lead to unexpected behavior and resource leaks. Ensure " \
        "the thread is properly managed.")
#endif // !defined( _thread_constructor)


#if !defined( _remove_algorithm)
#  define  _remove_algorithm \
         _with_warning("The 'remove' and 'remove_if' algorithms return the iterator past the last element " \
            "that should be kept. You need to call container.erase(result, container.end()) afterwards. " \
            "In C++20, 'std::erase' and 'std::erase_if' are simpler replacements for these two steps.")
#endif // !defined( _remove_algorithm)


#if !defined(base_deprecated_warning)
#  if defined(base_cpp_msvc)
#    define base_deprecated_warning(message)                                           \
       __pragma(warning(push))                                                         \
       __pragma(warning(disable: 4996))                                                \
       __pragma(message (__FILE__ "(" __LINE__ ") : warning C4996: " message))         \
       __pragma(warning(pop))
#  elif defined(base_cpp_clang)
#    define base_deprecated_warning(message)                                           \
       base_do_pragma("clang diagnostic push")                                         \
       base_do_pragma("clang diagnostic warning \"-Wdeprecated-declarations\"")        \
       base_do_pragma("clang diagnostic ignored \"-Wunused-but-set-variable\"")        \
       base_do_pragma("message \"" __FILE__ "(" __LINE__ ") : warning: " message "\"") \
       base_do_pragma("clang diagnostic pop")
#  elif defined(base_cpp_gnu)
#    define base_deprecated_warning(message)                                           \
       base_do_pragma("GCC diagnostic push")                                           \
       base_do_pragma("GCC diagnostic warning \"-Wdeprecated-declarations\"")          \
       base_do_pragma("message \"" __FILE__ "(" __LINE__ ") : warning: " message "\"") \ 
       base_do_pragma("GCC diagnostic pop")
#  else
#    define base_deprecated_warning(message)
#  endif // defined(base_cpp_msvc) || defined(base_cpp_clang) || defined(base_cpp_gnu)
#endif // !defined(base_deprecated_warning)