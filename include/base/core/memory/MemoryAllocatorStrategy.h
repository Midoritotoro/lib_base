#pragma once 

#include <base/core/arch/Platform.h> 

#include <base/core/utility/OverflowCheck.h>
#include <base/core/thread/CommonAtomicOperations.h>


__BASE_MEMORY_NAMESPACE_BEGIN

template <typename _Type>
class RawMemoryAllocatorStrategy {
public:
	using value_type				= _Type;
	using difference_type			= ptrdiff;

	using size_type					= sizetype;
	using _AtomicOperationsForSize	= thread::AtomicOperations<size_type>;

	~RawMemoryAllocatorStrategy() = delete;
	RawMemoryAllocatorStrategy() = delete;

	template <class _OtherType>
	RawMemoryAllocatorStrategy(const RawMemoryAllocatorStrategy<_OtherType>&) = delete;
	RawMemoryAllocatorStrategy(const RawMemoryAllocatorStrategy& other) = delete;

	RawMemoryAllocatorStrategy& operator=(const RawMemoryAllocatorStrategy&) = delete;


	static CONSTEXPR_CXX20 NODISCARD_RETURN_RAW_PTR
		inline DECLARE_MEMORY_ALLOCATOR
	value_type* Allocate(size_type bytes) ALLOC_SIZE(1)
	{
		value_type* pointer = nullptr;

		if (bytes <= 0 || bytes > _AtomicOperationsForSize::loadRelaxed(&MaximumAllocationSize))
			return nullptr;

#if defined(OS_MAC) || defined(OS_LINUX)
		if (bytes)
			if (posix_memalign(&pointer, MEMORY_DEFAULT_ALIGNMENT, bytes))
				pointer = nullptr;
#elif defined (OS_WIN)
		pointer = aligned_malloc(bytes, MEMORY_DEFAULT_ALIGNMENT);
#else
		pointer = malloc(bytes);
#endif
		if (!pointer && !bytes) {
			bytes = 1;
			pointer = Allocate(1);
		}

		return pointer;
	}

	static CONSTEXPR_CXX20 NODISCARD_RETURN_RAW_PTR
		inline DECLARE_MEMORY_ALLOCATOR
	value_type* AllocateZeros(size_type bytes) ALLOC_SIZE(1)
	{
		value_type* pointer = Allocate(bytes);

		if (pointer)
			memset(pointer, 0, bytes);

		return pointer;
	}

	static CONSTEXPR_CXX20 NODISCARD_RETURN_RAW_PTR
		inline DECLARE_MEMORY_ALLOCATOR ALLOC_SIZE(1, 2)
		value_type* AllocateArray( 
			size_type numberOfElements,
			size_type singleElementSize)
	{
		size_type result = 0;

		if (MultiplyOverflow(numberOfElements, singleElementSize, &result))
			return nullptr;

		return Allocate(result);
	}

	static CONSTEXPR_CXX20 NODISCARD_RETURN_RAW_PTR
		inline DECLARE_MEMORY_ALLOCATOR
	value_type* AllocateArrayAligned(
		size_type numberOfElements,
		size_type singleElementSize) ALLOC_SIZE(1, 2)
	{
		size_type result = 0;

		if (MultiplyOverflow(numberOfElements, singleElementSize, &result))
			return nullptr;

		return Allocate(result); // ? 
	}

	static CONSTEXPR_CXX20 NODISCARD_RETURN_RAW_PTR
		inline DECLARE_MEMORY_ALLOCATOR
	value_type* ReAllocate(
		value_type* pointer,
		size_type bytes) ALLOC_SIZE(2)
	{
		value_type* resultPointer = nullptr;

		if (bytes <= 0 || bytes > _AtomicOperationsForSize::loadRelaxed(&MaximumAllocationSize))
			return nullptr;

		return aligned_realloc(
			pointer, bytes + !bytes, 
			MEMORY_DEFAULT_ALIGNMENT);
	}

	static CONSTEXPR_CXX20 NODISCARD_RETURN_RAW_PTR
		inline DECLARE_MEMORY_ALLOCATOR
	value_type* ReAllocateFromAny(
		void* pointer,
		size_type bytes) ALLOC_SIZE(2)
	{
		return ReAllocate(reinterpret_cast<value_type*>(pointer), bytes);
	}

	static CONSTEXPR_CXX20 NODISCARD_RETURN_RAW_PTR
		inline ALLOC_SIZE(2, 3) DECLARE_MEMORY_ALLOCATOR
	value_type* ReallocateArray(
		value_type* pointer,
		size_type numberOfElements,
		size_type singleElementSize)
	{
		size_type result = 0;

		if (MultiplyOverflow(numberOfElements, singleElementSize, &result))
			return nullptr;

		return ReAllocate(pointer, result);
	}

	static CONSTEXPR_CXX20 NODISCARD_RETURN_RAW_PTR
		inline ALLOC_SIZE(2, 3) DECLARE_MEMORY_ALLOCATOR
	value_type* ReallocateArrayFromAny(
		void* pointer,
		size_type numberOfElements,
		size_type singleElementSize)
	{
		return ReallocateArray(
			reinterpret_cast<value_type*>(pointer),
			numberOfElements, singleElementSize);
	}

	static CONSTEXPR_CXX20 NODISCARD_RETURN_RAW_PTR
		inline DECLARE_MEMORY_ALLOCATOR
	value_type* FastReallocate(
		value_type* pointer,
		uint* size,
		size_type minimumSize)
	{
		size_type maximumSize = 0;

		if (minimumSize <= *size)
			return pointer;

		maximumSize = _AtomicOperationsForSize::loadRelaxed(&MaximumAllocationSize);
		maximumSize = std::min(maximumSize, UINT_MAX);

		if (minimumSize > maximumSize) {
			*size = 0;
			return nullptr;
		}

		minimumSize = std::min(maximumSize,
			std::max(minimumSize + minimumSize / 16 + 32,
				minimumSize));

		pointer = ReAllocate(pointer, minimumSize);

		if (!pointer)
			minimumSize = 0;

		*size = minimumSize;

		return pointer;
	}

	static CONSTEXPR_CXX20 inline
		DECLARE_MEMORY_ALLOCATOR
	void FastMalloc(
		value_type* pointer,
		uint* size,
		size_type minimumSize,
		bool zeroReallocate)
	{
		size_type maximumSize = 0;
		value_type* value = nullptr;

		memcpy(&value, pointer, sizeof(value));

		if (minimumSize <= *size)
			//Assert(value || !minimumSize);
			return;

		maximumSize = _AtomicOperationsForSize::loadRelaxed(&MaximumAllocationSize);
		/* *size is an unsigned, so the real maximum is <= UINT_MAX. */
		maximumSize = FFMIN(maximumSize, UINT_MAX);

		if (minimumSize > maximumSize) {
			FreeNull(pointer);
			*size = 0;

			return;
		}

		minimumSize = std::min(maximumSize, 
			std::max(minimumSize + minimumSize / 16 + 32, 
				minimumSize));

		FreeNull(pointer);

		value = zeroReallocate 
			? AllocateZeros(minimumSize) 
			: Allocate(minimumSize);

		memcpy(pointer, &value, sizeof(value));

		if (!value)
			minimumSize = 0;

		*size = minimumSize;
		return;
	}

	static CONSTEXPR_CXX20 inline
		DECLARE_MEMORY_ALLOCATOR
	void FastMallocZeros(
		value_type* pointer,
		uint* size,
		size_type minimumSize)
	{
		FastMalloc(pointer, size,
			minimumSize, true);
	}

	static CONSTEXPR_CXX20 inline
		void Deallocate(value_type* pointer)
	{
		aligned_free(pointer);
	}

	static CONSTEXPR_CXX20 inline
		void Free(value_type* pointer)
	{
		Deallocate(pointer);
	}

	static CONSTEXPR_CXX20 inline
		void FreeNull(value_type* pointer)
	{
		void* value = nullptr;

		memcpy(&value, pointer, sizeof(value));
		memcpy(pointer, &(void*){ NULL }, sizeof(value));

		Free(value);
	}

	static CONSTEXPR_CXX20 NODISCARD_RETURN_RAW_PTR
		DECLARE_MEMORY_ALLOCATOR inline
		value_type* MemoryDuplicate(
			const value_type* pointer,
			sizetype size)
	{
		value_type* outPointer = nullptr;

		if (!pointer)
			return nullptr;

		outPointer = Allocate(size);

		if (outPointer)
			memcpy(outPointer, pointer, size);
		
		return outPointer;
	}
private:
	static std::atomic<size_type> MaximumAllocationSize = INT_MAX;
};

template <typename _Type>
class DefaultMemoryAllocatorStrategy {
public:

};

__BASE_MEMORY_NAMESPACE_END
