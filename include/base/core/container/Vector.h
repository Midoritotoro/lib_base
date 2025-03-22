#pragma once


#include <base/core/memory/MemoryUtility.h>
#include <base/core/container/VectorBase.h>

__BASE_CONTAINER_NAMESPACE_BEGIN

template <
	typename	_Element_,
	class		_Allocator_>
class Vector<
	_Vector_Scalar_Algorithm_Tag_, 
	_Element_,
	_Allocator_>:
		public VectorBase<
			_Element_,
			_Allocator_>
	{
public:
	using _MyBase_	= VectorBase<_Element_, _Allocator_>;

	using pointer					= typename _MyBase_::pointer;
	using const_pointer				= typename _MyBase_::const_pointer;

	using size_type					= typename _MyBase_::size_type;
	using difference_type			= typename _MyBase_::difference_type;

	using value_type				= typename _MyBase_::value_type;
	using allocator_type			= typename _MyBase_::allocator_type;

	using reference					= typename _MyBase_::value_type&;
	using const_reference			= const typename _MyBase_::value_type&;

	using iterator					= typename _MyBase_::iterator;
	using const_iterator			= typename _MyBase_::const_iterator;

	using reverse_iterator			= typename _MyBase_::reverse_iterator;
	using const_reverse_iterator	= typename _MyBase_::const_reverse_iterator;

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

    constexpr Vector() noexcept {};
    constexpr ~Vector() noexcept = default;

    CONSTEXPR_CXX20 inline Vector(std::initializer_list<ValueType> elements) noexcept {
		const auto _Capacity		= capacity();
		const auto _UnusedCapacity	= unusedCapacity();

		const auto elementsSize		= elements.size();

		if (_UnusedCapacity < elementsSize)
			TryResize(_Capacity + elementsSize);

		for (SizeType i = 0; i < elementsSize; ++i) {
			const auto adress = (_start + i);
			*adress = (elements[i]);
		}
    }

	CONSTEXPR_CXX20 inline Vector(const Vector& other) {
		const auto _Capacity = other.capacity();
		TryResize(_Capacity);

		if (memory::MemoryCopy(_start, other._start, _Capacity) == false)
			AssertReturn(false, "base::container::Vector::Vector: Ошибка при копировании элементов. ");
	}

	CONSTEXPR_CXX20 inline Vector(const std::vector<ValueType>& vector) {
		const auto _Capacity = vector.capacity();
		TryResize(_Capacity);

		if (memory::MemoryCopy(_start, vector.data(), _Capacity) == false)
			AssertReturn(false, "base::container::Vector::Vector: Ошибка при копировании элементов. ");
    }

	CONSTEXPR_CXX20 inline Vector(const SizeType capacity) {
		TryResize(capacity);
    }

	CONSTEXPR_CXX20 inline Vector(
        const SizeType _Capacity,
        const ValueType& _Fill)
    {
		TryResize(_Capacity);
		fill(_Fill);
    }
    
	CONSTEXPR_CXX20 inline Vector(Vector&& rOther) noexcept {

    }

	CONSTEXPR_CXX20 inline void push_back(const ValueType& element) {

		
		++_current;

	//	memory::MemoryMove(_current, )
    }

	CONSTEXPR_CXX20 inline void push_front(const ValueType& element) {

    }

	CONSTEXPR_CXX20 inline void append(const ValueType& element) {

    }
   	
	CONSTEXPR_CXX20 inline NODISCARD ValueType pop() noexcept {
		const auto value = back();
		remveAt(size() - 1);

		return value;
	}

	inline NODISCARD SizeType find(ConstReference element) const noexcept {
		return 0;
	}

	inline NODISCARD Vector<size_type> 
		findAll(ConstReference element) const noexcept
	{
		return {}
	}
	
	inline NODISCARD size_type
		findLastOf(ConstReference element) const noexcept
	{
		
	}

	inline NODISCARD size_type 
		findFirstOf(const_reference element) const noexcept
	{
		;
	}

	inline NODISCARD size_type
		findLastNotOf(const_reference element) const noexcept
	{
		return 0;
	}
	
	inline NODISCARD size_type 
		findFirstNotOf(const_reference element) const noexcept
	{
		return 0;
	}

	inline void clear() {
		UNUSED(erase(constBegin(), constEnd()));
	}

	inline NODISCARD Iterator erase(
		ConstIterator first,
		ConstIterator last)
	{
		return {};
	}

	inline NODISCARD Iterator erase(ConstIterator it) {
		return {};
	}

	inline NODISCARD bool resize(
		size_type size,
		const_reference _Fill)
	{
		const auto resizeSuccess = TryResize(size);
		const auto fillSuccess   = fill(_Fill);

		const auto success		 = (resizeSuccess == true && fillSuccess == true);
		return success;
	}

    inline NODISCARD size_type indexOf(
		const_reference element,
		size_type from = 0) const noexcept
	{
		for (size_type i = from; i < size(); ++i) {
			const auto adress = (_start + i);

			if ((*adress) == element)
				return i;
		}

		return -1;
	}

	inline NODISCARD size_type lastIndexOf(
		const_reference element,
		size_type from = 0) const noexcept
	{
		
	}

	inline NODISCARD bool contains(
		const_reference element,
		size_type from = 0) const noexcept
	{
		for (size_type i = from; i < size(); ++i) {
			const auto adress = (_start + i);

			if ((*adress) == element)
				return true;
		}

		return false;
	}

	inline NODISCARD bool contains(
		const Vector<value_type> subVector,
		size_type from = 0) const noexcept
	{
		size_type overlaps = 0;
		const auto subVectorSize = subVector.size();

		for (size_type i = 0; i < size(); ++i) {
			const auto adress = (_start + i);

			if ((*adress) == subVector[i % subVectorSize] && (++overlaps == subVectorSize))
				return true;
		}

		return false;
	}
	
	inline NODISCARD size_type count(
		const_reference element) const noexcept
	{
		size_type _Count = 0;

		for (size_type i = 0; i < size(); ++i) {
			const auto adress = (_start + i);
			_Count += ((*adress) == element);
		}
	}
	
	inline NODISCARD size_type count(
		const Vector<value_type>& subVector) const noexcept
	{
		size_type _Count = 0;
		size_type overlaps = 0;

		const auto subVectorSize = subVector.size();

		for (size_type i = 0; i < size(); ++i) {
			const auto adress = (_start + i);

			((*adress) == subVector[i % subVectorSize] 
				&& (++overlaps == subVectorSize))
					? ++_Count;
					: overlaps = 0;
		}
		
		return _Count;
	}
	
	inline void fill(const_reference _Fill) noexcept {
		for (size_type i = 0; i < size(); ++i) {
			const auto adress = (_start + i);
			*adress = _Fill;
		}
	}

	inline NODISCARD bool resize(const SizeType _Capacity) {
		const auto bytesRequired = _Capacity * sizeof(ValueType);
		if (UNLIKELY(bytesRequired <= 0))
			return false;

		_Allocator_ allocator = {};

		const auto memory = allocator.Allocate(bytesRequired);

		if (UNLIKELY(memory == nullptr))
			return false;

		const auto blockStart	= memory;
		const auto blockEnd		= memory + _Capacity;

		UNUSED(memory::MemoryCopyCommon(
			begin(), end(),
			blockStart, blockEnd));

		_start		= blockStart;
		_end		= blockEnd;
			
		_current	= nullptr;
	}

	inline void insert(
		size_type index,
		const_reference element) noexcept
	{

	}

	inline void prepend(const_reference element) noexcept {

	}
private:
	inline void TryResize(sizetype newCapacity) {
		const auto isEnoughMemory = resize(newCapacity);

		if (UNLIKELY(isEnoughMemory == false))
			AssertReturn(false, "base::container::Vector: Not enough memory to expand the Vector.\n ", UNUSED(0));
	}

	inline void InsertToAdress(
		sizetype offset, 
		ValueType* value)
	{
		const auto adressForOffset = (_start + offset);
		
	}
};

__BASE_CONTAINER_NAMESPACE_END

#include <base/core/container/SimdOptimizedVector.h>