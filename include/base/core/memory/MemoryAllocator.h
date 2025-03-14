#pragma once 

#include <base/core/arch/Platform.h>
#include <base/core/memory/Memory.h>


__BASE_MEMORY_NAMESPACE_BEGIN

class MemoryAllocator {
	void* malloc(sizetype size) MALLOC_ATTRIBUTE ALLOC_SIZE(1);

	void* mallocz(sizetype size) MALLOC_ATTRIBUTE ALLOC_SIZE(1);
	ALLOC_SIZE(1, 2) void* malloc_array(sizetype nmemb, sizetype size);

	void* calloc(sizetype nmemb, sizetype size) MALLOC_ATTRIBUTE ALLOC_SIZE(1, 2);

	void* realloc(void* ptr, sizetype size) ALLOC_SIZE(2);

	int reallocp(void* ptr, sizetype size);

	void* realloc_f(void* ptr, sizetype nelem, sizetype elsize);

	ALLOC_SIZE(2, 3) void* realloc_array(void* ptr, sizetype nmemb, sizetype size);

	int reallocp_array(void* ptr, sizetype nmemb, sizetype size);

	void* fast_realloc(void* ptr, unsigned int* size, sizetype min_size);
	void fast_malloc(void* ptr, unsigned int* size, sizetype min_size);

	void fast_mallocz(void* ptr, unsigned int* size, sizetype min_size);

	void free(void* ptr);

	void freep(void* ptr);

	void* memdup(const void* p, sizetype size);
	void memcpy_backptr(uint8_t* dst, int back, int cnt);

	void dynarray_add(void* tab_ptr, int* nb_ptr, void* elem);

	int dynarray_add_nofree(void* tab_ptr, int* nb_ptr, void* elem);

	void* dynarray2_add(void** tab_ptr, int* nb_ptr, sizetype elem_size,
		const uint8_t* elem_data);

	int size_mult(sizetype a, sizetype b, sizetype* r);

	void max_alloc(sizetype max);
};

__BASE_MEMORY_NAMESPACE_END