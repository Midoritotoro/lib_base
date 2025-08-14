#pragma once 

#include <base/core/type_traits/TypeTraits.h>

__BASE_MEMORY_NAMESPACE_BEGIN

template <
    std::size_t A,
    std::size_t B>
struct MinimumSize:
    std::integral_constant<std::size_t, (A < B) ? A : B>
{};

template <
    std::size_t A,
    std::size_t B>
struct MaximumSize:
    std::integral_constant<std::size_t, (A > B) ? A : B>
{};

template <class _Type_>
struct MaximumObjects:
    std::integral_constant<std::size_t, ~static_cast<std::size_t>(0) / sizeof(_Type_)>
{};

#if defined(base_cpp_msvc)

template <class _Type_>
struct offset_value {
    _Type_  first;
    char    value;
    _Type_  second;
};

template <class _Type_>
struct AlignmentOf : 
    MinimumSize<
        sizeof(_Type_), 
        sizeof(offset_value<_Type_>)
            - (sizeof(_Type_) << 1)> 
{};

#elif defined(base_cpp_gnu)

template <class _Type_>
struct AlignmentOf: 
    std::integral_constant<
        std::size_t,
        __alignof__(_Type_)> 
{};

#elif defined(base_cpp_clang)

template <class _Type_>
struct AlignmentOf:
    std::integral_constant<
        std::size_t,
        __alignof(_Type_)> 
{};

#endif

__BASE_MEMORY_NAMESPACE_END
