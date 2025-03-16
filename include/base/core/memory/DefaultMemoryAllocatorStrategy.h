#pragma once 

#include <base/core/utility/OverflowCheck.h>
#include <base/core/thread/CommonAtomicOperations.h>

#include <base/core/utility/TypeTraits.h>
#include <base/core/memory/Memory.h>


__BASE_MEMORY_NAMESPACE_BEGIN

template <typename _Type>
class DefaultMemoryAllocatorStrategy {
public:
	using value_type = _Type;
	using difference_type = ptrdiff;

	using size_type = sizetype;
	using _AtomicOperationsForSize = thread::AtomicOperations<size_type>;

	using pointer = value_type*;
	using const_pointer = const value_type*;

	~DefaultMemoryAllocatorStrategy() = delete;
	DefaultMemoryAllocatorStrategy() = delete;

	template <class _OtherType>
	DefaultMemoryAllocatorStrategy(const DefaultMemoryAllocatorStrategy<_OtherType>&) = delete;
	DefaultMemoryAllocatorStrategy(const DefaultMemoryAllocatorStrategy& other) = delete;

	DefaultMemoryAllocatorStrategy& operator=(const DefaultMemoryAllocatorStrategy&) = delete;


	static NODISCARD_RETURN_RAW_PTR
	inline DECLARE_MEMORY_ALLOCATOR
	CLANG_CONSTEXPR_CXX20 // Clang и MSVC реализуют P0784R7 по-разному; см. GH-1532
	value_type* Allocate(size_type bytes) {
		return reinterpret_cast<value_type*>(::operator new(bytes));
	}
	
#ifdef __cpp_aligned_new
	static NODISCARD_RETURN_RAW_PTR
	inline DECLARE_MEMORY_ALLOCATOR
	CLANG_CONSTEXPR_CXX20
	value_type* AllocateAligned(
		size_type bytes,
		size_type alignment) 
	{
#  ifdef CPP_CLANG
#    if BASE_HAS_CXX20
		if (is_constant_evaluated())
			return reinterpret_cast<value_type*>(::operator new(bytes));
		else
#    endif
#  endif
			return reinterpret_cast<value_type*>(::operator new(bytes, std::align_val_t{ alignment }));
	}
#else 
	static NODISCARD_RETURN_RAW_PTR
	inline DECLARE_MEMORY_ALLOCATOR
	CLANG_CONSTEXPR_CXX20
	value_type* AllocateAligned(
		size_type bytes,
		size_type alignment) 
	{
		UNUSED(alignment);
		return Allocate(bytes);
	}
#endif 

	static NODISCARD_RETURN_RAW_PTR
	inline DECLARE_MEMORY_ALLOCATOR
	CLANG_CONSTEXPR_CXX20
	value_type* AllocateZeros(size_type bytes)
	{
		value_type* pointer = Allocate(bytes);

		if (pointer)
			memset(pointer, 0, bytes);

		return pointer;
	}

	static NODISCARD_RETURN_RAW_PTR
	inline DECLARE_MEMORY_ALLOCATOR
	CLANG_CONSTEXPR_CXX20
	value_type* AllocateZerosAligned(
		size_type bytes,
		size_type alignment)
	{
		value_type* pointer = AllocateAligned(
			bytes, alignment);

		if (pointer)
			memset(pointer, 0, bytes);

		return pointer;
	}

	static NODISCARD_RETURN_RAW_PTR
	inline DECLARE_MEMORY_ALLOCATOR
	CLANG_CONSTEXPR_CXX20
	value_type* AllocateArray(
		size_type numberOfElements,
		size_type singleElementSize)
	{
		size_type result = 0;

		if (MultiplyOverflow(numberOfElements, singleElementSize, &result))
			return nullptr;

		return Allocate(result);
	}

	static NODISCARD_RETURN_RAW_PTR
	inline DECLARE_MEMORY_ALLOCATOR
	CLANG_CONSTEXPR_CXX20
	value_type* AllocateArrayAligned(
		size_type numberOfElements,
		size_type singleElementSize,
		size_type alignment)
	{
		size_type result = 0;

		if (MultiplyOverflow(numberOfElements, singleElementSize, &result))
			return nullptr;

		return AllocateAligned(result, alignment);
	}

	static NODISCARD_RETURN_RAW_PTR
	inline DECLARE_MEMORY_ALLOCATOR
	CLANG_CONSTEXPR_CXX20
	value_type* ReAllocate(
		value_type* pointer,
		size_type bytes)
	{
		value_type* resultPointer = nullptr;

		if (bytes > _AtomicOperationsForSize::loadRelaxed(MaximumAllocationSize))
			return nullptr;

		return realloc(pointer, bytes + !bytes);
	}

	static NODISCARD_RETURN_RAW_PTR
	inline DECLARE_MEMORY_ALLOCATOR
	CLANG_CONSTEXPR_CXX20
	value_type* ReAllocateAligned(
		value_type* pointer,
		size_type bytes,
		size_type alignment)
	{
		value_type* resultPointer = nullptr;

		if (bytes > _AtomicOperationsForSize::loadRelaxed(MaximumAllocationSize))
			return nullptr;

		return aligned_realloc(
			pointer, bytes + !bytes,
			alignment);
	}

	static NODISCARD_RETURN_RAW_PTR
	inline DECLARE_MEMORY_ALLOCATOR
	CLANG_CONSTEXPR_CXX20
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

	static NODISCARD_RETURN_RAW_PTR
	inline DECLARE_MEMORY_ALLOCATOR
	CLANG_CONSTEXPR_CXX20
	value_type* ReallocateArrayAligned(
		value_type* pointer,
		size_type numberOfElements,
		size_type singleElementSize,
		size_type alignment)
	{
		size_type result = 0;

		if (MultiplyOverflow(numberOfElements, singleElementSize, &result))
			return nullptr;

		return ReAllocateAligned(
			pointer, result, alignment);
	}


	static NODISCARD_RETURN_RAW_PTR
	inline DECLARE_MEMORY_ALLOCATOR
	CLANG_CONSTEXPR_CXX20
	value_type* FastReallocate(
		value_type* pointer,
		uint* size,
		size_type minimumSize)
	{
		size_type maximumSize = 0;

		if (minimumSize <= *size)
			return pointer;

		maximumSize = _AtomicOperationsForSize::loadRelaxed(MaximumAllocationSize);
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

	static NODISCARD_RETURN_RAW_PTR
	inline DECLARE_MEMORY_ALLOCATOR
	CLANG_CONSTEXPR_CXX20
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

		maximumSize = _AtomicOperationsForSize::loadRelaxed(MaximumAllocationSize);
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

	static NODISCARD_RETURN_RAW_PTR
	inline DECLARE_MEMORY_ALLOCATOR
	CLANG_CONSTEXPR_CXX20
	void FastMallocZeros(
		value_type* pointer,
		uint* size,
		size_type minimumSize)
	{
		FastMalloc(pointer, size,
			minimumSize, true);
	}

	static NODISCARD_RETURN_RAW_PTR
	inline DECLARE_MEMORY_ALLOCATOR
	CLANG_CONSTEXPR_CXX20
	void Deallocate(value_type* pointer)
	{
		free(pointer);
	}

	static NODISCARD_RETURN_RAW_PTR
	inline DECLARE_MEMORY_ALLOCATOR
	CLANG_CONSTEXPR_CXX20
	void Free(value_type* pointer)
	{
		Deallocate(pointer);
	}

	static NODISCARD_RETURN_RAW_PTR
	inline DECLARE_MEMORY_ALLOCATOR
	CLANG_CONSTEXPR_CXX20
	void DeallocateAligned(value_type* pointer)
	{
		aligned_free(pointer);
	}

	static NODISCARD_RETURN_RAW_PTR
	inline DECLARE_MEMORY_ALLOCATOR
	CLANG_CONSTEXPR_CXX20
	void FreeAligned(value_type* pointer)
	{
		DeallocateAligned(pointer);
	}

	static NODISCARD_RETURN_RAW_PTR
	inline DECLARE_MEMORY_ALLOCATOR
	CLANG_CONSTEXPR_CXX20
	void FreeNull(value_type* pointer)
	{
		value_type* value = nullptr;

		memcpy(&value, pointer, sizeof(value));
		memcpy(pointer, &reinterpret_cast<value_type*>(nullptr), sizeof(value));

		Free(value);
	}

	static NODISCARD_RETURN_RAW_PTR
	inline DECLARE_MEMORY_ALLOCATOR
	CLANG_CONSTEXPR_CXX20
	void FreeNullAligned(value_type* pointer)
	{
		value_type* value = nullptr;

		memcpy(&value, pointer, sizeof(value));
		memcpy(pointer, &reinterpret_cast<value_type*>(nullptr), sizeof(value));

		FreeAligned(value);
	}

	static NODISCARD_RETURN_RAW_PTR
	inline DECLARE_MEMORY_ALLOCATOR
	CLANG_CONSTEXPR_CXX20
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
	static constexpr inline std::atomic<int> MaximumAllocationSize = INT_MAX;
};

__BASE_MEMORY_NAMESPACE_END
