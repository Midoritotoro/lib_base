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
	public STLCompatibleContainerBase<
		_Element_,
		VectorIterator<
			VectorBase<_Element_, _Allocator_>>,
		VectorConstIterator<
			VectorBase<_Element_, _Allocator_>>>
{
public:
	using _MyVectorBase_ = STLCompatibleContainerBase<
		_Element_,
		VectorIterator<
			VectorBase<_Element_, _Allocator_>>,
		VectorConstIterator<
			VectorBase<_Element_, _Allocator_>>>;
	
	using pointer					= typename _MyVectorBase_::pointer;									
	using const_pointer				= typename _MyVectorBase_::const_pointer;	

	using size_type					= typename _MyVectorBase_::size_type;								
	using difference_type			= typename _MyVectorBase_::difference_type;		

	using value_type				= typename _MyVectorBase_::value_type;								
	using allocator_type			= _Allocator_;			

	using reference					= value_type&;														
	using const_reference			= const value_type&;		

	using iterator					= typename _MyVectorBase_::iterator;									
	using const_iterator			= typename _MyVectorBase_::const_iterator;		

	using reverse_iterator			= typename _MyVectorBase_::reverse_iterator;					
	using const_reverse_iterator	= typename _MyVectorBase_::const_reverse_iterator;	

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
};

template <
	class		_AlgorithmTag_,
	typename	_Element_,
	class		_Allocator_>
class Vector :
	public VectorBase<_Element_, _Allocator_>
{
	
};

__BASE_CONTAINER_NAMESPACE_END