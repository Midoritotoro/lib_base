#pragma once


#include <base/core/arch/Platform.h>

__BASE_MEMORY_NAMESPACE_BEGIN

#if defined(CPP_GNU)
#  if CPP_GNU > 430
#    define ALLOC_SIZE(...) __attribute__((alloc_size(__VA_ARGS__)))
#  else
#    define ALLOC_SIZE(...)
#  endif
#  else 
#    define ALLOC_SIZE(...)
#endif

#ifndef MEMORY_DEFAULT_ALIGNMENT
#  define MEMORY_DEFAULT_ALIGNMENT MINIMUM_ACCEPTABLE_SIMD_ALIGNMENT
#endif


__BASE_MEMORY_NAMESPACE_END