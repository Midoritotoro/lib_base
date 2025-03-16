#pragma once 

#include <base/core/utility/OverflowCheck.h>
#include <base/core/thread/CommonAtomicOperations.h>

#include <base/core/utility/TypeTraits.h>


__BASE_MEMORY_NAMESPACE_BEGIN

template <typename _Type>
class DefaultMemoryAllocatorStrategy {
public:
	using value_type = _Type;
	using difference_type = ptrdiff;

	using size_type = sizetype;
	using _AtomicOperationsForSize = thread::AtomicOperations<size_type>;

	~DefaultMemoryAllocatorStrategy() = delete;
	DefaultMemoryAllocatorStrategy() = delete;

	template <class _OtherType>
	DefaultMemoryAllocatorStrategy(const DefaultMemoryAllocatorStrategy<_OtherType>&) = delete;
	DefaultMemoryAllocatorStrategy(const DefaultMemoryAllocatorStrategy& other) = delete;

	DefaultMemoryAllocatorStrategy& operator=(const DefaultMemoryAllocatorStrategy&) = delete;


	static NODISCARD_RETURN_RAW_PTR
	inline DECLARE_MEMORY_ALLOCATOR
#ifdef CPP_CLANG // Clang and MSVC implement P0784R7 differently; see GH-1532
		CONSTEXPR_CXX20
#endif
	value_type* Allocate(size_type bytes) ALLOC_SIZE(1) {
		return ::operator new(bytes);
	}

#ifdef __cpp_aligned_new
	static NODISCARD_RETURN_RAW_PTR
	inline DECLARE_MEMORY_ALLOCATOR
#  ifdef CPP_CLANG // Clang and MSVC implement P0784R7 differently; see GH-1532
	CONSTEXPR_CXX20
#  endif
	value_type* AllocateAligned(
		size_type bytes,
		size_type alignment) 
	{
#  ifdef CPP_CLANG // Clang and MSVC implement P0784R7 differently; see GH-1532
#    if BASE_HAS_CXX20
		if (is_constant_evaluated())
			return ::operator new(bytes);
		else
#    endif
#  endif
			return ::operator new(bytes, std::align_val_t{ alignment });
	}
#endif 


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

		if (bytes > _AtomicOperationsForSize::loadRelaxed(&MaximumAllocationSize))
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
			// Assert(value || !minimumSize);
			return;

		maximumSize = _AtomicOperationsForSize::loadRelaxed(&MaximumAllocationSize);
		/* *size is an unsigned, so the real maximum is <= UINT_MAX. */
		maximumSize = std::min(maximumSize, UINT_MAX);

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
		value_type* value = nullptr;

		memcpy(&value, pointer, sizeof(value));
		memcpy(pointer, &reinterpret_cast<value_type*>({ nullptr }), sizeof(value));

		Free(value);
	}

	static CONSTEXPR_CXX20 NODISCARD_RETURN_RAW_PTR
		DECLARE_MEMORY_ALLOCATOR inline
		value_type* MemoryDuplicate(
			const value_type* pointer,
			sizetype size)
	{
		if (!pointer)
			return nullptr;

		value_type* outPointer = Allocate(size);

		if (outPointer)
			memcpy(outPointer, pointer, size);

		return outPointer;
	}
private:
	static std::atomic<size_type> MaximumAllocationSize = INT_MAX;
};

__BASE_MEMORY_NAMESPACE_END
