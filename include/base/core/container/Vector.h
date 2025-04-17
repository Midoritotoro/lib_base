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

#include <base/core/memory/MemoryPointerConversion.h>
#include <base/core/memory/MemoryPlacement.h>

#include <base/core/memory/MemoryAllocatorUtility.h>
#include <base/core/memory/MemoryTypeTraits.h>

WARNING_DISABLE_MSVC(4834)
WARNING_DISABLE_MSVC(4002)
WARNING_DISABLE_MSVC(4003)
WARNING_DISABLE_MSVC(VCR001)

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

#  ifndef _VECTOR_TOO_LONG_DEBUG_
#    define _VECTOR_TOO_LONG_DEBUG_(_RetVal)									\
    _VECTOR_ERROR_DEBUG_(														\
      "base::container::Vector: Vector too long.\n ",							\
        _RetVal)
#  endif

#  ifndef _VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_
#    define _VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_								\
    _VECTOR_ERROR_DEBUG_NO_RET_(												\
      "base::container::Vector: Not enough memory to expand the Vector.\n ")
#  endif

#  ifndef _VECTOR_TOO_LONG_DEBUG_NO_RET_
#    define _VECTOR_TOO_LONG_DEBUG_NO_RET_												\
    _VECTOR_ERROR_DEBUG_NO_RET_(														\
      "base::container::Vector: Vector too long.\n ")
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

#  ifndef _VECTOR_TOO_LONG_DEBUG_
#    define _VECTOR_TOO_LONG_DEBUG_(_RetVal)									((void)0)
#  endif

#  ifndef _VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_
#    define _VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_								((void)0)
#  endif

#  ifndef _VECTOR_TOO_LONG_DEBUG_NO_RET_
#    define _VECTOR_TOO_LONG_DEBUG_NO_RET_										((void)0)
#  endif

#endif

#ifndef _VECTOR_OUTSIDE_TEMPLATE_ 
#  define _VECTOR_OUTSIDE_TEMPLATE_ template <typename _Element_, class _Allocator_>
#endif

#ifndef BASE_SILENCE_ALL_VECTOR_WARNINGS

#  ifndef _VECTOR_PUSH_FRONT_INEFFICIENT_WARNING_
#    define _VECTOR_PUSH_FRONT_INEFFICIENT_WARNING_ \
	BASE_DEPRECATED_WARNING(						\
		"base::container::Vector::pushFront: Using the pushFront method is inefficient in terms of performance")
#endif

#else

#  ifndef _VECTOR_PUSH_FRONT_INEFFICIENT_WARNING_
#    define _VECTOR_PUSH_FRONT_INEFFICIENT_WARNING_
#endif

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

	CONSTEXPR_CXX20 inline Vector& operator=(const std::vector<ValueType>& _Right);
	CONSTEXPR_CXX20 inline Vector& operator=(std::vector<ValueType>&& rVector);

	CONSTEXPR_CXX20 inline Vector& operator=(std::initializer_list<ValueType> initializerList);

	CONSTEXPR_CXX20 inline Reference operator[](const SizeType offset) noexcept;
	CONSTEXPR_CXX20 inline ConstReference operator[](const SizeType offset) const noexcept;

	template <typename _ComparedElementType_ = ValueType>
	CONSTEXPR_CXX20 inline compare_eq_result_container<
		Vector, _ComparedElementType_> operator==(const Vector& other) const noexcept;

	template <typename _ComparedElementType_ = ValueType>
	CONSTEXPR_CXX20 inline compare_eq_result_container<
		Vector, _ComparedElementType_> operator!=(const Vector& other) const noexcept;

	template <typename _ComparedElementType_ = ValueType>
	CONSTEXPR_CXX20 inline compare_lt_result_container<
		Vector, _ComparedElementType_> operator<(const Vector& other) const noexcept(
				noexcept(std::lexicographical_compare<
					typename Vector<_ComparedElementType_>::const_iterator,
				typename Vector::const_iterator>(
					std::declval<Vector<_ComparedElementType_>>().begin(), 
					std::declval<Vector<_ComparedElementType_>>().end(),
				other.begin(), other.end())));

	template <typename _ComparedElementType_ = ValueType>
	CONSTEXPR_CXX20 inline compare_lt_result_container<
		Vector, _ComparedElementType_> operator>(const Vector& other) const noexcept(
				noexcept(other < std::declval<Vector<_ComparedElementType_>>()));

	template <typename _ComparedElementType_ = ValueType>
	CONSTEXPR_CXX20 inline compare_lt_result_container<
		Vector, _ComparedElementType_> operator<=(const Vector& other) const noexcept(
			noexcept(other < std::declval<Vector<_ComparedElementType_>>()));

	template <typename _ComparedElementType_ = ValueType>
	CONSTEXPR_CXX20 inline compare_lt_result_container<
		Vector, _ComparedElementType_> operator>=(const Vector& other) const noexcept(
			noexcept(std::declval<Vector<_ComparedElementType_>>() < other));

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

	CONSTEXPR_CXX20 inline void pushFront(const ValueType& element);
	CONSTEXPR_CXX20 inline void pushFront(Vector&& other);
	CONSTEXPR_CXX20 inline void pushFront(ValueType&& element);

	CONSTEXPR_CXX20 inline void pushBack(const ValueType& element);
	CONSTEXPR_CXX20 inline void pushBack(Vector&& other);
	CONSTEXPR_CXX20 inline void pushBack(ValueType&& element);

	template <class ... _Valty_>
	CONSTEXPR_CXX20 inline NODISCARD Reference emplaceBack(_Valty_&&... _Val);

	template <class ... _Args_>
	CONSTEXPR_CXX20 inline NODISCARD Reference emplace_back(_Args_&&... args);

	template <class ... _Args_>
	CONSTEXPR_CXX20 inline NODISCARD Reference emplaceFront(_Args_&&... _Val);

	template <class ... _Args_>
	CONSTEXPR_CXX20 inline NODISCARD Reference emplace_front(_Args_&&... args);

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

	CONSTEXPR_CXX20 inline void reverse() noexcept;

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
	CONSTEXPR_CXX20 inline NODISCARD bool resize(
		BASE_GUARDOVERFLOW const SizeType newCapacity);

	CONSTEXPR_CXX20 inline void fill(const_reference _Fill);

	CONSTEXPR_CXX20 inline NODISCARD size_type count(const ValueType& element) const noexcept;
	CONSTEXPR_CXX20 inline NODISCARD size_type count(const Vector& subVector) const noexcept;

	template <typename _Predicate_>
	CONSTEXPR_CXX20 inline NODISCARD size_type count_if(_Predicate_ predicate) const noexcept;

	template <typename _Predicate_>
	CONSTEXPR_CXX20 inline NODISCARD size_type countIf(_Predicate_ predicate) const noexcept;

	CONSTEXPR_CXX20 inline NODISCARD bool contains(const ValueType& element) const noexcept;
	CONSTEXPR_CXX20 inline NODISCARD bool contains(const Vector& subVector) const noexcept;

	CONSTEXPR_CXX20 inline NODISCARD bool startsWith(const ValueType& element) const noexcept;
	CONSTEXPR_CXX20 inline NODISCARD bool startsWith(const Vector& subVector) const noexcept;

	CONSTEXPR_CXX20 inline NODISCARD bool endsWith(const ValueType& element) const noexcept;
	CONSTEXPR_CXX20 inline NODISCARD bool endsWith(const Vector& subVector) const noexcept;

	CONSTEXPR_CXX20 inline NODISCARD SizeType indexOf(const ValueType& element) const noexcept;
	CONSTEXPR_CXX20 inline NODISCARD SizeType lastIndexOf(const ValueType& element) const noexcept;

	CONSTEXPR_CXX20 inline NODISCARD SizeType find(const ValueType& element) const noexcept;

	template <typename _Predicate_>
	CONSTEXPR_CXX20 inline NODISCARD SizeType find_if(_Predicate_ predicate) const noexcept;

	template <typename _Predicate_>
	CONSTEXPR_CXX20 inline NODISCARD SizeType findIf(_Predicate_ predicate) const noexcept;

	CONSTEXPR_CXX20 inline NODISCARD Vector	sliced(
		SizeType positionFrom,
		SizeType elementsCount) const noexcept;
	CONSTEXPR_CXX20 inline NODISCARD Vector	sliced(SizeType positionFrom) const noexcept;

	// The same as sliced, but returns a non-owning slice of the vector without copying data.
	CONSTEXPR_CXX20 inline NODISCARD Vector	view(
		SizeType positionFrom,
		SizeType elementsCount) const noexcept;
	CONSTEXPR_CXX20 inline NODISCARD Vector	view(SizeType positionFrom) const noexcept;

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

	CONSTEXPR_CXX20 inline ValueType& first();
	CONSTEXPR_CXX20 inline const ValueType& first() const noexcept;
	CONSTEXPR_CXX20 inline const ValueType& constFirst() const noexcept;

	CONSTEXPR_CXX20 inline ValueType& last();
	CONSTEXPR_CXX20 inline const ValueType& last() const noexcept;
	CONSTEXPR_CXX20 inline const ValueType& constLast() const noexcept;

	CONSTEXPR_CXX20 inline NODISCARD SizeType removeAll(const ValueType& element);

	CONSTEXPR_CXX20 inline void removeFirst();
	CONSTEXPR_CXX20 inline void removeLast();

	template <typename _Predicate_>
	CONSTEXPR_CXX20 inline NODISCARD SizeType removeIf(_Predicate_ pred);	

	CONSTEXPR_CXX20 inline NODISCARD bool removeOne(const ValueType& element);

	CONSTEXPR_CXX20 inline void take(
		const pointer	newVectorStart,
		const SizeType	newVectorSize,
		const SizeType	newVectorCapacity);

	constexpr inline NODISCARD sizetype max_size() const noexcept;
	constexpr inline NODISCARD sizetype maxSize() const noexcept;
private: 
	CONSTEXPR_CXX20 inline Vector(
		const pointer _First,
		const pointer _Current,
		const pointer _Last) noexcept; // For view method

	constexpr inline NODISCARD SizeType calculateGrowth(SizeType newSize) const noexcept;
	CONSTEXPR_CXX20 inline NODISCARD bool resizeReallocate(SizeType newCapacity) noexcept;

	template <class... _Valty_>
	CONSTEXPR_CXX20 inline void emplaceBackWithUnusedCapacity(_Valty_&&... _Val);

	template <class... _Valty_>
	CONSTEXPR_CXX20 inline void emplaceBackReallocate(_Valty_&&... _Val);

	template <class... _Valty_>
	CONSTEXPR_CXX20 inline void emplaceFrontWithUnusedCapacity(_Valty_&&... _Val);

	template <class... _Valty_>
	CONSTEXPR_CXX20 inline void emplaceFrontReallocate(_Valty_&&... _Val);

	template <class... _Valty_>
	CONSTEXPR_CXX20 inline void emplaceAt(
		allocator_type& _Allocator,
		pointer&		_Location,
		_Valty_&&...	_Val);

	template <typename _Type_ = ValueType>
	CONSTEXPR_CXX20 inline NODISCARD bool elementsCompare(
		const Vector<_Type_>& other) const noexcept;

	template <class _Iterator_>
	CONSTEXPR_CXX20 inline void appendCountedRange(
		_Iterator_							_First,
		BASE_GUARDOVERFLOW	const SizeType	_Count) noexcept;
	
	template <
		class _Iterator_,
		class _Sentinel_>
	CONSTEXPR_CXX20 inline void appendUnCountedRange(
		_Iterator_ _First,
		_Sentinel_ _Last) noexcept;

	template <class _Iterator_>
	CONSTEXPR_CXX20 inline void prependCountedRange(
		_Iterator_							_First,
		BASE_GUARDOVERFLOW	const SizeType	_Count) noexcept;

	template <
		class _Iterator_,
		class _Sentinel_>
	CONSTEXPR_CXX20 inline void prependUnCountedRange(
		_Iterator_ _First,
		_Sentinel_ _Last) noexcept;

	CONSTEXPR_CXX20 inline void moveAssignUnEqualAllocator(Vector& other);

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
	const auto _SizeForAppend = elements.size();

	if (_SizeForAppend == 0)
		return;

	appendCountedRange(elements.begin(), _SizeForAppend);
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

	if (isEnoughMemory == false)
		_VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_

	if (memory::MemoryCopy(_pair._secondValue._start, _Start, _Capacity) == false)
		_VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline Vector<_Element_, _Allocator_>::
	Vector(const std::vector<ValueType>& vector) noexcept :
		_pair(_Zero_then_variadic_args_{}, VectorValueType())
{
	const auto _Capacity	= vector.capacity();
	auto& _Start			= _pair.second()._start;

	const auto isEnoughMemory = resize(_Capacity);

	if (isEnoughMemory == false)
		_VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_

	if (memory::MemoryCopy(_Start, vector.data(), _Capacity) == false)
		_VECTOR_ERROR_DEBUG_NO_RET_("base::container::Vector::Vector: Error when copying elements. ")
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline Vector<_Element_, _Allocator_>
	::Vector(const SizeType _Capacity) noexcept :
		_pair(_Zero_then_variadic_args_{}, VectorValueType())
{
	const auto isEnoughMemory = resize(_Capacity);

	if (isEnoughMemory == false)
		_VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline Vector<_Element_, _Allocator_>::Vector(
		const SizeType _Capacity,
		const ValueType& _Fill) noexcept :
	_pair(_Zero_then_variadic_args_{}, VectorValueType())
{
	const auto isEnoughMemory = resize(_Capacity);

	if (isEnoughMemory == false)
		_VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_

	fill(_Fill);
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline Vector<_Element_, _Allocator_>
	::Vector(Vector&& rOther) noexcept :
		_pair(std::exchange(rOther._pair, {}))
{}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline Vector<_Element_, _Allocator_>::Vector(
	const pointer _First,
	const pointer _Current,
	const pointer _Last) noexcept : 
	// For view method
		_pair(_Zero_then_variadic_args_{}, VectorValueType(
			_First, _Last, _Current
		))
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
		std::vector<ValueType>&& rVector)
{
	auto& pairValue = _pair._secondValue;
	auto& allocator	= _pair.first();

	auto& otherAllocator = rVector.get_allocator();

	constexpr auto _Pocma_val = memory::ChoosePocma_v<allocator_type>;

	if constexpr (_Pocma_val == memory::PocmaValues::NoPropagateAllocators) {
		if (allocator != otherAllocator) {
			// moveAssignUnEqualAllocator(_Right);
			return *this;
		}
	}

	FreeAllElements();

	// ...

	memory::POCMA(allocator, otherAllocator);
	pairValue.takeContents(rVector._pair._secondValue);


	return *this;
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
			moveAssignUnEqualAllocator(_Right);
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
		const std::vector<ValueType>& _Right)
{
	return *this;
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline Vector<_Element_, _Allocator_>& 
	Vector<_Element_, _Allocator_>::operator=(
		std::initializer_list<ValueType> initializerList) 
{
	const auto _SizeForAppend = initializerList.size();

	if (_SizeForAppend == 0)
		return;

	appendCountedRange(initializerList.begin(), _SizeForAppend);

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
template <typename _ComparedElementType_>
CONSTEXPR_CXX20 inline compare_eq_result_container<
	Vector<_Element_, _Allocator_>, _ComparedElementType_> 
		Vector<_Element_, _Allocator_>::operator==(const Vector& other) const noexcept
{
	if (size() != other.size())
		return false;

	if (begin() == other.begin())
		return true;

	// Element-by-element comparison
	return elementsCompare(other);
}

_VECTOR_OUTSIDE_TEMPLATE_
template <typename _ComparedElementType_>
CONSTEXPR_CXX20 inline compare_eq_result_container<
	Vector<_Element_, _Allocator_>, _ComparedElementType_> 
		Vector<_Element_, _Allocator_>::operator!=(const Vector& other) const noexcept
{
	return !(*this == other);
}

_VECTOR_OUTSIDE_TEMPLATE_
template <typename _ComparedElementType_>
CONSTEXPR_CXX20 inline compare_lt_result_container<
	Vector<_Element_, _Allocator_>, _ComparedElementType_> 
		Vector<_Element_, _Allocator_>::operator<(const Vector& other) const noexcept(
			noexcept(std::lexicographical_compare<
				typename Vector<_ComparedElementType_>::const_iterator,
				typename Vector::const_iterator>(
					std::declval<Vector<_ComparedElementType_>>().begin(),
					std::declval<Vector<_ComparedElementType_>>().end(),
					other.begin(), other.end())))
{
	return std::lexicographical_compare(
		begin(), end(),
		other.begin(), other.end());
}

_VECTOR_OUTSIDE_TEMPLATE_
template <typename _ComparedElementType_>
CONSTEXPR_CXX20 inline compare_lt_result_container<
	Vector<_Element_, _Allocator_>, _ComparedElementType_> 
		Vector<_Element_, _Allocator_>::operator>(const Vector& other) const noexcept(
			noexcept(other < std::declval<Vector<_ComparedElementType_>>()))
{
	return other < *this;
}

_VECTOR_OUTSIDE_TEMPLATE_
template <typename _ComparedElementType_>
CONSTEXPR_CXX20 inline compare_lt_result_container<
	Vector<_Element_, _Allocator_>, _ComparedElementType_> 
		Vector<_Element_, _Allocator_>::operator<=(const Vector& other) const noexcept(
			noexcept(other < std::declval<Vector<_ComparedElementType_>>()))
{
	return !(other < *this);
}

_VECTOR_OUTSIDE_TEMPLATE_
template <typename _ComparedElementType_>
CONSTEXPR_CXX20 inline compare_lt_result_container<
	Vector<_Element_, _Allocator_>, _ComparedElementType_> 
		Vector<_Element_, _Allocator_>::operator>=(const Vector& other) const noexcept(
			noexcept(std::declval<Vector<_ComparedElementType_>>() < other))
{
	return !(*this < other);
}

_VECTOR_OUTSIDE_TEMPLATE_
constexpr inline NODISCARD Vector<_Element_, _Allocator_>::Reference 
	Vector<_Element_, _Allocator_>::at(const SizeType offset) noexcept 
{
#ifdef _DEBUG
	const auto pairValue	= _pair._secondValue;

	const auto _Start		= pairValue._start;
	const auto _End			= pairValue._end;

	const auto isValidOffset = (_Start + offset >= _Start
		&& _Start + offset <= _End);

	_VECTOR_DEBUG_ASSERT_LOG_(isValidOffset, "base::container::VectorBase::operator[]: Index out of range. ", {}); // ? 
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
	auto& otherPairValue	= other._pair._secondValue;
	auto& otherAllocator	= other._pair.first();

	pointer& otherStart		= otherPairValue._start;
	pointer& otherCurrent	= otherPairValue._current;
	pointer& otherEnd		= otherPairValue._end;

	const auto otherSize	= static_cast<SizeType>(
		otherCurrent - otherStart);

	if (otherSize == 0)
		return;

	appendCountedRange(otherStart, otherSize);
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
	push_back(std::move(other));
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::append(ValueType&& element) {
	emplaceBack(std::move(element));
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::prepend(const ValueType& element) {
	push_front(element);
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::prepend(Vector&& other) {
	push_front(std::move(other));
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::prepend(ValueType&& element) {
	push_front(std::move(element));
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::push_front(const ValueType& element) {
	_VECTOR_PUSH_FRONT_INEFFICIENT_WARNING_
	emplaceFront(element);
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::push_front(Vector&& other) {
	_VECTOR_PUSH_FRONT_INEFFICIENT_WARNING_

	auto& otherPairValue	= other._pair._secondValue;
	auto& otherAllocator	= other._pair.first();

	pointer& otherStart		= otherPairValue._start;
	pointer& otherCurrent	= otherPairValue._current;

	const auto otherSize = static_cast<SizeType>(
		otherCurrent - otherStart);

	if (otherSize == 0)
		return;

	appendCountedRange(otherStart, otherSize);
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::push_front(ValueType&& element) {
	_VECTOR_PUSH_FRONT_INEFFICIENT_WARNING_
	emplaceFront(std::move(element));
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::pushFront(const ValueType& element) {
	push_front(element);
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::pushFront(Vector&& other) {
	push_front(std::move(other));
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::pushFront(ValueType&& element) {
	push_front(std::move(element));
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::pushBack(const ValueType& element) {
	emplaceBack(element);
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::pushBack(Vector&& other) {
	push_back(std::move(other));
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void  Vector<_Element_, _Allocator_>::pushBack(ValueType&& element) {
	emplaceBack(std::move(element));
}

_VECTOR_OUTSIDE_TEMPLATE_
template <class ... _Args_>
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>::Reference 
	Vector<_Element_, _Allocator_>::emplace_back(_Args_&&... args)
{
	emplaceBack(std::forward<_Args_>(args)...);
	return back();
}

_VECTOR_OUTSIDE_TEMPLATE_
template <class ... _Args_>
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>::Reference 
	Vector<_Element_, _Allocator_>::emplaceFront(_Args_&&... _Val) 
{
	auto& pairValue = _pair._secondValue;
	auto& allocator = _pair.first();

	pointer& _Start = pairValue._start;
	pointer& _Current = pairValue._current;
	pointer& _End = pairValue._end;

	constexpr auto valuesForAppendCount = sizeof...(_Val);

	const auto _Size = static_cast<SizeType>(_Current - _Start);
	const auto _Capacity = static_cast<SizeType>(_End - _Start);
	const auto _UnusedCapacity = static_cast<SizeType>(_End - _Current);

	if (_Capacity == 0) {
		const auto isEnoughMemory = resize(valuesForAppendCount);

		if (isEnoughMemory == false)
			_VECTOR_NOT_ENOUGH_MEMORY_DEBUG_(front())

		if constexpr (valuesForAppendCount == 1) {
			emplaceBack(std::forward<_Args_>(_Val)...);
			return front();
		}
		else {
			auto argsReversedTuple = reverseTuple(std::make_tuple(_Val...));
			emplaceBack(std::forward<decltype(argsReversedTuple)>(
				argsReversedTuple)...);
		}
	}
	else if (valuesForAppendCount < _UnusedCapacity) {

	}
	else {
		const auto isEnoughMemory = resize(valuesForAppendCount);

		if (isEnoughMemory == false)
			_VECTOR_NOT_ENOUGH_MEMORY_DEBUG_(front())


	}

	return front();
}

_VECTOR_OUTSIDE_TEMPLATE_
template <class ... _Args_>
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>::Reference 
	Vector<_Element_, _Allocator_>::emplace_front(_Args_&&... args) 
{
	return emplaceFront(std::forward<_Args_>(args)...);
}

_VECTOR_OUTSIDE_TEMPLATE_
template <class... _Valty_>
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>::Iterator 
	Vector<_Element_, _Allocator_>::emplace(
		ConstIterator where,
		_Valty_&&... value)
{
	return insert(where, std::forward<_Valty_>(value)...);
}

_VECTOR_OUTSIDE_TEMPLATE_
template <class... _Valty_>
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>::Iterator 
	Vector<_Element_, _Allocator_>::emplace(
		SizeType where,
		_Valty_&&... value)
{
	return insert(where, std::forward<_Valty_>(value)...);
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
	return pop();
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>::ValueType 
	Vector<_Element_, _Allocator_>::pop_back() noexcept
{
	return pop();
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>::ValueType 
	Vector<_Element_, _Allocator_>::pop_front() noexcept
{
	const auto value = front();
	removeAt(0);

	return value;
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>::ValueType 
	Vector<_Element_, _Allocator_>::popFront() noexcept
{
	return pop_front();
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::reverse() noexcept {
	auto& pairValue = _pair._secondValue;
	
	pointer& _Start = pairValue._start;
	pointer& _End	= pairValue._end;

	if (_Start == _End)
		return;

	std::reverse(_Start, _End);
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
	if (newCapacity <= capacity() || newCapacity > maxSize()) // something to do (reserve() never shrinks)
		return;

	const auto isEnoughMemory = resizeReallocate(newCapacity);

	if (isEnoughMemory == false)
		_VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::shrinkToFit() {
	auto& pairValue		= _pair._secondValue;
	auto& allocator		= _pair.first();

	pointer& _Current	= pairValue._current;
	pointer& _End		= pairValue._end;

	if (_Current == _End)
		return;

	memory::FreeRange(_Current, _End, allocator);

	_End = _Current;
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::shrink_to_fit() {
	return shrinkToFit();
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
	auto& pairValue = _pair._secondValue;
	auto& allocator = _pair.first();

	const auto _Size = size();

	UNUSED(memory::UninitializedFillCount(
		pairValue._start, _Size,
		_Fill, allocator));
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
	SizeType _Count = 0;

	for (SizeType i = 0; i < size(); ++i) {
		const auto& currentElement = at(i);

		if (currentElement == element)
			++_Count;
	}

	return _Count;
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>::size_type
	Vector<_Element_, _Allocator_>::count(const Vector& subVector) const noexcept
{
	SizeType _Ñount = 0;
	
	const auto subVectorSize	= subVector.size();
	const auto _Size			= size();

	if (subVectorSize == 0 || subVectorSize > size())
		return;

	for (SizeType i = 0; i < _Size; ++i) {
		SizeType overlaps = 0;

		const auto& currentElement = at(i);

		if (subVector[i % subVectorSize] == currentElement) {
			if ((++overlaps) == subVectorSize) {
				overlaps = 0;
				++_Ñount;
			}
		}
	}

	return _Ñount;
}

_VECTOR_OUTSIDE_TEMPLATE_
template <typename _Predicate_>
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>::size_type 
	Vector<_Element_, _Allocator_>::count_if(_Predicate_ predicate) const noexcept
{
	SizeType _Count = 0;

	for (SizeType i = 0; i < size(); ++i) {
		const auto& currentElement = at(i);

		if (predicate(currentElement))
			++_Count;
	}

	return _Count;
}

_VECTOR_OUTSIDE_TEMPLATE_
template <typename _Predicate_>
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>::size_type 
	Vector<_Element_, _Allocator_>::countIf(_Predicate_ predicate) const noexcept
{
	return count_if(predicate);
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD bool 
	Vector<_Element_, _Allocator_>::contains(const ValueType& element) const noexcept 
{
	for (SizeType i = 0; i < size(); ++i) {
		const auto& currentElement = at(i);

		if (currentElement == element)
			return true;
	}

	return false;
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD bool 
	Vector<_Element_, _Allocator_>::contains(const Vector& subVector) const noexcept 
{
	const auto subVectorSize = subVector.size();
	const auto _Size = size();

	if (subVectorSize == 0 || subVectorSize > size())
		return;

	for (SizeType i = 0; i < _Size; ++i) {
		SizeType overlaps = 0;

		const auto& currentElement = at(i);

		if (subVector[i % subVectorSize] == currentElement
			&& (++overlaps) == subVectorSize
		)
			return true;
	}

	return false;
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD bool Vector<_Element_, _Allocator_>
	::startsWith(const ValueType& element) const noexcept 
{
	const auto& firstElement = first();
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
	return find(element);
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>::SizeType 
	Vector<_Element_, _Allocator_>::lastIndexOf(const ValueType& element) const noexcept 
{
	for (SizeType i = size() - 1; i >= 0; --i) {
		auto& currentElement = at(i);

		if (currentElement == element)
			return i;
	}

	return -1;
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>::SizeType 
	Vector<_Element_, _Allocator_>::find(const ValueType& element) const noexcept 
{
	for (SizeType i = 0; i < size(); ++i) {
		auto& currentElement = at(i);

		if (currentElement == element)
			return i;
	}

	return -1;
}

_VECTOR_OUTSIDE_TEMPLATE_
template <typename _Predicate_>
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>::SizeType 
	Vector<_Element_, _Allocator_>::find_if(_Predicate_ predicate) const noexcept
{
	for (SizeType i = 0; i < size(); ++i) 
		if (predicate(at(i)))
			return i;

	return -1;
}

_VECTOR_OUTSIDE_TEMPLATE_
template <typename _Predicate_>
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>::SizeType 
	Vector<_Element_, _Allocator_>::findIf(_Predicate_ predicate) const noexcept
{
	return find_if(predicate);
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>	
	Vector<_Element_, _Allocator_>::sliced(
		SizeType positionFrom,
		SizeType elementsCount) const noexcept
{
	const auto _Size = size();

	if (positionFrom < 0 || positionFrom >= _Size)
		return {};

	if (elementsCount < 0 || (positionFrom + elementsCount) > _Size)
		return {};

	auto& pairValue = _pair._secondValue;

	pointer& _Start = pairValue._start;
	pointer& _Current = pairValue._current;

	const auto _Capacity = static_cast<SizeType>(pairValue._end - _Start);
	pointer _StartWithOffset = _Start + positionFrom;

	Vector newVector(_Capacity);

	UNUSED(memory::UninitializedCopyUnchecked(
		_StartWithOffset, _StartWithOffset + elementsCount,
		newVector._pair._secondValue._start));

	return std::move(newVector);
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_> 
	Vector<_Element_, _Allocator_>::sliced(SizeType positionFrom) const noexcept 
{
	const auto _Size = size();

	if (positionFrom < 0 || positionFrom >= _Size)
		return {};

	auto& pairValue = _pair._secondValue;

	pointer& _Start = pairValue._start;
	pointer& _Current = pairValue._current;

	const auto _Capacity = static_cast<SizeType>(pairValue._end - _Start);
	pointer _StartWithOffset = _Start + positionFrom;

	Vector newVector(_Capacity);

	UNUSED(memory::UninitializedCopyUnchecked(
		_StartWithOffset, _Current,
		newVector._pair._secondValue._start));

	return std::move(newVector);
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>
	Vector<_Element_, _Allocator_>::view(
		SizeType positionFrom,
		SizeType elementsCount) const noexcept
{
	const auto _Size = size();

	if (positionFrom < 0 || positionFrom >= _Size)
		return {};

	if (elementsCount < 0 || (positionFrom + elementsCount) > _Size)
		return {};

	auto& pairValue			= _pair._secondValue;
	auto _StartWithOffset	= pairValue._start + positionFrom;

	return Vector(
		_StartWithOffset, _StartWithOffset /* start as current */,
		_StartWithOffset + elementsCount);
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_> 
	Vector<_Element_, _Allocator_>::view(SizeType positionFrom) const noexcept
{
	const auto _Size = size();

	if (positionFrom < 0 || positionFrom >= _Size)
		return {};

	auto& pairValue			= _pair._secondValue;
	auto _StartWithOffset	= pairValue._start + positionFrom;

	return Vector(
		_StartWithOffset, _StartWithOffset /* start as current */,
		pairValue._end);
};

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
	if (this == memory::AddressOf(other))
		return;

	// check allocators
	// ...
	
	_pair.swap(other._pair);
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
CONSTEXPR_CXX20 inline Vector<_Element_, _Allocator_>::ValueType& 
	Vector<_Element_, _Allocator_>::first()
{
	Assert(!isEmpty(), "base::container::Vector::first: An attempt to get the first element of an empty Vector");
	return at(0); 
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline const Vector<_Element_, _Allocator_>::ValueType& 
	Vector<_Element_, _Allocator_>::first() const noexcept 
{
	Assert(!isEmpty(), "base::container::Vector::first: An attempt to get the first element of an empty Vector");
	return at(0);
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline const Vector<_Element_, _Allocator_>::ValueType& 
	Vector<_Element_, _Allocator_>::constFirst() const noexcept 
{
	Assert(!isEmpty(), "base::container::Vector::constFirst: An attempt to get the first element of an empty Vector");
	return at(0);
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline Vector<_Element_, _Allocator_>::ValueType& 
	Vector<_Element_, _Allocator_>::last()
{
	Assert(!isEmpty(), "base::container::Vector::last: An attempt to get the last element of an empty Vector");
	return at(size() - 1);
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline const Vector<_Element_, _Allocator_>::ValueType& 
	Vector<_Element_, _Allocator_>::last() const noexcept 
{
	Assert(!isEmpty(), "base::container::Vector::last: An attempt to get the last element of an empty Vector");
	return at(size() - 1);
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline const Vector<_Element_, _Allocator_>::ValueType& 
	Vector<_Element_, _Allocator_>::constLast() const noexcept
{
	Assert(!isEmpty(), "base::container::Vector::constLast: An attempt to get the last element of an empty Vector");
	return at(size() - 1);
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
template <typename _Predicate_>
CONSTEXPR_CXX20 inline NODISCARD Vector<_Element_, _Allocator_>::SizeType 
	Vector<_Element_, _Allocator_>::removeIf(_Predicate_ pred)
{
	for (SizeType i = 0; i < size(); ++i)
		if (pred(at(i)))
			return i;

	return -1;
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline NODISCARD bool Vector<_Element_, _Allocator_>
	::removeOne(const ValueType& element)
{
	//for (SizeType i = 0; i < size(); ++i) {
	//	auto& currentElement = at(i);
	//	if (currentElement == element) {
	//		auto& pairValue = _pair._secondValue;
	//		auto& allocator = _pair.first();

	//		pointer& _Start = pairValue._start;
	//		pointer& _Current = pairValue._current;

	//		pointer elementForRemove = _Start + i;

	//		std::allocator_traits<allocator_type>::destroy(allocator, )

	//		memory::MoveUnChecked()

	//		return true;
	//	}
	//}

	//return false;
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::take(
	const pointer	newVectorStart,
	const SizeType	newVectorSize,
	const SizeType	newVectorCapacity)
{
	auto& allocator = _pair.first();
	auto& pairValue = _pair._secondValue;

	pointer& _Start = pairValue._start;
	pointer& _End = pairValue._end;
	pointer& _Current = pairValue._current;;

	if (_Start) {
		// destroy and deallocate old array
		const auto _Capacity = static_cast<SizeType>(_End - _Start);

		memory::DestroyRange(_Start, _Current, allocator);
		allocator.deallocate(_Start, _Capacity);
	}

	_Start = newVectorStart;
	_Current = newVectorStart + newVectorSize;
	_End = newVectorStart + newVectorCapacity;
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

	if (oldCapacity == newCapacity)
		return false;

	auto& allocator		= _pair.first();
	pointer memory		= allocator.allocate(newCapacity);

	if (memory == nullptr)
		_VECTOR_NOT_ENOUGH_MEMORY_DEBUG_(false)

	const auto oldSize	= size();
	auto& pairValue		= _pair._secondValue;

	pointer& _Start		= pairValue._start;
	pointer& _End		= pairValue._end;
	pointer& _Current	= pairValue._current;

	auto& blockStart	= memory;
	auto blockEnd		= memory + newCapacity;

	if (oldSize != 0 && newCapacity >= oldCapacity)
		memory::MemoryCopyCommon(
			begin(), end(),
			blockStart, blockEnd);

	_Start	= blockStart;
	_End	= blockEnd;

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

	if (_Current != _End) {
		emplaceBackWithUnusedCapacity(std::forward<_Valty_>(_Val)...);
		return back();
	}

	emplaceBackReallocate(std::forward<_Valty_>(_Val)...);
	return back();
}

_VECTOR_OUTSIDE_TEMPLATE_
template <class... _Valty_>
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::emplaceBackWithUnusedCapacity(_Valty_&&... _Val) 
{
	auto& pairValue = _pair._secondValue;
	auto& allocator = _pair.first();

	emplaceAt(
		allocator, pairValue._current,
		std::forward<_Valty_>(_Val)...);
}

_VECTOR_OUTSIDE_TEMPLATE_
template <class... _Valty_>
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::emplaceBackReallocate(_Valty_&&... _Val) 
{
	auto& pairValue				= _pair._secondValue;
	auto& allocator				= _pair.first();

	const auto _NewSize			= static_cast<SizeType>(
		sizeof...(_Val) + capacity());
	const auto _IsEnoughMemory	= resize(_NewSize);

	if (UNLIKELY(_IsEnoughMemory == false))
		_VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_

	emplaceAt(
		allocator, pairValue._current,
		std::forward<_Valty_>(_Val)...);
}

_VECTOR_OUTSIDE_TEMPLATE_
template <class... _Valty_>
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::emplaceFrontWithUnusedCapacity(_Valty_&&... _Val) {

}

_VECTOR_OUTSIDE_TEMPLATE_
template <class... _Valty_>
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::emplaceFrontReallocate(_Valty_&&... _Val) {

}

_VECTOR_OUTSIDE_TEMPLATE_
template <class... _Valty_>
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::emplaceAt(
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
template <typename _Type_>
CONSTEXPR_CXX20 inline NODISCARD bool Vector<_Element_, _Allocator_>::elementsCompare(
	const Vector<_Type_>& other) const noexcept
{
	auto _Iterations = SizeType{ 0 };
	_Iterations = std::min(other.size(), size());

	for (size_type _Current = 0; _Current < _Iterations; ++_Current)
		if (at(_Current) != other.at(_Current))
			return false;

	return true;
}

_VECTOR_OUTSIDE_TEMPLATE_
template <class _Iterator_>
// insert counted range _First + [0, _Count) at end
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::appendCountedRange(
	_Iterator_							_First,
	BASE_GUARDOVERFLOW const SizeType	_Count) noexcept
{
	if (_Count <= 0)
		return;

	auto& pairValue			= _pair._secondValue;
	auto& allocator			= _pair.first();

	pointer& _Start			= pairValue._start;
	pointer& _End			= pairValue._end;
	pointer& _Current		= pairValue._current;

	const auto _OldUnusedCapacity	= static_cast<SizeType>(_End - _Current);
	const auto _OldCapacity			= static_cast<SizeType>(_End - _Start);
	const auto _OldSize				= static_cast<SizeType>(_Current - _Start);

	if (_OldUnusedCapacity < _Count) {
		// reallocate 

		if (_Count > maxSize() - _OldSize)
			_VECTOR_TOO_LONG_DEBUG_NO_RET_

		const auto _NewCapacity	= calculateGrowth(_OldCapacity + _Count);
		const auto _NewSize		= _OldSize + _Count;

		auto _BlockStart		= allocator.allocate(_NewCapacity);

		if (_BlockStart == nullptr)
			_VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_

		auto _BlockEnd			= _BlockStart + _NewSize;
		auto _InsertStart		= _BlockStart + _OldSize;

		BASE_TRY_BEGIN
		
		memory::UninitializedCopyCount(
			std::move(_First), _Count, 
			_BlockStart + _OldSize, allocator);

		if (_Count == 1) {
			if constexpr (
				std::is_nothrow_move_constructible_v<ValueType>
				|| std::is_copy_constructible_v<ValueType> == false
			) {
				memory::UninitializedMove(
					_Start, _Current,
					_BlockStart, allocator);
			}
			else {
				memory::UninitializedCopy(
					_Start, _Current,
					_BlockStart, allocator);
			}
		}
		else {
			memory::UninitializedMove(
				_Start, _Current,
				_BlockStart, allocator);
		}

		BASE_CATCH_ALL

		memory::DestroyRange(_Current, _Current + _Count, allocator);
		allocator.deallocate(_BlockStart, _NewCapacity);

		BASE_RERAISE;
		BASE_CATCH_END

		take(_BlockStart, _NewSize, _NewCapacity);
	}
	else {
		UNUSED(memory::UninitializedCopyCount(
			std::move(_First), _Count,
			_Start, allocator));
	}
}

_VECTOR_OUTSIDE_TEMPLATE_
template <
	class _Iterator_,
	class _Sentinel_>
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::appendUnCountedRange(
	_Iterator_ _First,
	_Sentinel_ _Last) noexcept
{
	for (; _First != _Last; ++_First)
		UNUSED(emplaceBack(*_First));
}

_VECTOR_OUTSIDE_TEMPLATE_
template <class _Iterator_>
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::prependCountedRange(
	_Iterator_							_First,
	BASE_GUARDOVERFLOW	const SizeType	_Count) noexcept
{
	if (_Count <= 0)
		return;
}

_VECTOR_OUTSIDE_TEMPLATE_
template <
	class _Iterator_,
	class _Sentinel_>
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::prependUnCountedRange(
	_Iterator_ _First,
	_Sentinel_ _Last) noexcept
{
	for (; _First != _Last; ++_First)
		UNUSED(emplaceFront(*_First));
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::moveAssignUnEqualAllocator(Vector& other) {
	auto& pairValue = _pair._secondValue;
	auto& allocator	= _pair.first();

	auto& otherPairValue = other._pair._secondValue;

	const pointer otherStart	= otherPairValue._start;
	const pointer otherCurrent	= otherPairValue._current;

	const auto otherSize		= static_cast<SizeType>(
		otherCurrent - otherStart);

	pointer& _Start		= pairValue._start;
	pointer& _Current	= pairValue._current;
	pointer& _End		= pairValue._End;

	const auto oldCapacity = static_cast<SizeType>(_End - _Start);

	if (otherSize > oldCapacity) {
//		_Clear_and_reserve_geometric(_Newsize);

		_Current = memory::UninitializedMove(otherStart, otherCurrent, _Start, allocator);
		return;
	}

	const auto oldSize = static_cast<SizeType>(_Current - _Start);

	if (otherSize > oldSize) {
		const pointer _Mid = otherStart + oldSize;
		memory::MoveUnChecked(otherStart, _Mid, _Start);

		_Current = memory::UninitializedMove(
			_Mid, otherCurrent,
			_Current, allocator);
	}
	else {
		const pointer _Newlast = _Start + otherSize;

		memory::MoveUnChecked(otherStart, otherCurrent, _Start);
		memory::DestroyRange(_Newlast, _Current, allocator);

		_Current = _Newlast;
	}
}

_VECTOR_OUTSIDE_TEMPLATE_
CONSTEXPR_CXX20 inline void Vector<_Element_, _Allocator_>::FreeAllElements() noexcept {
	auto& pairValue		= _pair._secondValue;
	auto& allocator		= _pair.first();

	pointer& _Start		= pairValue._start;
	pointer& _End		= pairValue._end;

	if (_Start == _End)
		return;

	memory::FreeRange(_Start, _End, allocator);

	pairValue._current = nullptr;
}

__BASE_CONTAINER_NAMESPACE_END
