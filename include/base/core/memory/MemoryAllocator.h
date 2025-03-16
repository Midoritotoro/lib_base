#pragma once 

#include <base/core/arch/Platform.h>
#include <base/core/memory/Memory.h>

__BASE_MEMORY_NAMESPACE_BEGIN


template <
	class _AllocatorStrategy_,
	typename _Type>
class MemoryAllocator {
	static_assert(!std::is_const_v<_Type>, "The C++ Standard forbids containers of const elements "
		"because allocator<const T> is ill-formed.");

	static_assert(!std::is_function_v<_Type>, "The C++ Standard forbids allocators for function elements "
		"because of [allocator.requirements].");

	static_assert(!std::is_reference_v<_Type>, "The C++ Standard forbids allocators for reference elements "
		"because of [allocator.requirements].");
public:
	using _AllocatorStrategyForType		= _AllocatorStrategy_<_Type>;
	using value_type					= _Type;

	using difference_type				= ptrdiff;
	using size_type						= sizetype;


	CONSTEXPR_CXX20 ~MemoryAllocator() noexcept = default;

	constexpr MemoryAllocator() noexcept = default;
	CONSTEXPR_CXX20 MemoryAllocator(const MemoryAllocator& other) noexcept = default;

	template <class _Other>
	constexpr MemoryAllocator(const MemoryAllocator<_Other>&) noexcept = default;

	CONSTEXPR_CXX20 MemoryAllocator& operator=(const allocator&) noexcept = default;

	CONSTEXPR_CXX20 NODISCARD_RETURN_RAW_PTR
		inline DECLARE_MEMORY_ALLOCATOR 
		value_type* Allocate(size_type bytes) ALLOC_SIZE(1) // malloc
	{
		return _AllocatorStrategyForType::Allocate(bytes);
	}

	CONSTEXPR_CXX20 NODISCARD_RETURN_RAW_PTR
		inline DECLARE_MEMORY_ALLOCATOR 
		value_type* AllocateZ(size_type bytes) ALLOC_SIZE(1) // mallocz
	{
		return _AllocatorStrategyForType::AllocateZ(bytes);
	}

	CONSTEXPR_CXX20 NODISCARD_RETURN_RAW_PTR
		inline DECLARE_MEMORY_ALLOCATOR ALLOC_SIZE(1, 2) 
		value_type* AllocateArray(
			sizetype nmemb,
			sizetype bytes)
	{
		return _AllocatorStrategyForType::AllocateArray(bytes);
	}

	CONSTEXPR_CXX20 NODISCARD_RETURN_RAW_PTR
		inline DECLARE_MEMORY_ALLOCATOR 
		value_type* AllocateArrayAligned(
			sizetype nmemb, 
			sizetype size) ALLOC_SIZE(1, 2)
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

#include <base/core/memory/RawMemoryAllocatorStrategy.h>
#include <base/core/memory/DefaultMemoryAllocatorStrategy.h>

__BASE_MEMORY_NAMESPACE_BEGIN

template <typename _Type>
class DefaultMemoryAllocator :
	public MemoryAllocator<DefaultMemoryAllocatorStrategy, _Type>
{

};

template <typename _Type>
class RawMemoryAllocator :
	public MemoryAllocator<RawMemoryAllocatorStrategy, _Type>
{

};

__BASE_MEMORY_NAMESPACE_END