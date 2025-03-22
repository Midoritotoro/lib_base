#pragma once 


#include <base/core/container/STLCompatibleContainerBase.h>
#include <base/core/container/VectorIterator.h>

#include <base/core/memory/MemoryAllocator.h>


__BASE_CONTAINER_NAMESPACE_BEGIN

struct _Vector_Scalar_Algorithm_Tag_ { 
	_Vector_Scalar_Algorithm_Tag_() = default;
};

struct _Vector_SIMD_Algorithm_Tag_ { 
	_Vector_SIMD_Algorithm_Tag_() = default;
};

enum class _Vector_SIMD_Algorithm_Alignment : sizetype { 

};

static constexpr _Vector_SIMD_Algorithm_Alignment _VectorSIMDAlignment =
	_Vector_SIMD_Algorithm_Alignment
#if LIB_BASE_HAS_SIMD_SUPPORT
	{ MINIMUM_ACCEPTABLE_SIMD_ALIGNMENT };
#else
	{ 0 }
#endif
	;

template <
	typename _Element_,
	class _Allocator_ = memory::RawMemoryAllocator<_Element_>>
class VectorBase : 
	public STLCompatibleContainerBase<_Element_>
{
public:
	CONSTEXPR_CXX20 VectorBase() noexcept {};
	CONSTEXPR_CXX20 ~VectorBase() noexcept {};

	using value_type				= _Element_;
	using allocator_type			= _Allocator_;

	using pointer					= value_type*;
	using const_pointer				= const pointer;

	using _MyVectorBase_			= STLCompatibleContainerBase<_Element_>;

	using size_type					= typename _MyVectorBase_::size_type;
	using difference_type			= typename _MyVectorBase_::difference_type;

	using reference					= value_type&;														
	using const_reference			= const value_type&;		

	using iterator					= VectorIterator<VectorBase<_Element_, _Allocator_>>;
	using const_iterator			= VectorConstIterator<VectorBase<_Element_, _Allocator_>>;

	using reverse_iterator			= std::reverse_iterator<iterator>;					
	using const_reverse_iterator	= std::reverse_iterator<const_iterator>;

	using Iterator					= iterator;															
	using ConstIterator				= const_iterator;	

	using ValueType					= value_type;														
	using SizeType					= size_type;	

	using Reference					= reference;														
	using ConstReference			= const_reference;			

	using Pointer					= pointer;															
	using ConstPointer				= const_pointer;

	using ReverseIterator			= reverse_iterator;											
	using ConstReverseIterator		= const_reverse_iterator;

	static constexpr sizetype _Buffer_For_Resizing_Length = 16; // ... 

	constexpr inline NODISCARD Iterator begin() noexcept {
		return Iterator(this);
	}

	constexpr inline NODISCARD ConstIterator begin() const noexcept {
		return ConstIterator(this);
	}

	constexpr inline NODISCARD ConstIterator cbegin() const noexcept {
		return ConstIterator(this);
	}

	constexpr inline NODISCARD ConstIterator constBegin() const noexcept {
		return ConstIterator(this);
	}

	constexpr inline NODISCARD Iterator end() noexcept {
		return Iterator(this) + size();
	}

	constexpr inline NODISCARD ConstIterator end() const noexcept {
		return ConstIterator(this) + size();
	}

	constexpr inline NODISCARD ConstIterator cend() const {
		return ConstIterator(this) + size();
	}

	constexpr inline NODISCARD ConstIterator constEnd() const noexcept {
		return ConstIterator(this) + size();
	}

	constexpr inline NODISCARD ReverseIterator rbegin() noexcept {
		return ReverseIterator(begin());
	}

	constexpr inline NODISCARD ReverseIterator rend() noexcept {
		return ReverseIterator(end());
	}

	constexpr inline NODISCARD ConstReverseIterator rbegin() const noexcept {
		return ConstReverseIterator(begin());
	}

	constexpr inline NODISCARD ConstReverseIterator rend() const noexcept {
		return ConstReverseIterator(end());
	}

	constexpr inline NODISCARD ConstReverseIterator crbegin() const noexcept {
		return ConstReverseIterator(begin());
	}

	constexpr inline NODISCARD ConstReverseIterator crend() const noexcept {
		return ConstReverseIterator(end());
	}
};

template <
	class		_AlgorithmTag_,
	typename	_Element_,
	class		_Allocator_ = memory::RawMemoryAllocator<_Element_>>
class Vector :
	public VectorBase<
		_Element_,
		_Allocator_>
{};

__BASE_CONTAINER_NAMESPACE_END