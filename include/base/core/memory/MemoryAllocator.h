#pragma once 

#include <base/core/memory/Memory.h>

__BASE_MEMORY_NAMESPACE_BEGIN

template <typename _Type_>
class MemoryAllocator {
	static_assert(!std::is_const_v<_Type_>,
		"The C++ Standard forbids containers of const elements "
		"because allocator<const T> is ill-formed.");

	static_assert(!std::is_function_v<_Type_>,
		"The C++ Standard forbids allocators for function elements "
		"because of [allocator.requirements].");

	static_assert(!std::is_reference_v<_Type_>,
		"The C++ Standard forbids allocators for reference elements "
		"because of [allocator.requirements].");
public:
	using pointer			= _Type_*;
	using const_pointer		= const _Type_*;

	using value_type		= _Type_;

	using difference_type	= ptrdiff;
	using size_type			= sizetype;


	CONSTEXPR_CXX20 ~MemoryAllocator() noexcept = default;

	constexpr MemoryAllocator() noexcept {};
	CONSTEXPR_CXX20 MemoryAllocator(const MemoryAllocator& other) noexcept = default;

	template <class _Other>
	constexpr MemoryAllocator(const MemoryAllocator<_Other>&) noexcept {};

	CONSTEXPR_CXX20 MemoryAllocator& operator=(const MemoryAllocator&) noexcept = default;

		static NODISCARD_RETURN_RAW_PTR
	inline DECLARE_MEMORY_ALLOCATOR
	CLANG_CONSTEXPR_CXX20 // Clang и MSVC реализуют P0784R7 по-разному; см. GH-1532
	value_type* allocate(size_type bytes) {
		return reinterpret_cast<value_type*>(::operator new(bytes));
	}
	
#ifdef __cpp_aligned_new
	static NODISCARD_RETURN_RAW_PTR
	inline DECLARE_MEMORY_ALLOCATOR
	CLANG_CONSTEXPR_CXX20
	value_type* allocateAligned(
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
	value_type* allocateAligned(
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
