#pragma once 

#include <base/core/arch/Platform.h>
#include <base/core/memory/Memory.h>
#include <base/core/memory/MemoryAllocatorStrategy.h>

__BASE_MEMORY_NAMESPACE_BEGIN


template <
	typename _Type,
	class _AllocatorStrategy_>
class MemoryAllocator {
public:
	using _AllocatorStrategyForType		= _AllocatorStrategy_<_Type>;
	using value_type					= _Type;

	using difference_type				= ptrdiff;
	using size_type						= sizetype;

	CONSTEXPR_CXX20 NODISCARD_RETURN_RAW_PTR
		inline DECLARE_MEMORY_ALLOCATOR 
		value_type* Allocate(size_type bytes) ALLOC_SIZE(1) // malloc
	{
		return _AllocatorStrategyFprType::Allocate(bytes);
	}

	CONSTEXPR_CXX20 NODISCARD_RETURN_RAW_PTR
		inline DECLARE_MEMORY_ALLOCATOR 
		value_type* AllocateZ(size_type bytes) ALLOC_SIZE(1) // mallocz
	{
		return _AllocatorStrategyForType::AllocateZ(bytes);
	}

	CONSTEXPR_CXX20 NODISCARD_RETURN_RAW_PTR
		inline DECLARE_MEMORY_ALLOCATOR ALLOC_SIZE(1, 2) 
		value_type* AllocateArray( // malloc_array
			sizetype nmemb,
			sizetype bytes)
	{
		return _AllocatorStrategyForType::AllocateArray(bytes);
	}


	CONSTEXPR_CXX20 NODISCARD_RETURN_RAW_PTR
		inline DECLARE_MEMORY_ALLOCATOR 
		value_type* AllocateArrayAligned(
			sizetype nmemb, 
			sizetype size) ALLOC_SIZE(1, 2) // calloc
	{

	}

	CONSTEXPR_CXX20 NODISCARD_RETURN_RAW_PTR
		inline DECLARE_MEMORY_ALLOCATOR 
		value_type* ReAllocate(
			value_type* pointer,
			sizetype size) ALLOC_SIZE(2) // realloc
	{
		return _AllocatorStrategyForType::ReAllocate(
			pointer, size);
	}

	CONSTEXPR_CXX20 NODISCARD_RETURN_RAW_PTR
		inline DECLARE_MEMORY_ALLOCATOR
	value_type* ReAllocateFromAny(
		void* pointer,
		sizetype size) ALLOC_SIZE(2)
	{
		return _AllocatorStrategyForType::ReAllocateFromAny(
			pointer, size);
	}

	CONSTEXPR_CXX20 NODISCARD_RETURN_RAW_PTR
		inline ALLOC_SIZE(2, 3) DECLARE_MEMORY_ALLOCATOR
		value_type* ReallocateArray(
			value_type* pointer,
			sizetype nmemb,
			sizetype size)
	{
		return _AllocatorStrategyForType::ReallocateArray(
			pointer, nmemb, size);
	}

	CONSTEXPR_CXX20 NODISCARD_RETURN_RAW_PTR
		inline ALLOC_SIZE(2, 3) DECLARE_MEMORY_ALLOCATOR
		value_type* ReallocateArrayFromAny(
			void* pointer, 
			sizetype nmemb, 
			sizetype size)
	{
		return _AllocatorStrategyForType::ReallocateArray(
			pointer, nmemb, size);
	}

	CONSTEXPR_CXX20 NODISCARD_RETURN_RAW_PTR
		inline DECLARE_MEMORY_ALLOCATOR 
		value_type* FastReallocate(
			value_type* pointer,
			uint* size,
			sizetype minimumSize)
	{
		return _AllocatorStrategyForType::FastReallocate(
			pointer, size, minimumSize);
	}


	CONSTEXPR_CXX20 inline
		DECLARE_MEMORY_ALLOCATOR
		void FastMalloc(
			value_type* pointer,
			uint* size, 
			sizetype minimumSize)
	{
		_AllocatorStrategyForType::FastMalloc(
			pointer, size, minimumSize);
	}

	CONSTEXPR_CXX20 inline
		DECLARE_MEMORY_ALLOCATOR
	void FastMallocZeros(
		value_type* pointer,
		uint* size,
		sizetype minimumSize)
	{
		_AllocatorStrategyForType::FastMallocZeros(
			pointer, size, minimumSize);
	}

	CONSTEXPR_CXX20 inline
		void Deallocate(value_type* pointer)
	{
		 _AllocatorStrategyForType::Deallocate(pointer);
	}

	CONSTEXPR_CXX20 inline
		void Free(value_type* pointer)
	{
		_AllocatorStrategyForType::Deallocate(pointer);
	}

	CONSTEXPR_CXX20 inline
		void FreeNull(value_type* pointer)
	{
		_AllocatorStrategyForType::Deallocate(pointer);
		pointer = nullptr;
	}

	CONSTEXPR_CXX20 NODISCARD_RETURN_RAW_PTR
		DECLARE_MEMORY_ALLOCATOR inline 
		value_type* MemoryDuplicate(
			const value_type* pointer,
			sizetype size)
	{
		return _AllocatorStrategyForType::MemoryDuplicate(
			pointer, size);
	}
};

__BASE_MEMORY_NAMESPACE_END