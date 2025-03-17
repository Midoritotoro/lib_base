#pragma once 

#include <base/core/memory/RawMemoryAllocatorStrategy.h>
#include <base/core/memory/DefaultMemoryAllocatorStrategy.h>


__BASE_MEMORY_NAMESPACE_BEGIN

template <class _AllocatorStrategy_>
class MemoryAllocator {
	static_assert(!std::is_const_v<_AllocatorStrategy_::value_type>, "The C++ Standard forbids containers of const elements "
		"because allocator<const T> is ill-formed.");

	static_assert(!std::is_function_v<_AllocatorStrategy_::value_type>, "The C++ Standard forbids allocators for function elements "
		"because of [allocator.requirements].");

	static_assert(!std::is_reference_v<_AllocatorStrategy_::value_type>, "The C++ Standard forbids allocators for reference elements "
		"because of [allocator.requirements].");
public:
	using pointer						= typename _AllocatorStrategy_::pointer;
	using const_pointer					= typename _AllocatorStrategy_::const_pointer;

	using value_type					= typename _AllocatorStrategy_::value_type;

	using difference_type				= typename _AllocatorStrategy_::difference_type;
	using size_type						= typename _AllocatorStrategy_::size_type;


	CONSTEXPR_CXX20 ~MemoryAllocator() noexcept = default;

	constexpr MemoryAllocator() noexcept {};
	CONSTEXPR_CXX20 MemoryAllocator(const MemoryAllocator& other) noexcept = default;

	template <class _Other>
	constexpr MemoryAllocator(const MemoryAllocator<_Other>&) noexcept {};

	CONSTEXPR_CXX20 MemoryAllocator& operator=(const MemoryAllocator&) noexcept = default;

	NODISCARD_RETURN_RAW_PTR inline
	DECLARE_MEMORY_ALLOCATOR CONSTEXPR_CXX20
	value_type* Allocate(size_type bytes) {
		return _AllocatorStrategy_::Allocate(bytes);
	}

	NODISCARD_RETURN_RAW_PTR inline
	DECLARE_MEMORY_ALLOCATOR CONSTEXPR_CXX20
	value_type* AllocateAligned(
		size_type bytes,
		size_type alignment = MEMORY_DEFAULT_ALIGNMENT)
	{
		return _AllocatorStrategy_::AllocateAligned(bytes, alignment);
	}

	NODISCARD_RETURN_RAW_PTR inline
	DECLARE_MEMORY_ALLOCATOR CONSTEXPR_CXX20
	value_type* AllocateZeros(size_type bytes) {
		return _AllocatorStrategy_::AllocateZeros(bytes);
	}

	NODISCARD_RETURN_RAW_PTR inline
	DECLARE_MEMORY_ALLOCATOR CONSTEXPR_CXX20
	value_type* AllocateArray(
		size_type numberOfElements,
		size_type singleElementSize)
	{
		return _AllocatorStrategy_::AllocateArray(
			numberOfElements, singleElementSize);
	}

	NODISCARD_RETURN_RAW_PTR inline
	DECLARE_MEMORY_ALLOCATOR CONSTEXPR_CXX20
	value_type* AllocateArrayAligned(
		size_type numberOfElements,
		size_type singleElementSize,
		size_type alignment = MEMORY_DEFAULT_ALIGNMENT)
	{
		return _AllocatorStrategy_::AllocateArrayAligned(
			numberOfElements, singleElementSize, alignment);
	}

	NODISCARD_RETURN_RAW_PTR inline
	DECLARE_MEMORY_ALLOCATOR CONSTEXPR_CXX20
	value_type* ReAllocate(
		value_type* pointer,
		size_type size)
	{
		return _AllocatorStrategy_::ReAllocate(
			pointer, size);
	}

	NODISCARD_RETURN_RAW_PTR inline
	DECLARE_MEMORY_ALLOCATOR CONSTEXPR_CXX20
	value_type* ReAllocateAligned(
		value_type* pointer,
		size_type size,
		size_type alignment = MEMORY_DEFAULT_ALIGNMENT)
	{
		return _AllocatorStrategy_::ReAllocateAligned(
			pointer, size, MEMORY_DEFAULT_ALIGNMENT);
	}

	NODISCARD_RETURN_RAW_PTR inline
	DECLARE_MEMORY_ALLOCATOR CONSTEXPR_CXX20
	value_type* ReallocateArray(
		value_type* pointer,
		size_type numberOfElements,
		size_type singleElementSize)
	{
		return _AllocatorStrategy_::ReallocateArray(
			pointer, numberOfElements, singleElementSize);
	}

	NODISCARD_RETURN_RAW_PTR inline
	DECLARE_MEMORY_ALLOCATOR CONSTEXPR_CXX20
	value_type* ReallocateFast(
		value_type* pointer,
		uint* size,
		sizetype minimumSize)
	{
		return _AllocatorStrategy_::ReallocateFast(
			pointer, size, minimumSize);
	}

	inline DECLARE_MEMORY_ALLOCATOR CONSTEXPR_CXX20
	void AllocateFast(
		value_type* pointer,
		uint* size, 
		sizetype minimumSize)
	{
		_AllocatorStrategy_::AllocateFast(
			pointer, size, minimumSize);
	}

	inline DECLARE_MEMORY_ALLOCATOR CONSTEXPR_CXX20
	void AllocateZerosFast(
		value_type* pointer,
		uint* size,
		sizetype minimumSize)
	{
		_AllocatorStrategy_::AllocateZerosFast(
			pointer, size, minimumSize);
	}

	inline CONSTEXPR_CXX20
	void Deallocate(value_type* pointer) {
		 _AllocatorStrategy_::Deallocate(pointer);
	}

	inline CONSTEXPR_CXX20
	void Free(value_type* pointer) {
		_AllocatorStrategy_::Free(pointer);
	}

	inline CONSTEXPR_CXX20
	void DeallocateAligned(value_type* pointer) {
		aligned_free(pointer);
	}

	inline CONSTEXPR_CXX20
	void FreeAligned(value_type* pointer) {
		DeallocateAligned(pointer);
	}

	inline CONSTEXPR_CXX20
	void FreeNull(value_type* pointer) {
		_AllocatorStrategy_::FreeNull(pointer);
	}

	inline CONSTEXPR_CXX20
	void FreeNullAligned(value_type* pointer) {
		value_type* value = nullptr;

		memcpy(&value, pointer, sizeof(value));
		memcpy(pointer, &reinterpret_cast<value_type*>(nullptr), sizeof(value));

		FreeAligned(value);
	}

	inline CONSTEXPR_CXX20
	void DeallocateNullAligned(value_type* pointer) {
		FreeNullAligned(pointer);
	}

	NODISCARD_RETURN_RAW_PTR inline
	DECLARE_MEMORY_ALLOCATOR CONSTEXPR_CXX20
	value_type* MemoryDuplicate(
		const value_type* pointer,
		sizetype size)
	{
		return _AllocatorStrategy_::MemoryDuplicate(
			pointer, size);
	}
};

template <typename _Type>
class DefaultMemoryAllocator :
	public MemoryAllocator<DefaultMemoryAllocatorStrategy<_Type>>
{};

template <typename _Type>
class RawMemoryAllocator :
	public MemoryAllocator<RawMemoryAllocatorStrategy<_Type>>
{};

__BASE_MEMORY_NAMESPACE_END