#pragma once 

#include <base/core/utility/OverflowCheck.h>
#include <base/core/thread/CommonAtomicOperations.h>

#include <base/core/utility/TypeTraits.h>
#include <base/core/memory/Memory.h>

#include <base/core/memory/MemoryRange.h>
#include <base/core/memory/MemoryAllocation.h>

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
			return reinterpret_cast<value_type*>(
				::operator new(bytes));
		else
#    endif
#  endif
			return reinterpret_cast<value_type*>(
				::operator new(bytes, std::align_val_t{ alignment }));
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
private:
	static constexpr inline std::atomic<int> MaximumAllocationSize = INT_MAX;
};

__BASE_MEMORY_NAMESPACE_END
