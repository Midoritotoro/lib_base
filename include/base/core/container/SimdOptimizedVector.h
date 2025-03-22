#pragma once 


#include <base/core/memory/MemoryUtility.h>
#include <base/core/container/VectorBase.h>

__BASE_CONTAINER_NAMESPACE_BEGIN

template <
	typename	_Element_,
	class		_Allocator_>
class Vector<
	_Vector_SIMD_Algorithm_Tag_,
	_Element_,
	_Allocator_>:
		public VectorBase<
			_Element_, 
			_Allocator_>
{
public:
	using _MyBase_ = VectorBase<_Element_, _Allocator_>;

	using pointer = typename _MyBase_::pointer;
	using const_pointer = typename _MyBase_::const_pointer;

	using size_type = typename _MyBase_::size_type;
	using difference_type = typename _MyBase_::difference_type;

	using value_type = typename _MyBase_::value_type;
	using allocator_type = typename _MyBase_::_Allocator_;

	using reference = typename _MyBase_::value_type&;
	using const_reference = const typename _MyBase_::value_type&;

	using iterator = typename _MyBase_::iterator;
	using const_iterator = typename _MyBase_::const_iterator;

	using reverse_iterator = typename _MyBase_::reverse_iterator;
	using const_reverse_iterator = typename _MyBase_::const_reverse_iterator;

	using Iterator = iterator;
	using ConstIterator = const_iterator;

	using ValueType = value_type;
	using SizeType = size_type;

	using Reference = reference;
	using ConstReference = const_reference;

	using Pointer = pointer;
	using ConstPointer = const_pointer;

	using ReverseIterator = reverse_iterator;
	using ConstReverseIterator = const_reverse_iterator;


	inline NODISCARD bool resize(const SizeType _Capacity) {
		/*const auto bytesRequired = _Capacity * sizeof(ValueType);
		if (UNLIKELY(bytesRequired <= 0))
			return false;

		_Allocator_ allocator = {};

		const auto memory = allocator.AllocateAligned(bytesRequired, _VectorSIMDAlignment);

		if (UNLIKELY(memory == nullptr))
			return false;

		const auto blockStart = memory;
		const auto blockEnd = memory + _Capacity;

		UNUSED(memory::MemoryCopyCommon(
			begin(), end(),
			blockStart, blockEnd));

		_start = blockStart;
		_end = blockEnd;

		_current = nullptr;*/
		return false;
	}

	inline NODISCARD bool resize(
		size_type size,
		const_reference _Fill)
	{
		/*const auto resizeSuccess = resize(size);
		const auto fillSuccess = fill(_Fill);

		const auto success = (resizeSuccess == true && fillSuccess == true);
		return success;*/
		return false;
	}
};

__BASE_CONTAINER_NAMESPACE_END
