#pragma once

// All warnings defined in this file can be disabled using 
//	#define BASE_SILENCE_ALL_VECTOR_WARNINGS.
// Calling std::terminate in all Debug-Only asserts on failure can be disabled using
//	#define BASE_VECTOR_DEBUG_ASSERT_NO_FAILURE

#include <base/core/memory/MemoryUtility.h>
#include <base/core/memory/MemoryRange.h>

#include <base/core/container/VectorIterator.h>

#include <base/core/container/CompressedPair.h>

#if defined(_DEBUG)

#ifdef BASE_VECTOR_DEBUG_ASSERT_NO_FAILURE
#  define _VECTOR_DEBUG_ASSERT_LOG_(_Cond, _RetVal, _Message)					\
    DebugAssertReturn(_Cond, _Message, _RetVal)
#else
#  define _VECTOR_DEBUG_ASSERT_LOG_(_Cond, _RetVal, _Message)					\
	UNUSED(_RetVal);															\
	DebugAssert(_Cond, _Message)												\
	
#endif


#  ifndef _VECTOR_ERROR_DEBUG_
#    define _VECTOR_ERROR_DEBUG_(_Messsage, _RetVal)							\
	do {																		\
		_VECTOR_DEBUG_ASSERT_LOG_(false, _Messsage);							\
		return _RetVal;															\
	} while(0);
#  endif

#  ifndef _VECTOR_ERROR_DEBUG_NO_RET_
#    define _VECTOR_ERROR_DEBUG_NO_RET_(_Messsage)								\
	do {																		\
		_VECTOR_DEBUG_ASSERT_LOG_(false, _Messsage);							\
		return;																	\
	} while(0);
#  endif

#  ifndef _VECTOR_NOT_ENOUGH_MEMORY_DEBUG_
#    define _VECTOR_NOT_ENOUGH_MEMORY_DEBUG_(_RetVal)							\
    _VECTOR_ERROR_DEBUG_(														\
      "base::container::Vector: Not enough memory to expand the Vector.\n ",	\
        _RetVal)
#  endif

#  ifndef _VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_
#    define _VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_								\
    _VECTOR_ERROR_DEBUG_NO_RET_(												\
      "base::container::Vector: Not enough memory to expand the Vector.\n ")
#  endif

#else

#  ifndef _VECTOR_ERROR_DEBUG_
#    define _VECTOR_ERROR_DEBUG_(_Messsage, _RetVal)							((void)0)
#  endif

#  ifndef _VECTOR_ERROR_DEBUG_NO_RET_
#    define _VECTOR_ERROR_DEBUG_NO_RET_(_Message)								((void)0)
#  endif

#  ifndef _VECTOR_NOT_ENOUGH_MEMORY_DEBUG_
#    define _VECTOR_NOT_ENOUGH_MEMORY_DEBUG_(_RetVal)							((void)0)
#  endif

#  ifndef _VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_
#    define _VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_								((void)0)
#  endif

#endif

#ifndef _VECTOR_BOOLS_PER_UINT32_
#  define _VECTOR_BOOLS_PER_UINT32_ (SIZEOF_TO_BITS(uint32))
#endif


__BASE_CONTAINER_NAMESPACE_BEGIN

enum class _Vector_SIMD_Algorithm_Alignment : sizetype {

};

template <class _Type_>
class VectorValue {
public:
	using value_type		= typename _Type_::value_type;
	using size_type			= typename _Type_::size_type;

	using difference_type	= typename _Type_::difference_type;
	using pointer			= typename _Type_::pointer;

	using const_pointer		= typename _Type_::const_pointer;

	using reference			= value_type&;
	using const_reference	= const value_type&;

	CONSTEXPR_CXX20 VectorValue() noexcept
	{}

	CONSTEXPR_CXX20 VectorValue(
		pointer start, 
		pointer end,
		pointer current) noexcept
	: 
		_start(start),
		_end(end),
		_current(_current) 
	{}

	CONSTEXPR_CXX20 void swap(VectorValue& other) noexcept {
		std::swap(_start, other._start);
		std::swap(_end, other._end);
		std::swap(_current, other._current);
	}

	CONSTEXPR_CXX20 void takeContents(VectorValue& other) noexcept {
		_start = other._start;
		_end = other._end;
		_current = other._current;

		other._start = nullptr;
		other._end = nullptr;
		other._current = nullptr;
	}

	pointer _start		= nullptr;
	pointer _end		= nullptr;

	pointer _current	= nullptr;
};


template <
	typename	_Element_,
	class		_Allocator_ = std::allocator<_Element_>>
class Vector
{
public:
	using value_type = _Element_;
	using allocator_type = _Allocator_;

	using pointer = value_type*;
	using const_pointer = const pointer;

	using size_type = sizetype;
	using difference_type = sizetype;

	using reference = value_type&;
	using const_reference = const value_type&;

	using iterator = VectorIterator<Vector<_Element_, _Allocator_>>;
	using const_iterator = VectorConstIterator<Vector<_Element_, _Allocator_>>;

	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

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
private:
	using VectorValueType = VectorValue<Vector<_Element_, _Allocator_>>;
public:
	inline static constexpr _Vector_SIMD_Algorithm_Alignment _VectorSIMDAlignment =
		_Vector_SIMD_Algorithm_Alignment
#if LIB_BASE_HAS_SIMD_SUPPORT
	{ MINIMUM_ACCEPTABLE_SIMD_ALIGNMENT };
#else
	{ 0 }
#endif
	;

	constexpr Vector() noexcept :
		_pair(_Zero_then_variadic_args_{}, VectorValueType())
	{};

	constexpr ~Vector() noexcept {
		FreeAllElements();
	}

	CONSTEXPR_CXX20 inline Vector(std::initializer_list<ValueType> elements) noexcept :
		_pair(_Zero_then_variadic_args_{}, VectorValueType())
	{
		const auto _Capacity		= capacity();
		const auto _UnusedCapacity	= unusedCapacity();

		const auto elementsSize		= elements.size();

		using _SizeType_			= typename std::initializer_list<ValueType>::size_type;

		if (_UnusedCapacity < elementsSize) {
			const auto isEnoughMemory = resize(_Capacity + elementsSize);

			if (UNLIKELY(isEnoughMemory == false))
				_VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_
		}

		for (_SizeType_ i = 0; i < elementsSize; ++i) {
			auto adress = (elements.begin() + i);
			push_back(*adress);
		}
	}

	CONSTEXPR_CXX20 inline Vector(const Vector& other) noexcept :
		_pair(_Zero_then_variadic_args_{}, VectorValueType())
	{
		const auto _Capacity = other.capacity();
		auto& _Start = other._pair._secondValue._start;

		const auto isEnoughMemory = resize(_Capacity);

		if (UNLIKELY(isEnoughMemory == false)) {
			DebugAssertLog(false, "base::container::Vector: Not enough memory to expand the Vector.\n ");
			return;
		}

		if (memory::MemoryCopy(_pair._secondValue._start, _Start, _Capacity) == false)
			_VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_
	}

	CONSTEXPR_CXX20 inline Vector(const std::vector<ValueType>& vector) noexcept :
		_pair(_Zero_then_variadic_args_{}, VectorValueType())
	{
		const auto _Capacity = vector.capacity();
		auto& _Start = _pair.second()._start;

		const auto isEnoughMemory = resize(_Capacity);

		if (UNLIKELY(isEnoughMemory == false))
			_VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_

		if (UNLIKELY(memory::MemoryCopy(_Start, vector.data(), _Capacity) == false))
			_VECTOR_ERROR_DEBUG_NO_RET_("base::container::Vector::Vector: Error when copying elements. ")
	}

	CONSTEXPR_CXX20 inline Vector(const SizeType _Capacity) noexcept :
		_pair(_Zero_then_variadic_args_{}, VectorValueType())
	{
		const auto isEnoughMemory = resize(_Capacity);

		if (UNLIKELY(isEnoughMemory == false))
			_VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_
	}

	CONSTEXPR_CXX20 inline Vector(
		const SizeType _Capacity,
		const ValueType& _Fill
	) noexcept :
		_pair(_Zero_then_variadic_args_{}, VectorValueType())
	{
		const auto isEnoughMemory = resize(_Capacity);

		if (UNLIKELY(isEnoughMemory == false))
			_VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_

		fill(_Fill);
	}

	CONSTEXPR_CXX20 inline Vector(Vector&& rOther) noexcept :
		_pair(std::exchange(rOther._pair, {}))
	{}

	constexpr inline Reference operator[](const SizeType offset) noexcept {
		const auto pairValue = _pair._secondValue;
		return *(pairValue._current + offset);
	}

	constexpr inline NODISCARD Reference at(const SizeType offset) noexcept {
#ifdef _DEBUG
		const auto pairValue	= _pair._secondValue;
		const auto _Current		= pairValue._current;

		const auto _Start		= pairValue._start;
		const auto _End			= pairValue._end;

		const auto isValidOffset = (_Current + offset > _Start
			&& _Current + offset < _End);

		DebugAssertLog(!isValidOffset, "base::container::VectorBase::operator[]: Index out of range. ");
#endif
		return (*this)[offset];
	}

	constexpr inline NODISCARD ValueType at(const SizeType index) const noexcept {
		return at(index);
	}

	constexpr inline NODISCARD SizeType size() const noexcept {
		return length();
	}

	constexpr inline NODISCARD SizeType length() const noexcept {
		const auto& pairValue = _pair._secondValue;
		return static_cast<SizeType>(pairValue._current - pairValue._start);
	}

	constexpr inline NODISCARD SizeType capacity() const noexcept {
		const auto& pairValue = _pair._secondValue;
		return static_cast<SizeType>(pairValue._end - pairValue._start);
	}

	constexpr inline NODISCARD SizeType unusedCapacity() const noexcept {
		return (capacity() - size());
	}

	constexpr inline NODISCARD bool isEmpty() const noexcept {
		return (length() == 0);
	}

	constexpr inline NODISCARD Pointer data() noexcept {
		auto& pairValue = _pair._secondValue;
		return pairValue._start;
	}

	constexpr inline NODISCARD ConstPointer data() const noexcept {
		const auto& pairValue = _pair._secondValue;
		return pairValue._start;
	}

	constexpr inline NODISCARD ConstPointer constData() const noexcept {
		const auto& pairValue = _pair._secondValue;
		return pairValue._start;
	}

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

	constexpr inline NODISCARD ValueType front() const noexcept {
		return at(0);
	}

	constexpr inline NODISCARD Reference front() noexcept {
		return at(0);
	}

	constexpr inline NODISCARD ValueType back() const noexcept {
		return at(size() - 1);
	}

	constexpr inline NODISCARD Reference back() noexcept {
		return at(size() - 1);
	}

	CONSTEXPR_CXX20 inline void push_back(const ValueType& element) {
		emplaceBack(element);
    }

	CONSTEXPR_CXX20 inline void append(const ValueType& element) {
		emplaceBack(element);
    }

	template <class ... _Args_>
	CONSTEXPR_CXX20 inline void emplace_back(_Args_&&... args) noexcept {
		emplaceBack(std::forward<_Args_>(args)...);
	}
   	
	CONSTEXPR_CXX20 inline NODISCARD ValueType pop() noexcept {
		const auto value = back();
		removeAt(size() - 1);

		return value;
	}

	inline void removeAt(BASE_GUARDOVERFLOW const size_type index) noexcept {

	}

	inline void clear() {
		auto& pairValue		= _pair._secondValue;
		auto& allocator		= _pair.first();

		pointer& _Start		= pairValue._start;
		pointer& _End		= pairValue._end;
		pointer& _Current	= pairValue._current;

		if (_Start == _End)
			return;

		memory::DeallocateRange(_Start, _End, allocator);
		_Current = _Start;
	}

	// Deletes the specified elements from the container
	// Moves the elements of the vector that follow the part being deleted to the place of the elements being deleted
	inline NODISCARD Iterator erase(
		ConstIterator first,
		ConstIterator last)
	{
		auto& pairValue		= _pair._secondValue;
		auto& allocator		= _pair.first();

		if (size() <= 0)
			return;

		memory::DeallocateRange(
			memory::UnFancy(first), 
			memory::UnFancy(last), allocator);

		return Iterator(this);
	}
	
	// Deletes the specified element from the container
	// Moves the elements of the vector that follow the part being deleted to the place of the element being deleted
	inline NODISCARD Iterator erase(ConstIterator it) noexcept(
		std::is_nothrow_move_assignable_v<ValueType>) 
	{
		auto& pairValue = _pair._secondValue;
		auto& allocator = _pair.first();

		pointer& _IteratorPointer = memory::UnFancy(it);

		if (size() <= 0)
			return;

		memory::DeallocateRange(
			pairValue._start,
			memory::UnFancy(it), allocator);

		return Iterator(this);
	}

	// increase capacity to newCapacity (without geometric growth)
	_CONSTEXPR20 void reserve(BASE_GUARDOVERFLOW size_type newCapacity) {
		if (UNLIKELY(newCapacity <= capacity()) || UNLIKELY(newCapacity > maxSize())) // something to do (reserve() never shrinks)
			return;

		const auto isEnoughMemory = resizeReallocate(newCapacity);

		if (UNLIKELY(isEnoughMemory == false))
			_VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_
	}

	inline NODISCARD bool resize(
		SizeType size,
		const_reference _Fill)
	{
		const auto isEnoughMemory = resize(size);

		if (isEnoughMemory)
			fill(_Fill);

		return isEnoughMemory;
	}
	
	inline void fill(const_reference _Fill) {
		for (SizeType i = 0; i < capacity(); ++i)
			insert(i, _Fill);
	}

	inline NODISCARD bool resize(
		BASE_GUARDOVERFLOW const SizeType newCapacity) 
	{
		const auto growth			= calculateGrowth(newCapacity);
		const auto isEnoughMemory	= resizeReallocate(growth);

		return isEnoughMemory;
	}

	inline void insert(
		size_type index,
		const_reference element)
	{
		auto& pairValue		= _pair._secondValue;
		const auto _Offset	= index * sizeof(ValueType);

		emplaceAt(pairValue._start + _Offset, element);
	}

	constexpr inline NODISCARD sizetype max_size() const noexcept {
		return static_cast<std::size_t>(-1) / sizeof(ValueType);
	}

	constexpr inline NODISCARD sizetype maxSize() const noexcept {
		return max_size();
	}
private:
	constexpr inline NODISCARD SizeType calculateGrowth(SizeType newSize) {
		const auto oldCapacity	= capacity();
		const auto _MaxSize		= maxSize();

		if (oldCapacity > _MaxSize - oldCapacity / 2)
			return _MaxSize; // Overflow

		const SizeType geometricGrowth = oldCapacity + oldCapacity / 2;

		if (geometricGrowth < newSize)
			return newSize;

		return geometricGrowth;
	}

	CONSTEXPR_CXX20 inline NODISCARD bool resizeReallocate(SizeType newCapacity) noexcept {
		const auto oldCapacity = capacity();

		if (UNLIKELY(oldCapacity == newCapacity))
			return false;

		const auto bytesRequired = static_cast<SizeType>(newCapacity * sizeof(ValueType));

		if (UNLIKELY(bytesRequired <= 0))
			return false;

		auto& allocator		= _pair.first();
		pointer memory		= allocator.allocate(bytesRequired);

		if (UNLIKELY(memory == nullptr))
			_VECTOR_NOT_ENOUGH_MEMORY_DEBUG_(false)

		const auto oldSize	= size();
		auto& pairValue		= _pair._secondValue;

		pointer& _Start		= pairValue._start;
		pointer& _End		= pairValue._end;
		pointer& _Current	= pairValue._current;

		auto& blockStart	= memory;
		auto blockEnd		= memory + newCapacity;

		if (LIKELY(oldSize != 0) && newCapacity >= oldCapacity)
			memory::MemoryCopyCommon(
				begin(), end(),
				blockStart, blockEnd);

		_Start		= blockStart;
		_End		= blockEnd;
			
		_Current	= blockStart + oldSize;
		
		return true;
	}

	template <class ..._Valty_>
	CONSTEXPR_CXX20 inline void emplaceBack(_Valty_&&... _Val) {
		auto& pairValue		= _pair._secondValue;

		pointer& _End		= pairValue._end;
		pointer& _Current	= pairValue._current;

		if (_Current != _End)
			return emplaceBackWithUnusedCapacity(std::forward<_Valty_>(_Val)...);

		emplaceBackReallocate(std::forward<_Valty_>(_Val)...);
	}

	template <class... _Valty_>
	CONSTEXPR_CXX20 inline void emplaceBackWithUnusedCapacity(_Valty_&&... _Val) {
		auto& pairValue		= _pair._secondValue;
		auto& allocator		= _pair.first();

		emplaceAt(
			allocator, pairValue._current,
			std::forward<_Valty_>(_Val)...);
	}

	template <class... _Valty_>
	CONSTEXPR_CXX20 inline void emplaceBackReallocate(_Valty_&&... _Val) {
		auto& pairValue				= _pair._secondValue;
		auto& allocator				= _pair.first();

		const auto _NewSize			= static_cast<size_type>(sizeof...(_Val) + capacity());
		const auto _IsEnoughMemory	= resize(_NewSize);

		if (UNLIKELY(_IsEnoughMemory == false))
			_VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_

		emplaceAt(
			allocator, pairValue._current,
			std::forward<_Valty_>(_Val)...);
	}

	template <class... _Valty_>
	CONSTEXPR_CXX20 inline void emplaceAt(
		allocator_type& _Allocator,
		pointer&		_Location,
		_Valty_&&...	_Val)
	{
#if defined(OS_WIN) && defined(CPP_MSVC)
		if constexpr (std::conjunction_v<
			std::is_nothrow_constructible<ValueType, _Valty_...>,
			std::_Uses_default_construct<allocator_type, ValueType*, _Valty_...>>
		)
#else
		if constexpr (std::is_nothrow_constructible_v<ValueType, _Valty_...>)
#endif
			memory::ConstructInPlace(
				*_Location, std::forward<_Valty_>(_Val)...);
		else
			std::allocator_traits<allocator_type>::construct(
				_Allocator, memory::UnFancy(_Location),
				std::forward<_Valty_>(_Val)...);

		++_Location;
	}

	inline void FreeAllElements() noexcept {
		auto& pairValue	= _pair._secondValue;
		auto& allocator = _pair.first();

		pointer& _Start = pairValue._start;
		pointer& _End	= pairValue._end;

		memory::FreeRange(_Start, _End, allocator);

		pairValue._current = nullptr;
	}

	CompressedPair<allocator_type, VectorValueType> _pair;
};


// --------------------------------------------------------------------------------------------------------------------------------------------


class VectorBoolConstIterator {

};

class VectorBoolIterator {

};

template <class _Allocator_>
class Vector<bool, _Allocator_>
{
public:
	using value_type = bool;
	using allocator_type = _Allocator_;

	using pointer = value_type*;
	using const_pointer = const pointer;

	using size_type = sizetype;
	using difference_type = sizetype;

	using reference = value_type&;
	using const_reference = const value_type&;

	using iterator = VectorIterator<Vector<bool, _Allocator_>>;
	using const_iterator = VectorConstIterator<Vector<bool, _Allocator_>>;

	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

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
};

__BASE_CONTAINER_NAMESPACE_END
