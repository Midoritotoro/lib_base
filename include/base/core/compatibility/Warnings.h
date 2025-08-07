#pragma once 

#include <base/core/compatibility/Nodiscard.h>


// Warnings


#if !defined(base_nodiscard_return_raw_ptr)
#  define base_nodiscard_return_raw_ptr \
        base_nodiscard_with_warning("This function allocates memory and returns a raw pointer. " \
            "Discarding the return value will cause a memory leak.")
#endif // !defined(base_nodiscard_return_raw_ptr)


#if !defined(base_nodiscard_thread_constructor)
#  define base_nodiscard_thread_constructor \
    base_nodiscard_constructor_with_warning("Creating a thread object without assigning it to a variable " \
        "may lead to unexpected behavior and resource leaks. Ensure " \
        "the thread is properly managed.")
#endif // !defined(base_nodiscard_thread_constructor)


#if !defined(base_nodiscard_remove_algorithm)
#  define base_nodiscard_remove_algorithm \
        base_nodiscard_with_warning("The 'remove' and 'remove_if' algorithms return the iterator past the last element " \
            "that should be kept. You need to call container.erase(result, container.end()) afterwards. " \
            "In C++20, 'std::erase' and 'std::erase_if' are simpler replacements for these two steps.")
#endif // !defined(base_nodiscard_remove_algorithm)
