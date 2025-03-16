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

#if defined(OS_WIN) && !defined(aligned_malloc)
#  define aligned_malloc		                _aligned_malloc
#elif !defined(aligned_malloc)
#  define aligned_malloc(size, alignment)		malloc(size)
#endif

#if defined(OS_WIN) && !defined(aligned_realloc)
#  define aligned_realloc                       _aligned_realloc
#elif !defined(aligned_realloc)
#  define aligned_realloc(block, size, align)   realloc(block, size)
#endif

#if defined (OS_WIN) && !defined(aligned_free)
#  define aligned_free(ptr)                     _aligned_free(ptr)
#elif !defined(aligned_free)
#  define aligned_free(ptr)                     free(ptr)
#endif

#ifndef MEMORY_DEFAULT_ALIGNMENT
#  define MEMORY_DEFAULT_ALIGNMENT MINIMUM_ACCEPTABLE_SIMD_ALIGNMENT
#endif


__BASE_MEMORY_NAMESPACE_END