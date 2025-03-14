#pragma once


#include <base/core/arch/Platform.h>

__BASE_MEMORY_NAMESPACE_BEGIN

#if defined(CPP_GNU)
#if CPP_GNU > 310
#  define MALLOC_ATTRIBUTE __attribute__((__malloc__))
#else
#  define MALLOC_ATTRIBUTE
#endif
#else 
#  define MALLOC_ATTRIBUTE
#endif

#if defined(CPP_GNU)
#if CPP_GNU > 430
#  define ALLOC_SIZE(...) __attribute__((alloc_size(__VA_ARGS__)))
#else
#  define ALLOC_SIZE(...)
#endif
#else 
#  define ALLOC_SIZE(...)
#endif


__BASE_MEMORY_NAMESPACE_END