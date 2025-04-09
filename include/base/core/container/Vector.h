#pragma once

// All warnings defined in this file can be disabled using 
//	#define BASE_SILENCE_ALL_VECTOR_WARNINGS.
// Calling std::terminate in all Debug-Only asserts on failure can be disabled using
//	#define BASE_VECTOR_DEBUG_ASSERT_NO_FAILURE

#include <base/core/memory/MemoryUtility.h>
#include <base/core/memory/MemoryRange.h>

#include <base/core/container/VectorIterator.h>

#include <base/core/container/CompressedPair.h>
#include <base/core/memory/MemoryAllocatorUtility.h>

WARNING_DISABLE_MSVC(4834)
WARNING_DISABLE_MSVC(4002)
WARNING_DISABLE_MSVC(4003)
WARNING_DISABLE_MSVC(1)

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

#ifndef _VECTOR_OUTSIDE_TEMPLATE_ 
#  define _VECTOR_OUTSIDE_TEMPLATE_ template <typename _Element_, class _Allocator_>
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

	CONSTEXPR_CXX20 inline VectorValue() noexcept
	{}

	CONSTEXPR_CXX20 inline VectorValue(
		pointer start, 
		pointer end,
		pointer current) noexcept
	: 
		_start(start),
		_end(end),
		_current(_current) 
	{}

	CONSTEXPR_CXX20 inline void swap(VectorValue& other) noexcept {
		std::swap(_start, other._start);
		std::swap(_end, other._end);
		std::swap(_current, other._current);
	}

	CONSTEXPR_CXX20 inline void takeContents(VectorValue& other) noexcept {
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

	inline constexpr Vector() noexcept;
	inline constexpr ~Vector() noexcept;

	CONSTEXPR_CXX20 inline Vector(std::initializer_list<ValueType> elements) noexcept;
	CONSTEXPR_CXX20 inline Vector(const Vector& other) noexcept;

	CONSTEXPR_CXX20 inline Vector(const std::vector<ValueType>& vector) noexcept;
	CONSTEXPR_CXX20 inline Vector(const SizeType _Capacity) noexcept;

	template <class _InputIterator_>
	CONSTEXPR_CXX20 inline Vector(
		_InputIterator_ _First, 
		_InputIterator_ _Last) noexcept;

	CONSTEXPR_CXX20 inline Vector(
		const SizeType _Capacity,
		const ValueType& _Fill) noexcept;
	CONSTEXPR_CXX20 inline Vector(Vector&& rOther) noexcept;

	CONSTEXPR_CXX20 inline NODISCARD allocator_type getAllocator() const noexcept;

	CONSTEXPR_CXX20 inline Vector& operator=(Vector&& _Right) noexcept(
		memory::ChoosePocma_v<allocator_type> != memory::PocmaValues::NoPropagateAllocators);

	CONSTEXPR_CXX20 inline Vector& operator=(const Vector& _Right);

	CONSTEXPR_CXX20 inline Vector& operator=(const std::vector<ValueType, allocator_type>& _Right);
	CONSTEXPR_CXX20 inline Vector& operator=(std::vector<ValueType, allocator_type>&& rVector);

	CONSTEXPR_CXX20 inline Vector& operator=(std::initializer_list<ValueType> initializerList);

	CONSTEXPR_CXX20 inline Reference operator[](const SizeType offset) noexcept;
	CONSTEXPR_CXX20 inline ConstReference operator[](const SizeType offset) const noexcept;

	template <typename U = T>
	QTypeTraits::compare_eq_result_container<QList, U> operator==(const QList& other) const
	{
		if (size() != other.size())
			return false;
		if (begin() == other.begin())
			return true;

		// do element-by-element comparison
		return d->compare(data(), other.data(), size());
	}
	template <typename U = T>
	QTypeTraits::compare_eq_result_container<QList, U> operator!=(const QList& other) const
	{
		return !(*this == other);
	}

	template <typename U = T>
	QTypeTraits::compare_lt_result_container<QList, U> operator<(const QList& other) const
		noexcept(noexcept(std::lexicographical_compare<typename QList<U>::const_iterator,
			typename QList::const_iterator>(
				std::declval<QList<U>>().begin(), std::declval<QList<U>>().end(),
				other.begin(), other.end())))
	{
		return std::lexicographical_compare(begin(), end(),
			other.begin(), other.end());
	}

	template <typename U = T>
	QTypeTraits::compare_lt_result_container<QList, U> operator>(const QList& other) const
		noexcept(noexcept(other < std::declval<QList<U>>()))
	{
		return other < *this;
	}

	template <typename U = T>
	QTypeTraits::compare_lt_result_container<QList, U> operator<=(const QList& other) const
		noexcept(noexcept(other < std::declval<QList<U>>()))
	{
		return !(other < *this);
	}

	template <typename U = T>
	QTypeTraits::compare_lt_result_container<QList, U> operator>=(const QList& other) const
		noexcept(noexcept(std::declval<QList<U>>() < other))
	{
		return !(*this < other);
	}

	CONSTEXPR_CXX20 inline NODISCARD Reference at(const SizeType offset) noexcept;
	CONSTEXPR_CXX20 inline NODISCARD ValueType at(const SizeType index) const noexcept;

	constexpr inline NODISCARD SizeType count() const noexcept;
	constexpr inline NODISCARD SizeType size() const noexcept;
	constexpr inline NODISCARD SizeType length() const noexcept;

	constexpr inline NODISCARD SizeType capacity() const noexcept;
	constexpr inline NODISCARD SizeType unusedCapacity() const noexcept;

	constexpr inline NODISCARD bool isEmpty() const noexcept;

	constexpr inline NODISCARD Pointer data() noexcept;
	constexpr inline NODISCARD ConstPointer data() const noexcept;

	constexpr inline NODISCARD ConstPointer constData() const noexcept;

	constexpr inline NODISCARD Iterator begin() noexcept;
	constexpr inline NODISCARD ConstIterator begin() const noexcept;

	constexpr inline NODISCARD ConstIterator cbegin() const noexcept;
	constexpr inline NODISCARD ConstIterator constBegin() const noexcept;

	constexpr inline NODISCARD Iterator end() noexcept;
	constexpr inline NODISCARD ConstIterator end() const noexcept;

	constexpr inline NODISCARD ConstIterator cend() const;
	constexpr inline NODISCARD ConstIterator constEnd() const noexcept;

	constexpr inline NODISCARD ReverseIterator rbegin() noexcept;
	constexpr inline NODISCARD ReverseIterator rend() noexcept;

	constexpr inline NODISCARD ConstReverseIterator rbegin() const noexcept;
	constexpr inline NODISCARD ConstReverseIterator rend() const noexcept;

	constexpr inline NODISCARD ConstReverseIterator crbegin() const noexcept;
	constexpr inline NODISCARD ConstReverseIterator crend() const noexcept;

	constexpr inline NODISCARD ValueType front() const noexcept;
	constexpr inline NODISCARD Reference front() noexcept;

	constexpr inline NODISCARD ValueType back() const noexcept;
	constexpr inline NODISCARD Reference back() noexcept;
	
	CONSTEXPR_CXX20 inline void push_back(const ValueType& element);
	CONSTEXPR_CXX20 inline void push_back(Vector&& other);
	CONSTEXPR_CXX20 inline void push_back(ValueType&& element);

	CONSTEXPR_CXX20 inline void append(const ValueType& element);
	CONSTEXPR_CXX20 inline void append(Vector&& other);
	CONSTEXPR_CXX20 inline void append(ValueType&& element);

	CONSTEXPR_CXX20 inline void prepend(const ValueType& element);
	CONSTEXPR_CXX20 inline void prepend(Vector&& other);
	CONSTEXPR_CXX20 inline void prepend(ValueType&& element);

	CONSTEXPR_CXX20 inline void push_front(const ValueType& element);
	CONSTEXPR_CXX20 inline void push_front(Vector&& other);
	CONSTEXPR_CXX20 inline void push_front(ValueType&& element);

	template <class ... _Args_>
	CONSTEXPR_CXX20 inline NODISCARD Reference emplace_back(_Args_&&... args);
	
	template <class ... _Args_>
	CONSTEXPR_CXX20 inline NODISCARD Reference emplaceBack(_Args_&&... args);

	template <class... _Valty_>
	CONSTEXPR_CXX20 inline NODISCARD Iterator emplace(
		ConstIterator where, 
		_Valty_&&... value);

	template <class... _Valty_>
	CONSTEXPR_CXX20 inline NODISCARD Iterator emplace(
		SizeType where,
		_Valty_&&... value);

	CONSTEXPR_CXX20 inline NODISCARD Iterator insert(
		SizeType index,
		const_reference element);

	CONSTEXPR_CXX20 inline NODISCARD Iterator insert(
		ConstIterator where, 
		const ValueType& value);

	CONSTEXPR_CXX20 inline NODISCARD Iterator insert(
		ConstIterator where,
		ValueType&& value);

	CONSTEXPR_CXX20 inline NODISCARD Iterator insert(
		ConstIterator where, 
		BASE_GUARDOVERFLOW const SizeType count, 
		const ValueType& value);

	CONSTEXPR_CXX20 inline Iterator insert(
		ConstIterator where,
		std::initializer_list<ValueType> initializerList);

	CONSTEXPR_CXX20 inline void assign(
		BASE_GUARDOVERFLOW const SizeType newSize,
		const ValueType& value);

	template <class _InputIterator_>
	CONSTEXPR_CXX20 inline void assign(
		_InputIterator_ _First,
		_InputIterator_ _Last);

	CONSTEXPR_CXX20 inline void assign(
		const std::initializer_list<ValueType> initializerList);

	CONSTEXPR_CXX20 inline NODISCARD ValueType pop() noexcept;

	CONSTEXPR_CXX20 inline NODISCARD ValueType popBack() noexcept;
	CONSTEXPR_CXX20 inline NODISCARD ValueType pop_back() noexcept;

	CONSTEXPR_CXX20 inline NODISCARD ValueType pop_front() noexcept;
	CONSTEXPR_CXX20 inline NODISCARD ValueType popFront() noexcept;

	CONSTEXPR_CXX20 inline void removeAt(BASE_GUARDOVERFLOW const SizeType index) noexcept;

	CONSTEXPR_CXX20 inline void clear();

	// Deletes the specified elements from the container
	// Moves the elements of the vector that follow the part being deleted to the place of the elements being deleted
	CONSTEXPR_CXX20 inline NODISCARD Iterator erase(
		ConstIterator first,
		ConstIterator last) noexcept(
			std::is_nothrow_move_assignable_v<ValueType>);

	// Deletes the specified element from the container
	// Moves the elements of the vector that follow the part being deleted to the place of the element being deleted
	CONSTEXPR_CXX20 inline NODISCARD Iterator erase(ConstIterator it) noexcept(
		std::is_nothrow_move_assignable_v<ValueType>);

	// increase capacity to newCapacity (without geometric growth)
	CONSTEXPR_CXX20 inline void reserve(BASE_GUARDOVERFLOW SizeType newCapacity);

	CONSTEXPR_CXX20 inline void shrinkToFit();
	CONSTEXPR_CXX20 inline void shrink_to_fit();

	CONSTEXPR_CXX20	inline NODISCARD  bool resize(
		BASE_GUARDOVERFLOW const SizeType size,
		const_reference _Fill);
	inline NODISCARD bool resize(
		BASE_GUARDOVERFLOW const SizeType newCapacity);

	CONSTEXPR_CXX20 inline void fill(const_reference _Fill);

	CONSTEXPR_CXX20 inline NODISCARD size_type count(const ValueType& element) const noexcept;
	CONSTEXPR_CXX20 inline NODISCARD size_type count(const Vector& subVector) const noexcept;

	CONSTEXPR_CXX20 inline NODISCARD bool contains(const ValueType& element) const noexcept;
	CONSTEXPR_CXX20 inline NODISCARD bool contains(const Vector& subVector) const noexcept;

	CONSTEXPR_CXX20 inline NODISCARD bool startsWith(const ValueType& element) const noexcept;
	CONSTEXPR_CXX20 inline NODISCARD bool startsWith(const Vector& subVector) const noexcept;

	CONSTEXPR_CXX20 inline NODISCARD bool endsWith(const ValueType& element) const noexcept;
	CONSTEXPR_CXX20 inline NODISCARD bool endsWith(const Vector& subVector) const noexcept;

	CONSTEXPR_CXX20 inline NODISCARD SizeType indexOf(const ValueType& element) const noexcept;
	CONSTEXPR_CXX20 inline NODISCARD SizeType lastIndexOf(const ValueType& element) const noexcept;

	CONSTEXPR_CXX20 inline NODISCARD Vector	sliced(
		SizeType positionFrom,
		SizeType elementsCount) const noexcept;
	CONSTEXPR_CXX20 inline NODISCARD Vector	sliced(SizeType positionFrom) const noexcept;

	CONSTEXPR_CXX20 inline void replace(
		SizeType position,
		const ValueType& value);

	CONSTEXPR_CXX20 inline void replace(
		SizeType position,
		ValueType&& value);

	CONSTEXPR_CXX20 inline void swap(Vector& other);

	CONSTEXPR_CXX20 inline void swapElementsAt(
		SizeType _First,
		SizeType _Last);
	CONSTEXPR_CXX20 inline void swapElementsAt(
		ConstIterator _First,
		ConstIterator _Last);

	CONSTEXPR_CXX20 inline void remove(
		SizeType _First,
		SizeType _Count);
	CONSTEXPR_CXX20 inline void remove(
		ConstIterator _First,
		ConstIterator _Last);

	CONSTEXPR_CXX20 inline NODISCARD SizeType removeAll(const ValueType& element);

	CONSTEXPR_CXX20 inline void removeFirst();
	CONSTEXPR_CXX20 inline void removeLast();

	template <
		typename _Predicate_,
		typename = std::enable_if_t<std::is_function_v<_Predicate_>>> // ?
	CONSTEXPR_CXX20 inline NODISCARD SizeType removeIf(_Predicate_ pred);

	CONSTEXPR_CXX20 inline NODISCARD bool removeOne(const ValueType& element);

	constexpr inline NODISCARD sizetype max_size() const noexcept;
	constexpr inline NODISCARD sizetype maxSize() const noexcept;
private:
	constexpr inline NODISCARD SizeType calculateGrowth(SizeType newSize) const noexcept;
	CONSTEXPR_CXX20 inline NODISCARD bool resizeReallocate(SizeType newCapacity) noexcept;

	template <class ..._Valty_>
	CONSTEXPR_CXX20 inline NODISCARD Reference emplaceBack(_Valty_&&... _Val);

	template <class... _Valty_>
	CONSTEXPR_CXX20 inline NODISCARD Reference emplaceBackWithUnusedCapacity(_Valty_&&... _Val);

	template <class... _Valty_>
	CONSTEXPR_CXX20 inline NODISCARD Reference emplaceBackReallocate(_Valty_&&... _Val);

	template <class... _Valty_>
	CONSTEXPR_CXX20 inline NODISCARD Reference emplaceAt(
		allocator_type& _Allocator,
		pointer& _Location,
		_Valty_&&...	_Val);

	CONSTEXPR_CXX20 inline void FreeAllElements() noexcept;

	CompressedPair<allocator_type, VectorValueType> _pair;
};

_VECTOR_OUTSIDE_TEMPLATE_
inline constexpr Vector<_Element_, _Allocator_>::Vector() noexcept :
	_pair(_Zero_then_variadic_args_{}, VectorValueType())
{};

_VECTOR_OUTSIDE_TEMPLATE_
inline constexpr Vector<_Element_, _Allocator_>::~Vector() noexcept {
	FreeAllElements();
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline Vector<_Element_, _Allocator_>
	::Vector(std::initializer_list<ValueType> elements) noexcept:
		_pair(_Zero_then_variadic_args_{}, VectorValueType())
{
	const auto _Capacity = capacity();
	const auto _UnusedCapacity = unusedCapacity();

	const auto elementsSize = elements.size();

	using _SizeType_ = typename std::initializer_list<ValueType>::size_type;

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

_VECTOR_OUTSIDE_TEMPLATE_
template <class _InputIterator_>
CONSTEXPR_CXX20 inline Vector<_Element_, _Allocator_>::Vector(
	_InputIterator_ _First,
	_InputIterator_ _Last) noexcept
{

}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline Vector<_Element_, _Allocator_>::Vector(const Vector& other) noexcept :
	_pair(_Zero_then_variadic_args_{}, VectorValueType())
{
	const auto _Capacity = other.capacity();
	auto& _Start = other._pair._secondValue._start;

	const auto isEnoughMemory = resize(_Capacity);

	if (UNLIKELY(isEnoughMemory == false))
		_VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_

	if (UNLIKELY(memory::MemoryCopy(_pair._secondValue._start, _Start, _Capacity) == false))
		_VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline Vector<_Element_, _Allocator_>::
	Vector(const std::vector<ValueType>& vector) noexcept :
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

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline Vector<_Element_, _Allocator_>
	::Vector(const SizeType _Capacity) noexcept :
		_pair(_Zero_then_variadic_args_{}, VectorValueType())
{
	const auto isEnoughMemory = resize(_Capacity);

	if (UNLIKELY(isEnoughMemory == false))
		_VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline Vector<_Element_, _Allocator_>::Vector(
		const SizeType _Capacity,
		const ValueType& _Fill) noexcept :
	_pair(_Zero_then_variadic_args_{}, VectorValueType())
{
	const auto isEnoughMemory = resize(_Capacity);

	if (UNLIKELY(isEnoughMemory == false))
		_VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_

	fill(_Fill);
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline Vector<_Element_, _Allocator_>
	::Vector(Vector&& rOther) noexcept :
		_pair(std::exchange(rOther._pair, {}))
{}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>::allocator_type 
	Vector<_Element_, _Allocator_>::getAllocator() const noexcept
{
	return _pair.first();
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline Vector<_Element_, _Allocator_>& 
	Vector<_Element_, _Allocator_>::operator=(const Vector& _Right) 
{
	return *this;
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline Vector<_Element_, _Allocator_>& 
	Vector<_Element_, _Allocator_>::operator=(
		std::vector<ValueType, allocator_type>&& rVector)
{
	return (*this;)
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 Vector<_Element_, _Allocator_>&
		Vector<_Element_, _Allocator_>::operator=(Vector&& _Right) noexcept(
			memory::ChoosePocma_v<allocator_type> 
			!= memory::PocmaValues::NoPropagateAllocators)
{
	if (this == memory::AddressOf(_Right))
		return *this;

	auto& pairValue = _pair._secondValue;
	auto& _Al = _pair.first();

	auto& _Right_al = _Right._pair.first();

	constexpr auto _Pocma_val = memory::ChoosePocma_v<allocator_type>;
	if constexpr (_Pocma_val == memory::PocmaValues::NoPropagateAllocators) {
		if (_Al != _Right_al) {
			//_Move_assign_unequal_alloc(_Right);
			return *this;
		}
	}

	FreeAllElements();

	memory::POCMA(_Al, _Right_al);
	pairValue.takeContents(_Right.pairValue._secondValue);

	return *this;
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline Vector<_Element_, _Allocator_>& 
	Vector<_Element_, _Allocator_>::operator=(
		const std::vector<ValueType, allocator_type>& _Right)
{
	return *this;
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline Vector<_Element_, _Allocator_>& 
	Vector<_Element_, _Allocator_>::operator=(
		std::initializer_list<ValueType> initializerList) 
{
	return *this;
}

_VECTOR_OUTSIDE_TEMPLATE_
constexpr inline Vector<_Element_, _Allocator_>::Reference
	Vector<_Element_, _Allocator_>::
		operator[](const SizeType offset) noexcept 
{
	auto& pairValue = _pair._secondValue;
	return *(pairValue._current + offset);
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline Vector<_Element_, _Allocator_>::ConstReference 
	Vector<_Element_, _Allocator_>::
		operator[](const SizeType offset) const noexcept
{	
	const auto& pairValue = _pair._secondValue;
	return *(pairValue._current + offset);
}

_VECTOR_OUTSIDE_TEMPLATE_
constexpr inline NODISCARD Vector<_Element_, _Allocator_>::Reference 
	Vector<_Element_, _Allocator_>::at(const SizeType offset) noexcept 
{
#ifdef _DEBUG
	const auto pairValue	= _pair._secondValue;
	const auto _Current		= pairValue._current;

	const auto _Start		= pairValue._start;
	const auto _End			= pairValue._end;

	const auto isValidOffset = (_Current + offset > _Start
		&& _Current + offset < _End);

	_VECTOR_DEBUG_ASSERT_LOG_(!isValidOffset, "base::container::VectorBase::operator[]: Index out of range. ", {}); // ? 
#endif
	return (*this)[offset];
}

_VECTOR_OUTSIDE_TEMPLATE_
constexpr inline NODISCARD Vector<_Element_, _Allocator_>::ValueType
	Vector<_Element_, _Allocator_>::at(const SizeType index) const noexcept
{
	return at(index);
}

_VECTOR_OUTSIDE_TEMPLATE_
constexpr inline NODISCARD Vector<_Element_, _Allocator_>::SizeType 
	Vector<_Element_, _Allocator_>::size() const noexcept 
{
	const auto& pairValue = _pair._secondValue;
	return static_cast<SizeType>(pairValue._current - pairValue._start);
}

_VECTOR_OUTSIDE_TEMPLATE_
constexpr inline NODISCARD Vector<_Element_, _Allocator_>::SizeType 
	Vector<_Element_, _Allocator_>::count() const noexcept
{
	const auto& pairValue = _pair._secondValue;
	return static_cast<SizeType>(pairValue._current - pairValue._start);
}

_VECTOR_OUTSIDE_TEMPLATE_
constexpr inline NODISCARD Vector<_Element_, _Allocator_>::SizeType
	Vector<_Element_, _Allocator_>::length() const noexcept 
{
	const auto& pairValue = _pair._secondValue;
	return static_cast<SizeType>(pairValue._current - pairValue._start);
}

_VECTOR_OUTSIDE_TEMPLATE_
constexpr inline NODISCARD Vector<_Element_, _Allocator_>::SizeType 
	Vector<_Element_, _Allocator_>::capacity() const noexcept
{
	const auto& pairValue = _pair._secondValue;
	return static_cast<SizeType>(pairValue._end - pairValue._start);
}

_VECTOR_OUTSIDE_TEMPLATE_
constexpr inline NODISCARD Vector<_Element_, _Allocator_>::SizeType
	Vector<_Element_, _Allocator_>::unusedCapacity() const noexcept
{
	return (capacity() - size());
}

_VECTOR_OUTSIDE_TEMPLATE_
constexpr inline NODISCARD bool Vector<_Element_, _Allocator_>::isEmpty() const noexcept {
	return (length() == 0);
}

_VECTOR_OUTSIDE_TEMPLATE_
constexpr inline NODISCARD Vector<_Element_, _Allocator_>::Pointer 
	Vector<_Element_, _Allocator_>::data() noexcept 
{
	auto& pairValue = _pair._secondValue;
	return pairValue._start;
}

_VECTOR_OUTSIDE_TEMPLATE_
constexpr inline NODISCARD Vector<_Element_, _Allocator_>::ConstPointer
	Vector<_Element_, _Allocator_>::data() const noexcept 
{
	const auto& pairValue = _pair._secondValue;
	return pairValue._start;
}

_VECTOR_OUTSIDE_TEMPLATE_
constexpr inline NODISCARD Vector<_Element_, _Allocator_>::ConstPointer 
	Vector<_Element_, _Allocator_>::constData() const noexcept 
{
	const auto& pairValue = _pair._secondValue;
	return pairValue._start;
}

_VECTOR_OUTSIDE_TEMPLATE_
constexpr inline NODISCARD Vector<_Element_, _Allocator_>::Iterator 
	Vector<_Element_, _Allocator_>::begin() noexcept
{
	return Iterator(this);
}

_VECTOR_OUTSIDE_TEMPLATE_
constexpr inline NODISCARD Vector<_Element_, _Allocator_>::ConstIterator 
	Vector<_Element_, _Allocator_>::begin() const noexcept
{
	return ConstIterator(this);
}

_VECTOR_OUTSIDE_TEMPLATE_
constexpr inline NODISCARD Vector<_Element_, _Allocator_>::ConstIterator 
	Vector<_Element_, _Allocator_>::cbegin() const noexcept
{
	return ConstIterator(this);
}

_VECTOR_OUTSIDE_TEMPLATE_
constexpr inline NODISCARD Vector<_Element_, _Allocator_>::ConstIterator 
	Vector<_Element_, _Allocator_>::constBegin() const noexcept
{
	return ConstIterator(this);
}

_VECTOR_OUTSIDE_TEMPLATE_
constexpr inline NODISCARD Vector<_Element_, _Allocator_>::Iterator 
	Vector<_Element_, _Allocator_>::end() noexcept 
{
	return Iterator(this) + size();
}

_VECTOR_OUTSIDE_TEMPLATE_
constexpr inline NODISCARD Vector<_Element_, _Allocator_>::ConstIterator
	Vector<_Element_, _Allocator_>::end() const noexcept
{
	return ConstIterator(this) + size();
}

_VECTOR_OUTSIDE_TEMPLATE_
constexpr inline NODISCARD Vector<_Element_, _Allocator_>::ConstIterator 
	Vector<_Element_, _Allocator_>::cend() const 
{
	return ConstIterator(this) + size();
}

_VECTOR_OUTSIDE_TEMPLATE_
constexpr inline NODISCARD Vector<_Element_, _Allocator_>::ConstIterator
	Vector<_Element_, _Allocator_>::constEnd() const noexcept
{
	return ConstIterator(this) + size();
}

_VECTOR_OUTSIDE_TEMPLATE_
constexpr inline NODISCARD Vector<_Element_, _Allocator_>::ReverseIterator 
	Vector<_Element_, _Allocator_>::rbegin() noexcept 
{
	return ReverseIterator(begin());
}

_VECTOR_OUTSIDE_TEMPLATE_
constexpr inline NODISCARD Vector<_Element_, _Allocator_>::ReverseIterator 
	Vector<_Element_, _Allocator_>::rend() noexcept 
{
	return ReverseIterator(end());
}

_VECTOR_OUTSIDE_TEMPLATE_
constexpr inline NODISCARD Vector<_Element_, _Allocator_>::ConstReverseIterator 
	Vector<_Element_, _Allocator_>::rbegin() const noexcept
{
	return ConstReverseIterator(begin());
}

_VECTOR_OUTSIDE_TEMPLATE_
constexpr inline NODISCARD Vector<_Element_, _Allocator_>::ConstReverseIterator 
	Vector<_Element_, _Allocator_>::rend() const noexcept
{
	return ConstReverseIterator(end());
}

_VECTOR_OUTSIDE_TEMPLATE_
constexpr inline NODISCARD Vector<_Element_, _Allocator_>::ConstReverseIterator 
	Vector<_Element_, _Allocator_>::crbegin() const noexcept
{
	return ConstReverseIterator(begin());
}

_VECTOR_OUTSIDE_TEMPLATE_
constexpr inline NODISCARD Vector<_Element_, _Allocator_>::ConstReverseIterator 
	Vector<_Element_, _Allocator_>::crend() const noexcept 
{
	return ConstReverseIterator(end());
}

_VECTOR_OUTSIDE_TEMPLATE_
constexpr inline NODISCARD Vector<_Element_, _Allocator_>::ValueType 
	Vector<_Element_, _Allocator_>::front() const noexcept
{
	return at(0);
}

_VECTOR_OUTSIDE_TEMPLATE_
constexpr inline NODISCARD Vector<_Element_, _Allocator_>::Reference 
	Vector<_Element_, _Allocator_>::front() noexcept 
{
	return at(0);
}

_VECTOR_OUTSIDE_TEMPLATE_
constexpr inline NODISCARD Vector<_Element_, _Allocator_>::ValueType 
	Vector<_Element_, _Allocator_>::back() const noexcept 
{
	return at(size() - 1);
}

_VECTOR_OUTSIDE_TEMPLATE_
constexpr inline NODISCARD Vector<_Element_, _Allocator_>::Reference 
	Vector<_Element_, _Allocator_>::back() noexcept
{
	return at(size() - 1);
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::push_back(const ValueType& element) {
	emplaceBack(element);
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::push_back(Vector&& other) {
	
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void  Vector<_Element_, _Allocator_>::push_back(ValueType&& element) {
	emplaceBack(std::move(element));
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::append(const ValueType& element) {
	emplaceBack(element);
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::append(Vector&& other) {
	
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::append(ValueType&& element) {
	emplaceBack(std::move(element));
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::prepend(const ValueType& element) {

}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::prepend(Vector&& other) {

}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::prepend(ValueType&& element) {

}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::push_front(const ValueType& element) {

}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::push_front(Vector&& other) {

}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::push_front(ValueType&& element) {

}

_VECTOR_OUTSIDE_TEMPLATE_
template <class ... _Args_>
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>::Reference
	Vector<_Element_, _Allocator_>::emplaceBack(_Args_&&... args)
{
	emplaceBack(std::forward<_Args_>(args)...);
}

_VECTOR_OUTSIDE_TEMPLATE_
template <class ... _Args_>
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>::Reference 
	Vector<_Element_, _Allocator_>::emplace_back(_Args_&&... args)
{
	emplaceBack(std::forward<_Args_>(args)...);
}

_VECTOR_OUTSIDE_TEMPLATE_
template <class... _Valty_>
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>::Iterator 
	Vector<_Element_, _Allocator_>::emplace(
		ConstIterator where,
		_Valty_&&... value)
{
	return Iterator(this);
}

_VECTOR_OUTSIDE_TEMPLATE_
template <class... _Valty_>
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>::Iterator 
	Vector<_Element_, _Allocator_>::emplace(
		SizeType where,
		_Valty_&&... value)
{
	return Iterator(this);
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>::Iterator 
	Vector<_Element_, _Allocator_>::insert(
		ConstIterator where,
		const ValueType& value)
{
	return Iterator(this);
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>::Iterator
	Vector<_Element_, _Allocator_>::insert(
		ConstIterator where,
		ValueType&& value)
{
	return Iterator(this);
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>::Iterator 
	Vector<_Element_, _Allocator_>::insert(
		ConstIterator where,
		BASE_GUARDOVERFLOW const SizeType count,
		const ValueType& value)
{
	return Iterator(this);
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline Vector<_Element_, _Allocator_>::Iterator 
	Vector<_Element_, _Allocator_>::insert(
		ConstIterator where,
		std::initializer_list<ValueType> initializerList)
{
	return Iterator(this);
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::assign(
	BASE_GUARDOVERFLOW const SizeType newSize,
	const ValueType& value)
{

}

_VECTOR_OUTSIDE_TEMPLATE_
template <class _InputIterator_>
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::assign(
	_InputIterator_ _First,
	_InputIterator_ _Last)
{

}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::assign(
	const std::initializer_list<ValueType> initializerList)
{

}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>::ValueType 
	Vector<_Element_, _Allocator_>::pop() noexcept 
{
	const auto value = back();
	removeAt(size() - 1);

	return value;
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>::ValueType 
	Vector<_Element_, _Allocator_>::popBack() noexcept 
{
	return {};
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>::ValueType 
	Vector<_Element_, _Allocator_>::pop_back() noexcept
{
	return {};
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>::ValueType 
	Vector<_Element_, _Allocator_>::pop_front() noexcept
{
	return {};
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>::ValueType 
	Vector<_Element_, _Allocator_>::popFront() noexcept
{
	return {};
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::removeAt(
	BASE_GUARDOVERFLOW const size_type index) noexcept 
{

}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::clear() {
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

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>::Iterator
	Vector<_Element_, _Allocator_>::erase(
		ConstIterator first,
		ConstIterator last) noexcept(
			std::is_nothrow_move_assignable_v<ValueType>)
{
	auto& pairValue = _pair._secondValue;
	auto& allocator = _pair.first();

	if (size() <= 0)
		return;

	memory::DeallocateRange(
		memory::UnFancy(first),
		memory::UnFancy(last), allocator);

	return Iterator(this);
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>::Iterator
	Vector<_Element_, _Allocator_>::erase(ConstIterator it) noexcept(
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

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::reserve(BASE_GUARDOVERFLOW size_type newCapacity) {
	if (UNLIKELY(newCapacity <= capacity()) || UNLIKELY(newCapacity > maxSize())) // something to do (reserve() never shrinks)
		return;

	const auto isEnoughMemory = resizeReallocate(newCapacity);

	if (UNLIKELY(isEnoughMemory == false))
		_VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::shrinkToFit() {

}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::shrink_to_fit() {

}


_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD bool Vector<_Element_, _Allocator_>::resize(
	SizeType size,
	const_reference _Fill)
{
	const auto isEnoughMemory = resize(size);

	if (isEnoughMemory)
		fill(_Fill);

	return isEnoughMemory;
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::fill(const_reference _Fill) {
	for (SizeType i = 0; i < capacity(); ++i)
		insert(i, _Fill);
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD bool Vector<_Element_, _Allocator_>::resize(
	BASE_GUARDOVERFLOW const SizeType newCapacity)
{
	const auto growth			= calculateGrowth(newCapacity);
	const auto isEnoughMemory	= resizeReallocate(growth);

	return isEnoughMemory;
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>::Iterator
	Vector<_Element_, _Allocator_>::insert(
		size_type index,
		const_reference element)
{
	auto& pairValue		= _pair._secondValue;
	const auto _Offset	= index * sizeof(ValueType);

	emplaceAt(pairValue._start + _Offset, element);
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>::size_type
	Vector<_Element_, _Allocator_>::count(const ValueType& element) const noexcept 
{
	return static_cast<size_type>(0);
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>::size_type
	Vector<_Element_, _Allocator_>::count(const Vector& subVector) const noexcept
{
	return static_cast<size_type>(0);
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD bool 
	Vector<_Element_, _Allocator_>::contains(const ValueType& element) const noexcept 
{
	return false;
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD bool 
	Vector<_Element_, _Allocator_>::contains(const Vector& subVector) const noexcept 
{
	return false;
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD bool Vector<_Element_, _Allocator_>
	::startsWith(const ValueType& element) const noexcept 
{
	return false;
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD bool Vector<_Element_, _Allocator_>
	::startsWith(const Vector& subVector) const noexcept
{
	return false;
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD bool Vector<_Element_, _Allocator_>
	::endsWith(const ValueType& element) const noexcept 
{
	return false;
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD bool Vector<_Element_, _Allocator_>
	::endsWith(const Vector& subVector) const noexcept 
{
	return false;
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>::SizeType 
	Vector<_Element_, _Allocator_>::indexOf(const ValueType& element) const noexcept
{
	return 0;
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>::SizeType 
	Vector<_Element_, _Allocator_>::lastIndexOf(const ValueType& element) const noexcept 
{
	return 0;
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>	
	Vector<_Element_, _Allocator_>::sliced(
		SizeType positionFrom,
		SizeType elementsCount) const noexcept
{
	return {};
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_> 
	Vector<_Element_, _Allocator_>::sliced(SizeType positionFrom) const noexcept 
{
	return {};
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::replace(
	SizeType position,
	const ValueType& value)
{

}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::replace(
	SizeType position,
	ValueType&& value) 
{

}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::swap(Vector& other) {

}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::swapElementsAt(
	SizeType _First,
	SizeType _Last)
{

}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::swapElementsAt(
	ConstIterator _First,
	ConstIterator _Last)
{

}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::remove(
	SizeType _First,
	SizeType _Count)
{

}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::remove(
	ConstIterator _First,
	ConstIterator _Last)
{

}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>::SizeType 
	Vector<_Element_, _Allocator_>::removeAll(const ValueType& element)
{
	return false;
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::removeFirst() {

}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::removeLast() {

}

_VECTOR_OUTSIDE_TEMPLATE_
template <
	typename _Predicate_,
	typename = std::enable_if_t<std::is_function_v<_Predicate_>>>
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>::SizeType 
	Vector<_Element_, _Allocator_>::removeIf(_Predicate_ pred)
{
	return 0;
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD bool Vector<_Element_, _Allocator_>
	::removeOne(const ValueType& element)
{
	return false;
}

_VECTOR_OUTSIDE_TEMPLATE_
constexpr inline NODISCARD sizetype Vector<_Element_, _Allocator_>::max_size() const noexcept {
	return static_cast<std::size_t>(-1) / sizeof(ValueType);
}

_VECTOR_OUTSIDE_TEMPLATE_
constexpr inline NODISCARD sizetype Vector<_Element_, _Allocator_>::maxSize() const noexcept {
	return static_cast<std::size_t>(-1) / sizeof(ValueType);
}

_VECTOR_OUTSIDE_TEMPLATE_
constexpr inline NODISCARD Vector<_Element_, _Allocator_>::SizeType 
	Vector<_Element_, _Allocator_>::calculateGrowth(SizeType newSize) const noexcept
{
	const auto oldCapacity	= capacity();
	const auto _MaxSize		= maxSize();

	if (oldCapacity > _MaxSize - oldCapacity / 2)
		return _MaxSize; // Overflow

	const SizeType geometricGrowth = oldCapacity + oldCapacity / 2;

	if (geometricGrowth < newSize)
		return newSize;

	return geometricGrowth;
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD bool Vector<_Element_, _Allocator_>::resizeReallocate(SizeType newCapacity) noexcept {
	const auto oldCapacity = capacity();

	if (UNLIKELY(oldCapacity == newCapacity))
		return false;

	const auto bytesRequired = static_cast<SizeType>(newCapacity * sizeof(ValueType));

	if (UNLIKELY(bytesRequired <= 0))
		return false;

	auto& allocator = _pair.first();
	pointer memory = allocator.allocate(bytesRequired);

	if (UNLIKELY(memory == nullptr))
		_VECTOR_NOT_ENOUGH_MEMORY_DEBUG_(false)

	const auto oldSize	= size();
	auto& pairValue		= _pair._secondValue;

	pointer& _Start		= pairValue._start;
	pointer& _End		= pairValue._end;
	pointer& _Current	= pairValue._current;

	auto& blockStart = memory;
	auto blockEnd = memory + newCapacity;

	if (LIKELY(oldSize != 0) && newCapacity >= oldCapacity)
		memory::MemoryCopyCommon(
			begin(), end(),
			blockStart, blockEnd);

	_Start = blockStart;
	_End = blockEnd;

	_Current = blockStart + oldSize;

	return true;
}

_VECTOR_OUTSIDE_TEMPLATE_
template <class ..._Valty_>
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>::Reference 
	Vector<_Element_, _Allocator_>::emplaceBack(_Valty_&&... _Val)
{
	auto& pairValue		= _pair._secondValue;

	pointer& _End		= pairValue._end;
	pointer& _Current	= pairValue._current;

	if (_Current != _End)
		return emplaceBackWithUnusedCapacity(std::forward<_Valty_>(_Val)...);

	emplaceBackReallocate(std::forward<_Valty_>(_Val)...);
}

_VECTOR_OUTSIDE_TEMPLATE_
template <class... _Valty_>
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>::Reference
	Vector<_Element_, _Allocator_>::emplaceBackWithUnusedCapacity(_Valty_&&... _Val) 
{
	auto& pairValue = _pair._secondValue;
	auto& allocator = _pair.first();

	emplaceAt(
		allocator, pairValue._current,
		std::forward<_Valty_>(_Val)...);
}

_VECTOR_OUTSIDE_TEMPLATE_
template <class... _Valty_>
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>::Reference 
	Vector<_Element_, _Allocator_>::emplaceBackReallocate(_Valty_&&... _Val) 
{
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

_VECTOR_OUTSIDE_TEMPLATE_
template <class... _Valty_>
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>::Reference 
	Vector<_Element_, _Allocator_>::emplaceAt(
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

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::FreeAllElements() noexcept {
	auto& pairValue = _pair._secondValue;
	auto& allocator = _pair.first();

	pointer& _Start = pairValue._start;
	pointer& _End	= pairValue._end;

	memory::FreeRange(_Start, _End, allocator);

	pairValue._current = nullptr;
}

__BASE_CONTAINER_NAMESPACE_END
