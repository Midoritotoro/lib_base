#pragma once 

#include <base/core/utility/TypeTraits.h>

__BASE_MEMORY_NAMESPACE_BEGIN

#if defined(CPP_MSVC)

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

#elif defined(CPP_GNU)

template <class _Type_>
struct AlignmentOf: 
    std::integral_constant<
        std::size_t,
        __alignof__(_Type_)> 
{};

#elif defined(CPP_CLANG)

template <class _Type_>
struct AlignmentOf:
    std::integral_constant<
        std::size_t,
        __alignof(_Type_)> 
{};

#endif

__BASE_MEMORY_NAMESPACE_END
