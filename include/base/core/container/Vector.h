#pragma once

// All warnings defined in this file can be disabled using 
//	#define BASE_SILENCE_ALL_VECTOR_WARNINGS.
// Calling std::terminate in all Debug-Only asserts on failure can be disabled using
//	#define BASE_VECTOR_DEBUG_ASSERT_NO_FAILURE

#include <base/core/container/VectorIterator.h>

#include <base/core/container/CompressedPair.h>
#include <base/core/memory/Memory.h>

#include <base/core/container/VectorDebug.h>
#include <base/core/container/VectorValue.h>


base_disable_warning_msvc(4834)
base_disable_warning_msvc(4002)
base_disable_warning_msvc(4003)


__BASE_CONTAINER_NAMESPACE_BEGIN

struct VectorSimd {
	// If SSE4.2 is supported on the device being launched,
	// then these instructions are used.
	// Otherwise, it is similar to _Optimization_Disable_
	struct _Optimization_Enable_Only_If_SSE42_ {
	};

	// If AVX is supported on the device being launched,
	// then these instructions are used.
	// Otherwise, it is similar to _Optimization_Disable_
	struct _Optimization_Enable_Only_If_AVX_ {
	};

	// If AVX512 is supported on the device being launched,
	// then these instructions are used.
	// Otherwise, it is similar to _Optimization_Disable_
	struct _Optimization_Enable_Only_If_AVX512_ {
	};

	// Auto-selecting the most efficient option
	struct _Optimization_Auto_ {
	};

	// A fully scalar vector
	struct _Optimization_Disable_ {
	};
};

enum class _Vector_SIMD_Algorithm_Alignment : sizetype {
};

template <
	typename	_Element_,
	class		_Allocator_ 		= std::allocator<_Element_>,
	class 		_SimdOptimization_ 	= VectorSimd::_Optimization_Disable_>
class Vector
{
	using AllocatorForType			= memory::RebindAllocator<_Allocator_, _Element_>; 
	using AllocatorTraits			= std::allocator_traits<AllocatorForType>;
public:
	using value_type 				= _Element_;
	using allocator_type 			= _Allocator_;

	using pointer 					= value_type*;
	using const_pointer 			= const pointer;

	using size_type 				= sizetype;
	using difference_type 			= ptrdiff;

	using reference 				= value_type&;
	using const_reference 			= const value_type&;

	using iterator 					= VectorIterator<Vector<
		_Element_, _Allocator_, _SimdOptimization_>>;
	using const_iterator 			= VectorConstIterator<Vector<
		_Element_, _Allocator_, _SimdOptimization_>>;

	using reverse_iterator 			= std::reverse_iterator<iterator>;
	using const_reverse_iterator 	= std::reverse_iterator<const_iterator>;

	using Iterator		 			= iterator;
	using ConstIterator 			= const_iterator;

	using ValueType 				= value_type;
	using SizeType	 				= size_type;

	using Reference 				= reference;
	using ConstReference 			= const_reference;

	using Pointer 					= pointer;
	using ConstPointer 				= const_pointer;

	using ReverseIterator 			= reverse_iterator;
	using ConstReverseIterator 		= const_reverse_iterator;
private:
	using VectorValueType = VectorValue<Vector<_Element_, _Allocator_, _SimdOptimization_>>;

	inline static constexpr auto _Vector_Default_Capacity_ = 16;
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

	base_constexpr_cxx20 inline Vector(std::initializer_list<ValueType> elements) noexcept;
	base_constexpr_cxx20 inline Vector(const Vector& other) noexcept;

	base_constexpr_cxx20 inline Vector(const std::vector<ValueType>& vector) noexcept;
	base_constexpr_cxx20 inline Vector(const SizeType _Capacity) noexcept;

	template <class _InputIterator_>
	base_constexpr_cxx20 inline Vector(
		_InputIterator_ _First, 
		_InputIterator_ _Last) noexcept;

	base_constexpr_cxx20 inline Vector(
		const SizeType _Capacity,
		const ValueType& _Fill) noexcept;
	base_constexpr_cxx20 inline Vector(Vector&& rOther) noexcept;

	base_constexpr_cxx20 inline   allocator_type& get_allocator() noexcept;
	base_constexpr_cxx20 inline   const allocator_type& get_allocator() const noexcept;

	base_constexpr_cxx20 inline   allocator_type& getAllocator() noexcept;
	base_constexpr_cxx20 inline   const allocator_type& getAllocator() const noexcept;

	base_constexpr_cxx20 inline Vector& operator=(Vector&& _Right) noexcept(
		memory::ChoosePocma_v<allocator_type> != memory::PocmaValues::NoPropagateAllocators);

	base_constexpr_cxx20 inline Vector& operator=(const Vector& _Right);

	base_constexpr_cxx20 inline Vector& operator=(const std::vector<ValueType>& _Right);
	base_constexpr_cxx20 inline Vector& operator=(std::vector<ValueType>&& rVector);

	base_constexpr_cxx20 inline Vector& operator=(std::initializer_list<ValueType> initializerList);

	base_constexpr_cxx20 inline Reference operator[](const SizeType offset) noexcept;
	base_constexpr_cxx20 inline ConstReference operator[](const SizeType offset) const noexcept;

	template <typename _ComparedElementType_ = ValueType>
	base_constexpr_cxx20 inline compare_eq_result_container<
		Vector, _ComparedElementType_> operator==(const Vector& other) const noexcept;

	template <typename _ComparedElementType_ = ValueType>
	base_constexpr_cxx20 inline compare_eq_result_container<
		Vector, _ComparedElementType_> operator!=(const Vector& other) const noexcept;

	template <typename _ComparedElementType_ = ValueType>
	base_constexpr_cxx20 inline compare_lt_result_container<
		Vector, _ComparedElementType_> operator<(const Vector& other) const noexcept(
				noexcept(std::lexicographical_compare<
					typename Vector<_ComparedElementType_>::const_iterator,
				typename Vector::const_iterator>(
					std::declval<Vector<_ComparedElementType_>>().begin(), 
					std::declval<Vector<_ComparedElementType_>>().end(),
				other.begin(), other.end())));

	template <typename _ComparedElementType_ = ValueType>
	base_constexpr_cxx20 inline compare_lt_result_container<
		Vector, _ComparedElementType_> operator>(const Vector& other) const noexcept(
				noexcept(other < std::declval<Vector<_ComparedElementType_>>()));

	template <typename _ComparedElementType_ = ValueType>
	base_constexpr_cxx20 inline compare_lt_result_container<
		Vector, _ComparedElementType_> operator<=(const Vector& other) const noexcept(
			noexcept(other < std::declval<Vector<_ComparedElementType_>>()));

	template <typename _ComparedElementType_ = ValueType>
	base_constexpr_cxx20 inline compare_lt_result_container<
		Vector, _ComparedElementType_> operator>=(const Vector& other) const noexcept(
			noexcept(std::declval<Vector<_ComparedElementType_>>() < other));

	base_constexpr_cxx20 inline   Reference at(const SizeType offset) noexcept;
	base_constexpr_cxx20 inline   ValueType at(const SizeType index) const noexcept;

	base_constexpr_cxx20 inline   SizeType count() const noexcept;
	base_constexpr_cxx20 inline   SizeType size() const noexcept;
	base_constexpr_cxx20 inline   SizeType length() const noexcept;

	base_constexpr_cxx20 inline   SizeType capacity() const noexcept;
	base_constexpr_cxx20 inline   SizeType unusedCapacity() const noexcept;

	base_constexpr_cxx20 inline   bool isEmpty() const noexcept;

	base_constexpr_cxx20 inline   Pointer data() noexcept;
	base_constexpr_cxx20 inline   ConstPointer data() const noexcept;

	base_constexpr_cxx20 inline   ConstPointer constData() const noexcept;

	base_constexpr_cxx20 inline   Iterator begin() noexcept;
	base_constexpr_cxx20 inline   ConstIterator begin() const noexcept;

	base_constexpr_cxx20 inline   ConstIterator cbegin() const noexcept;
	base_constexpr_cxx20 inline   ConstIterator constBegin() const noexcept;

	base_constexpr_cxx20 inline   Iterator end() noexcept;
	base_constexpr_cxx20 inline   ConstIterator end() const noexcept;

	base_constexpr_cxx20 inline   ConstIterator cend() const;
	base_constexpr_cxx20 inline   ConstIterator constEnd() const noexcept;

	base_constexpr_cxx20 inline   ReverseIterator rbegin() noexcept;
	base_constexpr_cxx20 inline   ReverseIterator rend() noexcept;

	base_constexpr_cxx20 inline   ConstReverseIterator rbegin() const noexcept;
	base_constexpr_cxx20 inline   ConstReverseIterator rend() const noexcept;

	base_constexpr_cxx20 inline   ConstReverseIterator crbegin() const noexcept;
	base_constexpr_cxx20 inline   ConstReverseIterator crend() const noexcept;

	base_constexpr_cxx20 inline   ValueType front() const noexcept;
	base_constexpr_cxx20 inline   Reference front() noexcept;

	base_constexpr_cxx20 inline   ValueType back() const noexcept;
	base_constexpr_cxx20 inline   Reference back() noexcept;
	
	base_constexpr_cxx20 inline void push_back(const ValueType& element);
	base_constexpr_cxx20 inline void push_back(Vector&& other);
	base_constexpr_cxx20 inline void push_back(ValueType&& element);

	base_constexpr_cxx20 inline void append(const ValueType& element);
	base_constexpr_cxx20 inline void append(Vector&& other);
	base_constexpr_cxx20 inline void append(ValueType&& element);

	base_constexpr_cxx20 inline void prepend(const ValueType& element);
	base_constexpr_cxx20 inline void prepend(Vector&& other);
	base_constexpr_cxx20 inline void prepend(ValueType&& element);

	base_constexpr_cxx20 inline void push_front(const ValueType& element);
	base_constexpr_cxx20 inline void push_front(Vector&& other);
	base_constexpr_cxx20 inline void push_front(ValueType&& element);

	base_constexpr_cxx20 inline void pushFront(const ValueType& element);
	base_constexpr_cxx20 inline void pushFront(Vector&& other);
	base_constexpr_cxx20 inline void pushFront(ValueType&& element);

	base_constexpr_cxx20 inline void pushBack(const ValueType& element);
	base_constexpr_cxx20 inline void pushBack(Vector&& other);
	base_constexpr_cxx20 inline void pushBack(ValueType&& element);

	template <class ... _Valty_>
	base_constexpr_cxx20 inline   Reference emplaceBack(_Valty_&&... _Val);

	template <class ... _Args_>
	base_constexpr_cxx20 inline   Reference emplace_back(_Args_&&... args);

	template <class ... _Args_>
	base_constexpr_cxx20 inline   Reference emplaceFront(_Args_&&... _Val);

	template <class ... _Args_>
	base_constexpr_cxx20 inline   Reference emplace_front(_Args_&&... args);

	template <class... _Valty_>
	base_constexpr_cxx20 inline   Iterator emplace(
		ConstIterator where, 
		_Valty_&&... value);

	template <class... _Valty_>
	base_constexpr_cxx20 inline   Iterator emplace(
		SizeType where,
		_Valty_&&... value);

	base_constexpr_cxx20 inline   Iterator insert(
		SizeType index,
		const_reference element);

	base_constexpr_cxx20 inline   Iterator insert(
		ConstIterator where, 
		const ValueType& value);

	base_constexpr_cxx20 inline   Iterator insert(
		ConstIterator where,
		ValueType&& value);

	base_constexpr_cxx20 inline   Iterator insert(
		ConstIterator where, 
		const SizeType count, 
		const ValueType& value);

	base_constexpr_cxx20 inline Iterator insert(
		ConstIterator where,
		std::initializer_list<ValueType> initializerList);

	base_constexpr_cxx20 inline void assign(
		const SizeType newSize,
		const ValueType& value);

	template <class _InputIterator_>
	base_constexpr_cxx20 inline void assign(
		_InputIterator_ _First,
		_InputIterator_ _Last);

	base_constexpr_cxx20 inline void assign(
		const std::initializer_list<ValueType> initializerList);

	base_constexpr_cxx20 inline   ValueType pop() noexcept;

	base_constexpr_cxx20 inline   ValueType popBack() noexcept;
	base_constexpr_cxx20 inline   ValueType pop_back() noexcept;

	base_constexpr_cxx20 inline   ValueType pop_front() noexcept;
	base_constexpr_cxx20 inline   ValueType popFront() noexcept;

	base_constexpr_cxx20 inline void reverse() noexcept;

	base_constexpr_cxx20 inline void removeAt(const SizeType index) noexcept;

	base_constexpr_cxx20 inline void clear();

	// Deletes the specified elements from the container
	// Moves the elements of the vector that follow the part being deleted to the place of the elements being deleted
	base_constexpr_cxx20 inline   Iterator erase(
		ConstIterator first,
		ConstIterator last) noexcept(
			std::is_nothrow_move_assignable_v<ValueType>);

	// Deletes the specified element from the container
	// Moves the elements of the vector that follow the part being deleted to the place of the element being deleted
	base_constexpr_cxx20 inline   Iterator erase(ConstIterator it) noexcept(
		std::is_nothrow_move_assignable_v<ValueType>);

	// increase capacity to newCapacity (without geometric growth)
	base_constexpr_cxx20 inline void reserve(SizeType newCapacity);

	base_constexpr_cxx20 inline void shrinkToFit();
	base_constexpr_cxx20 inline void shrink_to_fit();

	base_constexpr_cxx20	inline    bool resize(
		const SizeType size,
		const_reference _Fill);
	base_constexpr_cxx20 inline   bool resize(
		const SizeType newCapacity);

	base_constexpr_cxx20 inline void fill(const_reference _Fill);

	base_constexpr_cxx20 inline   size_type count(const ValueType& element) const noexcept;
	base_constexpr_cxx20 inline   size_type count(const Vector& subVector) const noexcept;

	template <typename _Predicate_>
	base_constexpr_cxx20 inline   size_type count_if(_Predicate_ predicate) const noexcept;

	template <typename _Predicate_>
	base_constexpr_cxx20 inline   size_type countIf(_Predicate_ predicate) const noexcept;

	base_constexpr_cxx20 inline   bool contains(const ValueType& element) const noexcept;
	base_constexpr_cxx20 inline   bool contains(const Vector& subVector) const noexcept;

	base_constexpr_cxx20 inline   bool startsWith(const ValueType& element) const noexcept;
	base_constexpr_cxx20 inline   bool startsWith(const Vector& subVector) const noexcept;

	base_constexpr_cxx20 inline   bool endsWith(const ValueType& element) const noexcept;
	base_constexpr_cxx20 inline   bool endsWith(const Vector& subVector) const noexcept;

	base_constexpr_cxx20 inline   SizeType indexOf(const ValueType& element) const noexcept;
	base_constexpr_cxx20 inline   SizeType lastIndexOf(const ValueType& element) const noexcept;

	base_constexpr_cxx20 inline   SizeType find(const ValueType& element) const noexcept;

	template <typename _Predicate_>
	base_constexpr_cxx20 inline   SizeType find_if(_Predicate_ predicate) const noexcept;

	template <typename _Predicate_>
	base_constexpr_cxx20 inline   SizeType findIf(_Predicate_ predicate) const noexcept;

	base_constexpr_cxx20 inline   Vector	sliced(
		SizeType positionFrom,
		SizeType elementsCount) const noexcept;
	base_constexpr_cxx20 inline   Vector	sliced(SizeType positionFrom) const noexcept;

	// The same as sliced, but returns a non-owning slice of the vector without copying data.
	base_constexpr_cxx20 inline   Vector	view(
		SizeType positionFrom,
		SizeType elementsCount) const noexcept;
	base_constexpr_cxx20 inline   Vector	view(SizeType positionFrom) const noexcept;

	base_constexpr_cxx20 inline void replace(
		SizeType 			positionFrom,
		const ValueType& 	oldValue,
		const ValueType& 	newValue);

	base_constexpr_cxx20 inline void replace(
		SizeType 			positionFrom,
		SizeType 			positionTo,
		const ValueType& 	oldValue,
		const ValueType& 	newValue);

	base_constexpr_cxx20 inline void replace(
		ConstIterator 		positionFrom,
		const ValueType& 	oldValue,
		const ValueType& 	newValue);

	base_constexpr_cxx20 inline void replace(
		ConstIterator 		positionFrom,
		ConstIterator 		positionTo,
		const ValueType& 	oldValue,
		const ValueType& 	newValue);

	base_constexpr_cxx20 inline void replace(
		const ValueType& oldValue,
		const ValueType& newValue);

	base_constexpr_cxx20 inline void swap(Vector& other);

	base_constexpr_cxx20 inline void swapElementsAt(
		SizeType _First,
		SizeType _Last);
	base_constexpr_cxx20 inline void swapElementsAt(
		Iterator _First,
		Iterator _Last);

	base_constexpr_cxx20 inline void remove(
		SizeType _First,
		SizeType _Count);
	base_constexpr_cxx20 inline void remove(
		ConstIterator _First,
		ConstIterator _Last);

	base_constexpr_cxx20 inline ValueType& first();
	base_constexpr_cxx20 inline const ValueType& first() const noexcept;
	base_constexpr_cxx20 inline const ValueType& constFirst() const noexcept;

	base_constexpr_cxx20 inline ValueType& last();
	base_constexpr_cxx20 inline const ValueType& last() const noexcept;
	base_constexpr_cxx20 inline const ValueType& constLast() const noexcept;

	base_constexpr_cxx20 inline void removeAll(const ValueType& element);

	base_constexpr_cxx20 inline void removeFirst();
	base_constexpr_cxx20 inline void removeLast();

	template <typename _Predicate_>
	base_constexpr_cxx20 inline void removeIf(_Predicate_ pred);	

	base_constexpr_cxx20 inline   bool removeOne(const ValueType& element);

	base_constexpr_cxx20 inline void take(
		const pointer	newVectorStart,
		const SizeType	newVectorSize,
		const SizeType	newVectorCapacity);

	base_constexpr_cxx20 inline   Vector makeView(const std::vector< _Element_>& other);
	base_constexpr_cxx20 inline   Vector makeView(const Vector& other);

	constexpr inline   sizetype max_size() const noexcept;
	constexpr inline   sizetype maxSize() const noexcept;
private: 
	base_constexpr_cxx20 inline Vector(
		const pointer _First,
		const pointer _Current,
		const pointer _Last) noexcept; // For view method

	constexpr inline   SizeType calculateGrowth(SizeType newSize) const noexcept;
	base_constexpr_cxx20 inline   bool resizeReallocate(SizeType newCapacity) noexcept;

	template <class... _Valty_>
	base_constexpr_cxx20 inline void emplaceBackWithUnusedCapacity(_Valty_&&... _Val);

	template <class... _Valty_>
	base_constexpr_cxx20 inline void emplaceBackReallocate(_Valty_&&... _Val);

	template <class... _Valty_>
	base_constexpr_cxx20 inline void emplaceAt(
		allocator_type& _Allocator,
		pointer&		_Location,
		_Valty_&&...	_Val);

	template <typename _Type_ = ValueType>
	base_constexpr_cxx20 inline   bool elementsCompare(
		const Vector<_Type_>& other) const noexcept;

	template <class _Iterator_>
	base_constexpr_cxx20 inline void appendCountedRange(
		_Iterator_							_First,
		const SizeType	_Count) noexcept;
	
	template <
		class _Iterator_,
		class _Sentinel_>
	base_constexpr_cxx20 inline void appendUnCountedRange(
		_Iterator_ _First,
		_Sentinel_ _Last) noexcept;

	template <class _Iterator_>
	base_constexpr_cxx20 inline void prependCountedRange(
		_Iterator_							_First,
		const SizeType	_Count) noexcept;

	template <
		class _Iterator_,
		class _Sentinel_>
	base_constexpr_cxx20 inline void prependUnCountedRange(
		_Iterator_ _First,
		_Sentinel_ _Last) noexcept;

	template <class _Iterator_>
	base_constexpr_cxx20 inline void insertCountedRange(
		ConstIterator						_To,
		_Iterator_							_First,
		const SizeType	_Count) noexcept;

	template <
		class _Iterator_,
		class _Sentinel_>
	base_constexpr_cxx20 inline void insertUnCountedRange(
		ConstIterator	_To,
		_Iterator_ 		_First,
		_Sentinel_ 		_Last) noexcept;


	template <
		class _Iterator_,
		class _Sentinel_>
	base_constexpr_cxx20 inline void assignUnCountedRange(
		_Iterator_ _First,
		_Sentinel_ _Last);

	template <class _Iterator_>
	base_constexpr_cxx20 inline void assignCountedRange(
		_Iterator_							_First,
		const SizeType	_NewSize);

	base_constexpr_cxx20 inline void ReallocateMoveExcept(
		const bool 		insertAtEnd, 
		const SizeType 	toOffset,
		const pointer	newVectorStart);
		
	base_constexpr_cxx20 inline void ReallocateCopyExcept(
		const bool 		insertAtEnd, 
		const SizeType 	toOffset,
		const pointer	newVectorStart);

	base_constexpr_cxx20 inline void moveAssignUnEqualAllocator(Vector& other);
	base_constexpr_cxx20 inline void clearAndReserveGeometricGrowth(SizeType newCapacity); 

	base_constexpr_cxx20 inline void FreeAllElements() noexcept;

	CompressedPair<allocator_type, VectorValueType> _pair;
};

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
inline constexpr Vector<_Element_, _Allocator_, _SimdOptimization_>::Vector() noexcept :
	_pair(_Zero_then_variadic_args_{}, VectorValueType())
{};

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
inline constexpr Vector<_Element_, _Allocator_, _SimdOptimization_>::~Vector() noexcept {
	FreeAllElements();
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline Vector<_Element_, _Allocator_, _SimdOptimization_>
	::Vector(std::initializer_list<ValueType> elements) noexcept:
		_pair(_Zero_then_variadic_args_{}, VectorValueType())
{
	const auto _SizeForAppend = elements.size();

	if (_SizeForAppend == 0)
		return;

	appendCountedRange(elements.begin(), _SizeForAppend);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
template <class _InputIterator_>
base_constexpr_cxx20 inline Vector<_Element_, _Allocator_, _SimdOptimization_>::Vector(
	_InputIterator_ _First,
	_InputIterator_ _Last) noexcept
{
	/*template <_Container_compatible_range<_Ty> _Rng>
    constexpr vector(from_range_t, _Rng&& _Range, const _Alloc& _Al = _Alloc())
        : _Mypair(_One_then_variadic_args_t{}, _Al) {
        if constexpr (_RANGES sized_range<_Rng> || _RANGES forward_range<_Rng>) {
            const auto _Length = _To_unsigned_like(_RANGES distance(_Range));
            const auto _Count  = _Convert_size<size_type>(_Length);
            _Construct_n(_Count, _RANGES _Ubegin(_Range), _RANGES _Uend(_Range));
        } else {
            auto&& _Alproxy = _GET_PROXY_ALLOCATOR(_Alty, _Getal());
            _Container_proxy_ptr<_Alty> _Proxy(_Alproxy, _Mypair._Myval2);
            _Tidy_guard<vector> _Guard{this};

            _Append_uncounted_range(_RANGES _Ubegin(_Range), _RANGES _Uend(_Range));

            _Guard._Target = nullptr;
            _Proxy._Release();
        }
    }*/
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline Vector<_Element_, _Allocator_, _SimdOptimization_>::Vector(const Vector& other) noexcept :
	_pair(_Zero_then_variadic_args_{}, VectorValueType())
{
	const auto _Capacity = other.capacity();
	auto& _Start = other._pair._secondValue._start;

	const auto isEnoughMemory = resize(_Capacity);

	if (isEnoughMemory == false)
		_VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_;

	if (memory::MemoryCopy(_pair._secondValue._start, _Start, _Capacity) == false)
		_VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_;
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline Vector<_Element_, _Allocator_, _SimdOptimization_>::
	Vector(const std::vector<ValueType>& vector) noexcept :
		_pair(_Zero_then_variadic_args_{}, VectorValueType())
{
	const auto _Capacity	= vector.capacity();
	auto& _Start			= _pair.second()._start;

	const auto isEnoughMemory = resize(_Capacity);

	if (isEnoughMemory == false)
		_VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_;

	if (memory::MemoryCopy(_Start, vector.data(), _Capacity) == false)
		_VECTOR_ERROR_DEBUG_NO_RET_("base::container::Vector::Vector: Error when copying elements. ");
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline Vector<_Element_, _Allocator_, _SimdOptimization_>
	::Vector(const SizeType _Capacity) noexcept :
		_pair(_Zero_then_variadic_args_{}, VectorValueType())
{
	const auto isEnoughMemory = resize(_Capacity);

	if (isEnoughMemory == false)
		_VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_;
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline Vector<_Element_, _Allocator_, _SimdOptimization_>::Vector(
		const SizeType _Capacity,
		const ValueType& _Fill) noexcept :
	_pair(_Zero_then_variadic_args_{}, VectorValueType())
{
	const auto isEnoughMemory = resize(_Capacity);

	if (isEnoughMemory == false)
		_VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_;

	fill(_Fill);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline Vector<_Element_, _Allocator_, _SimdOptimization_>
	::Vector(Vector&& rOther) noexcept :
		_pair(std::exchange(rOther._pair, {}))
{}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline Vector<_Element_, _Allocator_, _SimdOptimization_>::Vector(
	const pointer _First,
	const pointer _Current,
	const pointer _Last) noexcept : 
	// For view method
		_pair(_Zero_then_variadic_args_{}, VectorValueType(
			_First, _Last, _Current
		))
{}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::allocator_type&
	Vector<_Element_, _Allocator_, _SimdOptimization_>::getAllocator() noexcept
{
	return _pair.first();
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   const Vector<_Element_, _Allocator_, _SimdOptimization_>::allocator_type&
	Vector<_Element_, _Allocator_, _SimdOptimization_>::getAllocator() const noexcept
{
	return _pair.first();
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::allocator_type&
	Vector<_Element_, _Allocator_, _SimdOptimization_>::get_allocator() noexcept
{
	return _pair.first();
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   const Vector<_Element_, _Allocator_, _SimdOptimization_>::allocator_type&
	Vector<_Element_, _Allocator_, _SimdOptimization_>::get_allocator() const noexcept
{
	return _pair.first();
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline Vector<_Element_, _Allocator_, _SimdOptimization_>& 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::operator=(const Vector& _Right) 
{
	return *this;
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline Vector<_Element_, _Allocator_, _SimdOptimization_>& 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::operator=(
		std::vector<ValueType>&& rVector)
{
	auto& pairValue = _pair._secondValue;
	auto& allocator	= _pair.first();

	auto& otherAllocator = rVector.get_allocator();

	constexpr auto _Pocma_val = memory::ChoosePocma_v<allocator_type>;

	if constexpr (_Pocma_val == memory::PocmaValues::NoPropagateAllocators) {
		if (allocator != otherAllocator) {
			auto& _View = makeView(rVector);
			moveAssignUnEqualAllocator(_View);

			return *this;
		}
	}

	FreeAllElements();

	memory::POCMA(allocator, otherAllocator);
	pairValue.takeContents(rVector._pair._secondValue);

	return *this;
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 Vector<_Element_, _Allocator_, _SimdOptimization_>&
		Vector<_Element_, _Allocator_, _SimdOptimization_>::operator=(Vector&& _Right) noexcept(
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

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline Vector<_Element_, _Allocator_, _SimdOptimization_>& 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::operator=(
		const std::vector<ValueType>& _Right)
{
	if (this == memory::AddressOf(_Right))
		return *this;

	auto& pairValue 		= _pair._secondValue;
	auto& otherPairValue 	= _Right._secondValue;

	base_unused(memory::UninitializedCopy(
		otherPairValue._start, otherPairValue._current, 
		pairValue._start, getAllocator()));

}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline Vector<_Element_, _Allocator_, _SimdOptimization_>& 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::operator=(
		std::initializer_list<ValueType> initializerList) 
{
	auto _Count = SizeType(0);

	if (ConvertIntegral<SizeType>(initializerList.size(), _Count))
		assignCountedRange(initializerList.begin(), _Count);

	return *this;
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline Vector<_Element_, _Allocator_, _SimdOptimization_>::Reference
	Vector<_Element_, _Allocator_, _SimdOptimization_>::
		operator[](const SizeType offset) noexcept 
{
	return *(_pair._secondValue._current + offset);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline Vector<_Element_, _Allocator_, _SimdOptimization_>::ConstReference 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::
		operator[](const SizeType offset) const noexcept
{	
	const auto& pairValue = _pair._secondValue;
	return *(pairValue._current + offset);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
template <typename _ComparedElementType_>
base_constexpr_cxx20 inline compare_eq_result_container<
	Vector<_Element_, _Allocator_, _SimdOptimization_>, _ComparedElementType_> 
		Vector<_Element_, _Allocator_, _SimdOptimization_>::operator==(const Vector& other) const noexcept
{
	if (size() != other.size())
		return false;

	if (begin() == other.begin())
		return true;

	// Element-by-element comparison
	return elementsCompare(other);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
template <typename _ComparedElementType_>
base_constexpr_cxx20 inline compare_eq_result_container<
	Vector<_Element_, _Allocator_, _SimdOptimization_>, _ComparedElementType_> 
		Vector<_Element_, _Allocator_, _SimdOptimization_>::operator!=(const Vector& other) const noexcept
{
	return !(*this == other);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
template <typename _ComparedElementType_>
base_constexpr_cxx20 inline compare_lt_result_container<
	Vector<_Element_, _Allocator_, _SimdOptimization_>, _ComparedElementType_> 
		Vector<_Element_, _Allocator_, _SimdOptimization_>::operator<(const Vector& other) const noexcept(
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

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
template <typename _ComparedElementType_>
base_constexpr_cxx20 inline compare_lt_result_container<
	Vector<_Element_, _Allocator_, _SimdOptimization_>, _ComparedElementType_> 
		Vector<_Element_, _Allocator_, _SimdOptimization_>::operator>(const Vector& other) const noexcept(
			noexcept(other < std::declval<Vector<_ComparedElementType_>>()))
{
	return other < *this;
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
template <typename _ComparedElementType_>
base_constexpr_cxx20 inline compare_lt_result_container<
	Vector<_Element_, _Allocator_, _SimdOptimization_>, _ComparedElementType_> 
		Vector<_Element_, _Allocator_, _SimdOptimization_>::operator<=(const Vector& other) const noexcept(
			noexcept(other < std::declval<Vector<_ComparedElementType_>>()))
{
	return !(other < *this);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
template <typename _ComparedElementType_>
base_constexpr_cxx20 inline compare_lt_result_container<
	Vector<_Element_, _Allocator_, _SimdOptimization_>, _ComparedElementType_> 
		Vector<_Element_, _Allocator_, _SimdOptimization_>::operator>=(const Vector& other) const noexcept(
			noexcept(std::declval<Vector<_ComparedElementType_>>() < other))
{
	return !(*this < other);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::Reference 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::at(const SizeType offset) noexcept 
{
#ifdef _DEBUG
	const auto pairValue	= _pair._secondValue;

	const auto _Start		= pairValue._start;
	const auto _End			= pairValue._end;

	const auto isValidOffset = (_Start + offset >= _Start
		&& _Start + offset <= _End);

	_VECTOR_DEBUG_ASSERT_LOG_(isValidOffset, "base::container::VectorBase::operator[]: Index out of range. ", {});
#endif
	return (*this)[offset];
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::ValueType
	Vector<_Element_, _Allocator_, _SimdOptimization_>::at(const SizeType index) const noexcept
{
	return at(index);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::SizeType 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::size() const noexcept 
{
	const auto& pairValue = _pair._secondValue;
	return static_cast<SizeType>(pairValue._current - pairValue._start);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::SizeType 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::count() const noexcept
{
	const auto& pairValue = _pair._secondValue;
	return static_cast<SizeType>(pairValue._current - pairValue._start);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::SizeType
	Vector<_Element_, _Allocator_, _SimdOptimization_>::length() const noexcept 
{
	const auto& pairValue = _pair._secondValue;
	return static_cast<SizeType>(pairValue._current - pairValue._start);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::SizeType 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::capacity() const noexcept
{
	const auto& pairValue = _pair._secondValue;
	return static_cast<SizeType>(pairValue._end - pairValue._start);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::SizeType
	Vector<_Element_, _Allocator_, _SimdOptimization_>::unusedCapacity() const noexcept
{
	return (capacity() - size());
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   bool Vector<_Element_, _Allocator_, _SimdOptimization_>::isEmpty() const noexcept {
	return (length() == 0);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::Pointer 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::data() noexcept 
{
	auto& pairValue = _pair._secondValue;
	return pairValue._start;
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::ConstPointer
	Vector<_Element_, _Allocator_, _SimdOptimization_>::data() const noexcept 
{
	const auto& pairValue = _pair._secondValue;
	return pairValue._start;
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::ConstPointer 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::constData() const noexcept 
{
	const auto& pairValue = _pair._secondValue;
	return pairValue._start;
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::Iterator 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::begin() noexcept
{
	return Iterator(this);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::ConstIterator 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::begin() const noexcept
{
	return ConstIterator(this);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::ConstIterator 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::cbegin() const noexcept
{
	return ConstIterator(this);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::ConstIterator 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::constBegin() const noexcept
{
	return ConstIterator(this);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::Iterator 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::end() noexcept 
{
	return Iterator(this) + size();
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::ConstIterator
	Vector<_Element_, _Allocator_, _SimdOptimization_>::end() const noexcept
{
	return ConstIterator(this) + size();
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::ConstIterator 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::cend() const 
{
	return ConstIterator(this) + size();
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::ConstIterator
	Vector<_Element_, _Allocator_, _SimdOptimization_>::constEnd() const noexcept
{
	return ConstIterator(this) + size();
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::ReverseIterator 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::rbegin() noexcept 
{
	return ReverseIterator(begin());
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::ReverseIterator 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::rend() noexcept 
{
	return ReverseIterator(end());
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::ConstReverseIterator 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::rbegin() const noexcept
{
	return ConstReverseIterator(begin());
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::ConstReverseIterator 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::rend() const noexcept
{
	return ConstReverseIterator(end());
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::ConstReverseIterator 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::crbegin() const noexcept
{
	return ConstReverseIterator(begin());
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::ConstReverseIterator 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::crend() const noexcept 
{
	return ConstReverseIterator(end());
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::ValueType 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::front() const noexcept
{
	return at(0);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::Reference 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::front() noexcept 
{
	return at(0);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::ValueType 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::back() const noexcept 
{
	return at(size() - 1);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::Reference 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::back() noexcept
{
	return at(size() - 1);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::push_back(const ValueType& element) {
	emplaceBack(element);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::push_back(Vector&& other) {
	auto& otherPairValue	= other._pair._secondValue;
	auto& otherAllocator	= other._pair.first();

	pointer& otherStart		= otherPairValue._start;
	pointer& otherCurrent	= otherPairValue._current;

	const auto otherSize	= static_cast<SizeType>(
		otherCurrent - otherStart);

	if (otherSize == 0)
		return;

	appendCountedRange(otherStart, otherSize);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void  Vector<_Element_, _Allocator_, _SimdOptimization_>::push_back(ValueType&& element) {
	emplaceBack(std::move(element));
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::append(const ValueType& element) {
	emplaceBack(element);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::append(Vector&& other) {
	push_back(std::move(other));
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::append(ValueType&& element) {
	emplaceBack(std::move(element));
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::prepend(const ValueType& element) {
	push_front(element);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::prepend(Vector&& other) {
	push_front(std::move(other));
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::prepend(ValueType&& element) {
	push_front(std::move(element));
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::push_front(const ValueType& element) {
	_VECTOR_PUSH_FRONT_INEFFICIENT_WARNING_
	emplaceFront(element);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::push_front(Vector&& other) {
	_VECTOR_PUSH_FRONT_INEFFICIENT_WARNING_

	auto& otherPairValue	= other._pair._secondValue;
	auto& otherAllocator	= other._pair.first();

	pointer& otherStart		= otherPairValue._start;
	pointer& otherCurrent	= otherPairValue._current;

	const auto otherSize = static_cast<SizeType>(
		otherCurrent - otherStart);

	if (otherSize == 0)
		return;

	prependCountedRange(otherStart, otherSize);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::push_front(ValueType&& element) {
	_VECTOR_PUSH_FRONT_INEFFICIENT_WARNING_;
	emplaceFront(std::move(element));
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::pushFront(const ValueType& element) {
	push_front(element);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::pushFront(Vector&& other) {
	push_front(std::move(other));
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::pushFront(ValueType&& element) {
	push_front(std::move(element));
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::pushBack(const ValueType& element) {
	emplaceBack(element);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::pushBack(Vector&& other) {
	push_back(std::move(other));
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void  Vector<_Element_, _Allocator_, _SimdOptimization_>::pushBack(ValueType&& element) {
	emplaceBack(std::move(element));
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
template <class ... _Args_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::Reference 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::emplace_back(_Args_&&... args)
{
	emplaceBack(std::forward<_Args_>(args)...);
	return back();
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
template <class ... _Args_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::Reference 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::emplaceFront(_Args_&&... _Val) 
{
	auto& pairValue		= _pair._secondValue;
	auto& allocator		= _pair.first();

	pointer& _Start		= pairValue._start;
	pointer& _Current	= pairValue._current;
	pointer& _End		= pairValue._end;

	const auto _Size			= static_cast<SizeType>(_Current - _Start);
	const auto _Capacity		= static_cast<SizeType>(_End - _Start);
	const auto _UnusedCapacity	= static_cast<SizeType>(_End - _Current);

	if (_Capacity == 0) {
		// allocate new array
		const auto isEnoughMemory = resizeReallocate(_Vector_Default_Capacity_);

		if (isEnoughMemory == false)
			_VECTOR_NOT_ENOUGH_MEMORY_DEBUG_(front());
	}
	else if (_UnusedCapacity >= 1) {
		base_unused(memory::UninitializedMove(
			_Start, _Current, 
			_Start + 1, allocator));
	}
	else { 
		// reallocate old array
		const auto isEnoughMemory = resize(_Capacity + _Vector_Default_Capacity_);

		if (isEnoughMemory == false)
			_VECTOR_NOT_ENOUGH_MEMORY_DEBUG_(front());
			
		base_unused(memory::UninitializedMove(
			_Start, _Current,
			_Start + 1, allocator));
	}

	emplaceAt(
		allocator, _Start,
		std::forward<_Args_>(_Val)...);

	return front();
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
template <class ... _Args_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::Reference 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::emplace_front(_Args_&&... args) 
{
	return emplaceFront(std::forward<_Args_>(args)...);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
template <class... _Valty_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::Iterator 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::emplace(
		ConstIterator where,
		_Valty_&&... value)
{
	return insert(where, std::forward<_Valty_>(value)...);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
template <class... _Valty_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::Iterator 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::emplace(
		SizeType where,
		_Valty_&&... value)
{
	return insert(where, std::forward<_Valty_>(value)...);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::Iterator 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::insert(
		ConstIterator where,
		const ValueType& value)
{
	auto& pairValue = _pair._secondValue;
	pointer& _Start = pairValue._start;

	pointer& wherePointer = where._currentElement;

	_VECTOR_DEBUG_ASSERT_LOG_(
		wherePointer >= _Start && wherePointer <= pairValue._end,
		Iterator{}, _VECTOR_OUT_OF_RANGE_);

	emplaceAt(getAllocator(), wherePointer, value);

	return Iterator(wherePointer);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::Iterator
	Vector<_Element_, _Allocator_, _SimdOptimization_>::insert(
		ConstIterator where,
		ValueType&& value)
{
	auto& pairValue = _pair._secondValue;
	pointer& _Start = pairValue._start;

	pointer& wherePointer = where._currentElement;

	_VECTOR_DEBUG_ASSERT_LOG_(
		wherePointer >= _Start && wherePointer <= pairValue._end,
		Iterator{}, _VECTOR_OUT_OF_RANGE_);

	emplaceAt(
		getAllocator(), wherePointer,
		std::move(value));

	return Iterator(wherePointer);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::Iterator 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::insert(
		ConstIterator 		where,
		const SizeType 		count,
		const ValueType& 	value)
{
	if (count <= 0)
		return;

	auto& pairValue = _pair._secondValue;
	pointer& _Start = pairValue._start;

	pointer& wherePointer = where._currentElement;

	_VECTOR_DEBUG_ASSERT_LOG_(
		wherePointer >= _Start && (wherePointer + count) <= pairValue._end,
		Iterator{}, _VECTOR_OUT_OF_RANGE_);

	emplaceAt(
		getAllocator(), wherePointer,
		std::move(value));

	return Iterator(wherePointer);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline Vector<_Element_, _Allocator_, _SimdOptimization_>::Iterator 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::insert(
		ConstIterator where,
		std::initializer_list<ValueType> initializerList)
{
	const auto sizeForInsert = initializerList.size();
	
	if (sizeForInsert <= 0)
		return;

	auto& pairValue = _pair._secondValue;
	pointer& _Start = pairValue._start;

	pointer& wherePointer = where._currentElement;

	_VECTOR_DEBUG_ASSERT_LOG_(
		wherePointer >= _Start && (wherePointer + sizeForInsert) <= pairValue._end,
		Iterator{}, _VECTOR_OUT_OF_RANGE_);

	insertCountedRange(
		where, initializerList.begin(),
		initializerList.end());

	return Iterator(wherePointer);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::assign(
	const SizeType		newSize,
	const ValueType&	value)
{
	
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
template <class _InputIterator_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::assign(
	_InputIterator_ _First,
	_InputIterator_ _Last)
{
	assignUnCountedRange(std::move(_First), std::move(_Last));
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::assign(
	const std::initializer_list<ValueType> initializerList)
{
	assignUnCountedRange(initializerList.begin(), initializerList.end());
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::ValueType 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::pop() noexcept 
{
	const auto value = back();
	removeAt(size() - 1);

	return value;
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::ValueType 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::popBack() noexcept 
{
	return pop();
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::ValueType 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::pop_back() noexcept
{
	return pop();
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::ValueType 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::pop_front() noexcept
{
	const auto value = front();
	removeAt(0);

	return value;
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::ValueType 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::popFront() noexcept
{
	return pop_front();
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::reverse() noexcept {
	auto& pairValue = _pair._secondValue;
	
	pointer& _Start = pairValue._start;
	pointer& _End	= pairValue._end;

	if (_Start == _End)
		return;

	// ::base::reverse(_Start, _End);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::removeAt(
	const size_type index) noexcept 
{
	auto& pairValue = _pair._secondValue;
	auto& allocator = _pair.first();

	pointer& _Start = pairValue._start;
	pointer& _Current = pairValue._current;

	const auto _Size = static_cast<SizeType>(_Current - _Start);

	_VECTOR_DEBUG_ASSERT_LOG_(index >= 0 && index < _Size, _VECTOR_OUT_OF_RANGE_);
	
	auto& forDestroy = at(index);

	memory::DestroyInPlace(forDestroy);
	allocator.deallocate(index, 1);

	if (index != _Size - 1)
		base_unused(memory::UninitializedMove(
			_Start + index + 1, _Current,
			_Start + index, allocator));
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::clear() {
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

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::Iterator
	Vector<_Element_, _Allocator_, _SimdOptimization_>::erase(
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

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::Iterator
	Vector<_Element_, _Allocator_, _SimdOptimization_>::erase(ConstIterator it) noexcept(
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

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::reserve(size_type newCapacity) {
	if (newCapacity <= capacity() || newCapacity > maxSize()) // something to do (reserve() never shrinks)
		return;

	const auto isEnoughMemory = resizeReallocate(newCapacity);

	if (isEnoughMemory == false)
		_VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_;
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::shrinkToFit() {
	auto& pairValue		= _pair._secondValue;
	auto& allocator		= _pair.first();

	pointer& _Current	= pairValue._current;
	pointer& _End		= pairValue._end;

	if (_Current == _End)
		return;

	memory::FreeRange(_Current, _End, allocator);

	_End = _Current;
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::shrink_to_fit() {
	return shrinkToFit();
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   bool Vector<_Element_, _Allocator_, _SimdOptimization_>::resize(
	SizeType size,
	const_reference _Fill)
{
	const auto isEnoughMemory = resize(size);

	if (isEnoughMemory)
		fill(_Fill);

	return isEnoughMemory;
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::fill(const_reference _Fill) {
	auto& pairValue = _pair._secondValue;
	auto& allocator = _pair.first();

	const auto _Size = size();

	base_unused(memory::UninitializedFillCount(
		pairValue._start, _Size,
		_Fill, allocator));
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   bool Vector<_Element_, _Allocator_, _SimdOptimization_>::resize(SizeType newCapacity)
{
	const auto growth			= calculateGrowth(newCapacity);
	const auto isEnoughMemory	= resizeReallocate(growth);

	return isEnoughMemory;
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::Iterator
	Vector<_Element_, _Allocator_, _SimdOptimization_>::insert(
		SizeType index,
		const_reference element)
{
	auto& pairValue		= _pair._secondValue;
	emplaceAt(pairValue._start + index, element);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::size_type
	Vector<_Element_, _Allocator_, _SimdOptimization_>::count(const ValueType& element) const noexcept 
{
	SizeType _Count = 0;

	for (SizeType i = 0; i < size(); ++i) {
		const auto& currentElement = at(i);

		if (currentElement == element)
			++_Count;
	}

	return _Count;
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::size_type
	Vector<_Element_, _Allocator_, _SimdOptimization_>::count(const Vector& subVector) const noexcept
{
	SizeType _Count = 0;
	
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
				++_Count;
			}
		}
	}

	return _Count;
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
template <typename _Predicate_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::size_type 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::count_if(_Predicate_ predicate) const noexcept
{
	SizeType _Count = 0;

	for (SizeType i = 0; i < size(); ++i) {
		const auto& currentElement = at(i);

		if (predicate(currentElement))
			++_Count;
	}

	return _Count;
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
template <typename _Predicate_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::size_type 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::countIf(_Predicate_ predicate) const noexcept
{
	return count_if(predicate);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   bool 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::contains(const ValueType& element) const noexcept 
{
	for (SizeType i = 0; i < size(); ++i) {
		const auto& currentElement = at(i);

		if (currentElement == element)
			return true;
	}

	return false;
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   bool 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::contains(const Vector& subVector) const noexcept 
{
	const auto subVectorSize	= subVector.size();
	const auto _Size			= size();

	if (subVectorSize == 0 || subVectorSize > _Size)
		return false;

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

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   bool Vector<_Element_, _Allocator_, _SimdOptimization_>
	::startsWith(const ValueType& element) const noexcept 
{
	const auto& firstElement = first();
	return (firstElement == element);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   bool Vector<_Element_, _Allocator_, _SimdOptimization_>
	::startsWith(const Vector& subVector) const noexcept
{
	const auto otherSize = subVector.size();
	
	if (otherSize > size())
		return;

	for (SizeType i = 0; i < otherSize; ++i) {
		const auto& currentElement		= at(i);
		const auto& otherCurrentElement = subVector[i];

		if (currentElement != otherCurrentElement)
			return false;
	}

	return true;
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   bool Vector<_Element_, _Allocator_, _SimdOptimization_>
	::endsWith(const ValueType& element) const noexcept 
{
	const auto& lastElement = last();
	return (lastElement == element);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   bool Vector<_Element_, _Allocator_, _SimdOptimization_>
	::endsWith(const Vector& subVector) const noexcept 
{
	const auto otherSize = subVector.size();

	if (otherSize > size())
		return;

	for (SizeType i = otherSize - 1; i >= 0; ++i) {
		const auto& currentElement = at(i);
		const auto& otherCurrentElement = subVector[i];

		if (currentElement != otherCurrentElement)
			return false;
	}

	return true;
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::SizeType 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::indexOf(const ValueType& element) const noexcept
{
	return find(element);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::SizeType 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::lastIndexOf(const ValueType& element) const noexcept 
{
	for (SizeType i = size() - 1; i >= 0; --i) {
		auto& currentElement = at(i);

		if (currentElement == element)
			return i;
	}

	return -1;
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::SizeType 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::find(const ValueType& element) const noexcept 
{
	for (SizeType i = 0; i < size(); ++i) {
		auto& currentElement = at(i);

		if (currentElement == element)
			return i;
	}

	return -1;
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
template <typename _Predicate_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::SizeType 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::find_if(_Predicate_ predicate) const noexcept
{
	for (SizeType i = 0; i < size(); ++i) 
		if (predicate(at(i)))
			return i;

	return -1;
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
template <typename _Predicate_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::SizeType 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::findIf(_Predicate_ predicate) const noexcept
{
	return find_if(predicate);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>	
	Vector<_Element_, _Allocator_, _SimdOptimization_>::sliced(
		SizeType positionFrom,
		SizeType elementsCount) const noexcept
{
	const auto _Size = size();

	_VECTOR_DEBUG_ASSERT_LOG_(
		positionFrom > 0 && positionFrom < _Size,
		Vector{}, _VECTOR_OUT_OF_RANGE_);

	_VECTOR_DEBUG_ASSERT_LOG_(
		elementsCount > 0 && (positionFrom + elementsCount) <= _Size,
		Vector{}, _VECTOR_OUT_OF_RANGE_);

	auto& pairValue = _pair._secondValue;

	pointer& _Start = pairValue._start;
	pointer& _Current = pairValue._current;

	const auto _Capacity = static_cast<SizeType>(pairValue._end - _Start);
	pointer _StartWithOffset = _Start + positionFrom;

	Vector newVector(_Capacity);

	base_unused(memory::UninitializedCopyUnchecked(
		_StartWithOffset, _StartWithOffset + elementsCount,
		newVector._pair._secondValue._start));

	return std::move(newVector);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_> 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::sliced(SizeType positionFrom) const noexcept 
{
	const auto _Size = size();

	_VECTOR_DEBUG_ASSERT_LOG_(
		positionFrom > 0 && positionFrom < _Size,
		Vector{}, _VECTOR_OUT_OF_RANGE_);

	auto& pairValue = _pair._secondValue;

	pointer& _Start = pairValue._start;
	pointer& _Current = pairValue._current;

	const auto _Capacity = static_cast<SizeType>(pairValue._end - _Start);
	pointer _StartWithOffset = _Start + positionFrom;

	Vector newVector(_Capacity);

	base_unused(memory::UninitializedCopyUnchecked(
		_StartWithOffset, _Current,
		newVector._pair._secondValue._start));

	return std::move(newVector);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>
	Vector<_Element_, _Allocator_, _SimdOptimization_>::view(
		SizeType positionFrom,
		SizeType elementsCount) const noexcept
{
	const auto _Size = size();

	_VECTOR_DEBUG_ASSERT_LOG_(
		positionFrom > 0 && positionFrom < _Size,
		Vector{}, _VECTOR_OUT_OF_RANGE_);

	_VECTOR_DEBUG_ASSERT_LOG_(
		elementsCount > 0 && (positionFrom + elementsCount) <= _Size,
		Vector{}, _VECTOR_OUT_OF_RANGE_);

	auto& pairValue			= _pair._secondValue;
	auto _StartWithOffset	= pairValue._start + positionFrom;

	return Vector(
		_StartWithOffset, _StartWithOffset /* start as current */,
		_StartWithOffset + elementsCount);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_> 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::view(SizeType positionFrom) const noexcept
{
	const auto _Size = size();

	_VECTOR_DEBUG_ASSERT_LOG_(
		positionFrom > 0 && positionFrom < _Size,
		Vector{}, _VECTOR_OUT_OF_RANGE_);

	auto& pairValue			= _pair._secondValue;
	auto _StartWithOffset	= pairValue._start + positionFrom;

	return Vector(
		_StartWithOffset, _StartWithOffset /* start as current */,
		pairValue._end);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::replace(
	SizeType		 positionFrom,
	const ValueType& oldValue,
	const ValueType& newValue)
{
	const auto _Size = size();

	if (positionFrom > _Size || positionFrom < 0)
		return;

	for (SizeType i = positionFrom; i < _Size; ++i) {
		const auto& currentValue = at(i);

		if (currentValue == oldValue)
			insert(i, newValue);
	}
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::replace(
	const ValueType& oldValue,
	const ValueType& newValue)
{
	for (SizeType i = 0; i < size(); ++i) {
		const auto& currentValue = at(i);

		if (currentValue == oldValue)
			insert(i, newValue);
	}
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::replace(
	SizeType 			positionFrom,
	SizeType 			positionTo,
	const ValueType& 	oldValue,
	const ValueType& 	newValue)
{
	_VECTOR_DEBUG_ASSERT_LOG_(
		positionFrom < size() && positionFrom > 0,
		base_unused(0), _VECTOR_OUT_OF_RANGE_);

	_VECTOR_DEBUG_ASSERT_LOG_(
		positionTo < size() && positionTo > positionFrom,
		base_unused(0), _VECTOR_OUT_OF_RANGE_);

	for (SizeType i = positionFrom; i < positionTo; ++i) {
		const auto& currentValue = at(i);

		if (currentValue == oldValue)
			insert(i, newValue);
	}
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::replace(
	ConstIterator 		positionFrom,
	const ValueType& 	oldValue,
	const ValueType& 	newValue)
{
	auto& pairValue 	= _pair._secondValue;

	pointer& _Start		= pairValue._start;
	pointer& _Current 	= pairValue._current;

	const pointer& fromPointer = positionFrom._currentElement;

	if (fromPointer > _Current || fromPointer < _Start);
		return;

	auto temp = Iterator(fromPointer);

	for (; temp != end(); ++temp)
		if (*temp == oldValue)
			insert(temp, newValue);

}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::replace(
	ConstIterator 		positionFrom,
	ConstIterator 		positionTo,
	const ValueType& 	oldValue,
	const ValueType& 	newValue)
{
	
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::swap(Vector& other) {
	if (this == memory::AddressOf(other))
		return;

	// check allocators
	// ...
	
	_pair.swap(other._pair);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::swapElementsAt(
	SizeType _First,
	SizeType _Last)
{
	auto& firstElement	= at(_First);
	auto& lastElement	= at(_Last);

	if (firstElement == lastElement)
		return;

	std::swap(firstElement, lastElement);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::swapElementsAt(
	Iterator _First,
	Iterator _Last)
{
	auto& firstElement	= *_First;
	auto& lastElement	= *_Last;

	if (firstElement == lastElement)
		return;

	std::swap(firstElement, lastElement);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::remove(
	SizeType _First,
	SizeType _Count)
{
	auto& pairValue = _pair._secondValue;
	auto& allocator = getAllocator();

	const auto _Size = size();
	
	_VECTOR_DEBUG_ASSERT_LOG_(
		_First > 0 && (_First + _Count) <= _Size, 
		base_unused(0), _VECTOR_OUT_OF_RANGE_);

	auto deallocateAt = pairValue._start + _First;

	base_unused(memory::DeallocateRangeCount(
		deallocateAt, 
		_Count, allocator));

	base_unused(memory::UninitializedMove(
		deallocateAt + 1, pairValue._current,
		deallocateAt, allocator));
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::remove(
	ConstIterator _First,
	ConstIterator _Last)
{
	auto& pairValue = _pair._secondValue;
	auto& allocator = _pair.first();

	pointer& _Start 	= pairValue._start;
	pointer& _Current 	= pairValue._current;

	const pointer& firstPointer = _First._currentElement;
	const pointer& lastPointer  = _Last._currentElement;

	_VECTOR_DEBUG_ASSERT_LOG_(firstPointer >= _Start && lastPointer <= _Current, base_unused(0), _VECTOR_OUT_OF_RANGE_);
	_VECTOR_DEBUG_ASSERT_LOG_(firstPointer <= lastPointer, base_unused(0), _VECTOR_OUT_OF_RANGE_);

	memory::DeallocateRange(firstPointer, lastPointer, allocator);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline Vector<_Element_, _Allocator_, _SimdOptimization_>::ValueType& 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::first()
{
	_VECTOR_DEBUG_ASSERT_LOG_(!isEmpty(), "base::container::Vector::first: An attempt to get the first element of an empty Vector");
	return at(0); 
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline const Vector<_Element_, _Allocator_, _SimdOptimization_>::ValueType& 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::first() const noexcept 
{
	_VECTOR_DEBUG_ASSERT_LOG_(!isEmpty(), "base::container::Vector::first: An attempt to get the first element of an empty Vector");
	return at(0);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline const Vector<_Element_, _Allocator_, _SimdOptimization_>::ValueType& 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::constFirst() const noexcept 
{
	_VECTOR_DEBUG_ASSERT_LOG_(!isEmpty(), "base::container::Vector::constFirst: An attempt to get the first element of an empty Vector");
	return at(0);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline Vector<_Element_, _Allocator_, _SimdOptimization_>::ValueType& 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::last()
{
	_VECTOR_DEBUG_ASSERT_LOG_(!isEmpty(), "base::container::Vector::last: An attempt to get the last element of an empty Vector");
	return at(size() - 1);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline const Vector<_Element_, _Allocator_, _SimdOptimization_>::ValueType& 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::last() const noexcept 
{
	_VECTOR_DEBUG_ASSERT_LOG_(!isEmpty(), "base::container::Vector::last: An attempt to get the last element of an empty Vector");
	return at(size() - 1);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline const Vector<_Element_, _Allocator_, _SimdOptimization_>::ValueType& 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::constLast() const noexcept
{
	_VECTOR_DEBUG_ASSERT_LOG_(!isEmpty(), "base::container::Vector::constLast: An attempt to get the last element of an empty Vector");
	return at(size() - 1);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::removeAll(const ValueType& element) {
	for (SizeType i = 0; i < size(); ++i) {
		const auto& currentElement = at(i);

		if (currentElement == element)
			removeAt(i);
	}

}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::removeFirst() {
	removeAt(0);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::removeLast() {
	removeAt(size() - 1);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
template <typename _Predicate_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::removeIf(_Predicate_ pred) {
	for (SizeType i = 0; i < size(); ++i)
		if (pred(at(i)))
			removeAt(i);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   bool Vector<_Element_, _Allocator_, _SimdOptimization_>
	::removeOne(const ValueType& element)
{
	for (SizeType i = 0; i < size(); ++i) {
		const auto& currentElement = at(i);
		if (currentElement == element) {
			removeAt(i);
			return true;
		}
	}

	return false;
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::take(
	const pointer	newVectorStart,
	const SizeType	newVectorSize,
	const SizeType	newVectorCapacity)
{
	auto& allocator 	= _pair.first();
	auto& pairValue 	= _pair._secondValue;

	pointer& _Start 	= pairValue._start;
	pointer& _End 		= pairValue._end;
	pointer& _Current 	= pairValue._current;;

	if (_Start) {
		// destroy and deallocate old array
		const auto _Capacity = static_cast<SizeType>(_End - _Start);

		memory::DestroyRange(_Start, _Current, allocator);
		allocator.deallocate(_Start, _Capacity);
	}

	_Start 		= newVectorStart;
	_Current 	= newVectorStart + newVectorSize;
	_End 		= newVectorStart + newVectorCapacity;
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_> 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::makeView(const std::vector<_Element_>& other)
{
	const auto _Size 	= other.size();
	pointer& otherData 	= other.data();

	_VECTOR_DEBUG_ASSERT_LOG_(
		otherData != nullptr, Vector{}, "base::container::Vector::makeView: "
		"It is not possible to create a view of an vector with zero capacity");

	return Vector(
		otherData, otherData /* start as current */,
		otherData + _Size);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_> Vector<_Element_, _Allocator_, _SimdOptimization_>::makeView(const Vector& other) {
	const auto _Size 	= other.size();
	pointer& otherStart = other._pair._secondValue._start;

	_VECTOR_DEBUG_ASSERT_LOG_(
		otherStart != nullptr, Vector{}, "base::container::Vector::makeView: "
		"It is not possible to create a view of an empty vector (with zero capacity)");

	return Vector(
		otherStart, otherStart /* start as current */,
		otherStart + _Size);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
constexpr inline   sizetype Vector<_Element_, _Allocator_, _SimdOptimization_>::max_size() const noexcept {
	return static_cast<std::size_t>(-1) / sizeof(ValueType);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
constexpr inline   sizetype Vector<_Element_, _Allocator_, _SimdOptimization_>::maxSize() const noexcept {
	return static_cast<std::size_t>(-1) / sizeof(ValueType);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
constexpr inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::SizeType 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::calculateGrowth(SizeType newSize) const noexcept
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

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline   bool Vector<_Element_, _Allocator_, _SimdOptimization_>::resizeReallocate(SizeType newCapacity) noexcept {
	const auto oldCapacity = capacity();

	if (oldCapacity == newCapacity)
		return false;

	auto& allocator		= _pair.first();
	pointer memory		= allocator.allocate(newCapacity);

	if (memory == nullptr)
		_VECTOR_NOT_ENOUGH_MEMORY_DEBUG_(false);

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

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
template <class ..._Valty_>
base_constexpr_cxx20 inline   Vector<_Element_, _Allocator_, _SimdOptimization_>::Reference 
	Vector<_Element_, _Allocator_, _SimdOptimization_>::emplaceBack(_Valty_&&... _Val)
{
	auto& pairValue		= _pair._secondValue;

	if (pairValue._current != pairValue._end) {
		emplaceBackWithUnusedCapacity(std::forward<_Valty_>(_Val)...);
		return back();
	}

	emplaceBackReallocate(std::forward<_Valty_>(_Val)...);
	return back();
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
template <class... _Valty_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::emplaceBackWithUnusedCapacity(_Valty_&&... _Val) 
{
	auto& pairValue = _pair._secondValue;
	auto& allocator = _pair.first();

	emplaceAt(
		allocator, pairValue._current,
		std::forward<_Valty_>(_Val)...);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
template <class... _Valty_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::emplaceBackReallocate(_Valty_&&... _Val) 
{
	auto& pairValue				= _pair._secondValue;
	auto& allocator				= _pair.first();

	const auto _NewCapacity			= static_cast<SizeType>(
		_Vector_Default_Capacity_ + capacity());
	const auto _IsEnoughMemory	= resize(_NewCapacity);

	if (_IsEnoughMemory == false)
		_VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_;

	emplaceAt(
		allocator, pairValue._current,
		std::forward<_Valty_>(_Val)...);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
template <class... _Valty_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::emplaceAt(
	allocator_type& _Allocator,
	pointer&		_Location,
	_Valty_&&...	_Val)
{
	
	if constexpr (IsNoThrowMoveConstructibleArgs<ValueType, allocator_type, _Valty_...>)
		memory::ConstructInPlace(
			*_Location, std::forward<_Valty_>(_Val)...);
	else
		AllocatorTraits::construct(
			_Allocator, memory::UnFancy(_Location),
			std::forward<_Valty_>(_Val)...);

	++_Location;
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
template <typename _Type_>
base_constexpr_cxx20 inline   bool Vector<_Element_, _Allocator_, _SimdOptimization_>::elementsCompare(
	const Vector<_Type_>& other) const noexcept
{
	for (SizeType _Current = 0; _Current < size(); ++_Current) {
		const auto& myCurrentElement 		= at(_Current);
		const auto& otherCurrentElement 	= other.at(_Current);

		if (myCurrentElement != otherCurrentElement)
			return false;
	}

	return true;
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
template <class _Iterator_>
// Insert counted range _First + [0, _Count) at end
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::appendCountedRange(
	_Iterator_		_First,
	const SizeType	_Count) noexcept
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
			_VECTOR_TOO_LONG_DEBUG_NO_RET_;

		const auto _NewCapacity	= calculateGrowth(_OldCapacity + _Count);
		const auto _NewSize		= _OldSize + _Count;

		auto _BlockStart		= allocator.allocate(_NewCapacity);

		if (_BlockStart == nullptr)
			_VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_;

		auto _BlockEnd			= _BlockStart + _NewSize;
		auto _InsertStart		= _BlockStart + _OldSize;

		try {
			memory::UninitializedCopyCount(
				std::move(_First), _Count, 
				_InsertStart, allocator);

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
		}              
		catch (...) {

			memory::DestroyRange(_Current, _Current + _Count, allocator);
			allocator.deallocate(_BlockStart, _NewCapacity);

			throw;
		}

		take(_BlockStart, _NewSize, _NewCapacity);
	}
	else {
		base_unused(memory::UninitializedCopyCount(
			std::move(_First), _Count,
			_Start, allocator));
	}
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
template <
	class _Iterator_,
	class _Sentinel_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::appendUnCountedRange(
	_Iterator_ _First,
	_Sentinel_ _Last) noexcept
{
	auto _Count = SizeType(0);

	for (auto _Temp = _First; _Temp != _Last; ++_Temp, ++_Count) {
	}

	appendCountedRange(std::move(_First), _Count);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
template <class _Iterator_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::prependCountedRange(
	_Iterator_		_First,
	const SizeType	_Count) noexcept
{
	if (_Count <= 0)
		return;

	auto& pairValue 	= _pair._secondValue;
	auto& allocator 	= _pair.first();

	pointer& _Start 	= pairValue._start;
	pointer& _End 		= pairValue._end;
	pointer& _Current 	= pairValue._current;

	const auto _UnusedCapacity 	= static_cast<SizeType>(_End - _Current);
	const auto _Capacity 		= static_cast<SizeType>(_End - _Start);

	if (_Capacity == 0) {
		appendCountedRange(std::move(_First), _Count);
	}
	else if (_UnusedCapacity < _Count) {
		// reallocate 
		const auto _Size = static_cast<SizeType>(_Current - _Start);

		if (_Count > maxSize() - _Size)
			_VECTOR_TOO_LONG_DEBUG_NO_RET_;

		const auto _NewCapacity = calculateGrowth(_Capacity + _Count);
		const auto _NewSize 	= _Size + _Count;

		auto _BlockStart 		= allocator.allocate(_NewCapacity);

		if (_BlockStart == nullptr)
			_VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_;

		auto _BlockEnd 			= _BlockStart + _NewSize;

		try {

			base_unused(memory::UninitializedMove(
				_Start, _Current,
				_Current, allocator));

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

		}
		catch (...) {
			memory::DestroyRange(_Start, _Start + _Count, allocator);
			allocator.deallocate(_BlockStart, _NewCapacity);

			throw;
		}

		take(_BlockStart, _NewSize, _NewCapacity);
	}
	else {
		base_unused(memory::UninitializedMove(
			_Start, _Current,
			_Current, allocator));

		base_unused(memory::UninitializedMove(
			std::move(_First), _Count,
			_Start, allocator));
	}
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
template <
	class _Iterator_,
	class _Sentinel_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::prependUnCountedRange(
	_Iterator_ _First,
	_Sentinel_ _Last) noexcept
{
	auto _Count = SizeType(0);

	for (auto _Temp = _First; _Temp != _Last; ++_Temp)
		++_Count;

	prependCountedRange(std::move(_First), _Count);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
template <class _Iterator_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::insertCountedRange(
	ConstIterator	_To,
	_Iterator_		_First,
	const SizeType	_Count) noexcept
{
	if (_Count <= 0)
		return;

	pointer& pointerTo 		= _To._currentElement;

	auto& pairValue			= _pair._secondValue;
	auto& allocator			= _pair.first();

	pointer& _Start			= pairValue._start;
	pointer& _End			= pairValue._end;
	pointer& _Current		= pairValue._current;

	const auto _OldUnusedCapacity	= static_cast<SizeType>(_End - _Current);
	const auto _OldCapacity		= static_cast<SizeType>(_End - _Start);
	const auto _OldSize			= static_cast<SizeType>(_Current - _Start);

	_VECTOR_DEBUG_ASSERT_LOG_(
		pointerTo >= _Start && (pointerTo + _Count) <= _End,
		base_unused(0), _VECTOR_OUT_OF_RANGE_);
	
	if (_OldUnusedCapacity < _Count) {
		// reallocate 

		if (_Count > maxSize() - _OldSize)
			_VECTOR_TOO_LONG_DEBUG_NO_RET_;

		const auto _NewCapacity			= calculateGrowth(_OldCapacity + _Count);
		const auto _NewSize				= _OldSize + _Count;

		const auto toOffset = static_cast<SizeType>(pointerTo - _Start);

		auto _NewVectorStart = allocator.allocate(_NewCapacity);

		if (_NewVectorStart == nullptr)
			_VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_;

		auto _NewVectorCurrent		= _NewVectorStart + _NewSize;
		auto _NewVectorInsertPlace	= _NewVectorStart + toOffset;

		try {

			const auto _InsertAtEnd = (_NewVectorInsertPlace == _NewVectorCurrent);

			if (_InsertAtEnd) {
				base_unused(memory::UninitializedCopyCount(
					std::move(_First), _Count,
					_NewVectorCurrent, allocator));
			}
			else {
				base_unused(memory::UninitializedCopyCount(
					std::move(_First), _Count,
					_NewVectorInsertPlace, allocator));
			}

			if (_Count == 1) {
				if constexpr (
					std::is_nothrow_move_constructible_v<ValueType>
					|| std::is_copy_constructible_v<ValueType> == false
					) {
					ReallocateMoveExcept(_InsertAtEnd, toOffset,
						_NewVectorStart);
				}
				else {
					ReallocateCopyExcept(_InsertAtEnd, toOffset,
						_NewVectorStart);
				}
			}
			else {
				ReallocateMoveExcept(_InsertAtEnd, toOffset,
					_NewVectorStart);
			}
		}
		catch (...) {
			memory::DestroyRange(_Current, _Current + _Count, allocator);
			allocator.deallocate(_NewVectorStart, _NewCapacity);

			throw;
		}

		take(_NewVectorStart, _NewSize, _NewCapacity);
	}
	else {
		base_unused(memory::UninitializedCopyCount(
			std::move(_First), _Count,
			_Start, allocator));
	}
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
template <
	class _Iterator_,
	class _Sentinel_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::insertUnCountedRange(
	ConstIterator	_To,
	_Iterator_ 		_First,
	_Sentinel_ 		_Last) noexcept
{
	auto& pairValue 	= _pair._secondValue;

	pointer& _Start 	= pairValue._start;
	pointer& _Current 	= pairValue._current;

	pointer& pointerTo 	= _To._currentElement;

	_VECTOR_DEBUG_ASSERT_LOG_(
		pointerTo >= _Start && pointerTo <= _Current,
		base_unused(0), _VECTOR_OUT_OF_RANGE_);

	auto _Count = SizeType(0);

	for (auto _Temp = _First; _Temp != _Last; ++_Temp, ++_Count) {
	}

	insertCountedRange(_To, std::move(_First), _Count);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
template <
	class _Iterator_,
	class _Sentinel_>
// assign unknown number of elements from [_First, _Last)
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::assignUnCountedRange(
	_Iterator_ _First,
	_Sentinel_ _Last)
{
	auto& pairValue    	= _pair._secondValue;

	pointer& _Start 	= pairValue._start;
	pointer& _Current  	= pairValue._current;

	pointer _Next = _Start;

	for (; _First != _Last && _Next != _Current; ++_First, (void) ++_Next)
		*_Next = *_First;

	// Code size optimization: we've exhausted only the source, only the dest, or both.
	// If we've exhausted only the source: we Trim, then Append does nothing.
	// If we've exhausted only the dest: Trim does nothing, then we Append.
	// If we've exhausted both: Trim does nothing, then Append does nothing.

	// Trim.
	memory::DestroyRange(_Next, _Current, getAllocator());
	_Current = _Next;

	appendUnCountedRange(std::move(_First), std::move(_Last));
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
template <class _Iterator_>
// assign elements from counted range _First + [0, _Newsize)
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::assignCountedRange(
	_Iterator_ _First,
	const SizeType _Newsize)
{
	auto& pairValue    	= _pair._secondValue;
	auto& allocator		= _pair.first();

	pointer& _Start 	= pairValue._start;
	pointer& _Current  	= pairValue._current;
	pointer& _End		= pairValue._end;

	const auto _OldCapacity = static_cast<SizeType>(_End - _Start);

	if (_Newsize > _OldCapacity) {
		clearAndReserveGeometricGrowth(_Newsize);

		_Current = memory::UninitializedCopyCount(
			std::move(_First), _Newsize, _Start, allocator);
		
		return;
	}

	const auto _OldSize = static_cast<SizeType>(_Current - _Start);

	if (_Newsize > _OldSize) {
		bool _Copied = false;
		if constexpr (memory::IteratorCopyCategory<_Iterator_, pointer>::BitcopyAssignable) {
#if BASE_HAS_CXX20
			if (!is_constant_evaluated())
#endif
			{
				memory::MemoryCopy(_First, static_cast<size_t>(_OldSize), _Start);
				_First += _OldSize;
				_Copied = true;
			}
		}

		if (!_Copied) {
			for (auto _Mid = _Start; _Mid != _Current; ++_Mid, (void) ++_First) {
				*_Mid = *_First;
			}
		}


		_Current = memory::UninitializedCopyCount(
			std::move(_First), _Newsize - _OldSize,
			_Current, allocator);

	} else {
		const pointer _NewLast = _Start + _Newsize;

		memory::MemoryCopyCountUnchecked(std::move(_First), _Newsize, _Start);
		memory::DestroyRange(_NewLast, _Current, allocator);

		_Current = _NewLast;
	}
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::ReallocateMoveExcept(
	const bool 		insertAtEnd, 
	const SizeType 	toOffset,
	const pointer	newVectorStart)
{
	auto& pairValue			= _pair._secondValue;
	auto& allocator			= getAllocator();

	pointer& _Start			= pairValue._start;
	pointer& _Current		= pairValue._current;

	base_unused(memory::UninitializedMoveCount(
		_Start, toOffset,
		newVectorStart, allocator));

	if (insertAtEnd == false && (_Start + toOffset) <= _Current)
		base_unused(memory::UninitializedMoveCount(
			_Start + toOffset, _Current,
			newVectorStart + toOffset, allocator));
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::ReallocateCopyExcept(
	const bool 		insertAtEnd, 
	const SizeType 	toOffset,
	const pointer	newVectorStart)
{
	auto& pairValue			= _pair._secondValue;
	auto& allocator			= getAllocator();

	pointer& _Start			= pairValue._start;
	pointer& _Current		= pairValue._current;

	base_unused(memory::UninitializedCopyCount(
		_Start, toOffset,
		newVectorStart, allocator));

	if (insertAtEnd == false && (_Start + toOffset) <= _Current)
		base_unused(memory::UninitializedCopyCount(
			_Start + toOffset, _Current,
			newVectorStart + toOffset, allocator));
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::moveAssignUnEqualAllocator(Vector& other) {
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
		clearAndReserveGeometricGrowth(otherSize);

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

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::FreeAllElements() noexcept {
	auto& pairValue		= _pair._secondValue;
	auto& allocator		= _pair.first();

	pointer& _Start		= pairValue._start;
	pointer& _Current  	= pairValue._current;
	pointer& _End		= pairValue._end;

	if (_Start == _End)
		return;

	memory::FreeUsedRange(
		_Start, _Current, 
		_End, allocator);
}

template <
	typename	_Element_,
	class		_Allocator_,
	class		_SimdOptimization_>
base_constexpr_cxx20 inline void Vector<_Element_, _Allocator_, _SimdOptimization_>::clearAndReserveGeometricGrowth(const SizeType newSize) { 
	if (newSize == 0)
		return;

	auto& pairValue 	= _pair._secondValue;
	auto& allocator 	= _pair.first();

	pointer& _Start 	= pairValue._start;
	pointer& _Current  	= pairValue._current;
	pointer& _End		= pairValue._end;

	if (newSize > maxSize())
		return;
	
	const auto newCapacity = calculateGrowth(newSize);
	
	if (_Start != nullptr) { 
		// destroy and deallocate old array
		
		memory::FreeUsedRange(
			_Start, _Current, 
			_End, allocator);
	}
	
	const auto isEnoughMemory = resizeReallocate(newCapacity);

	if (isEnoughMemory == false)
		_VECTOR_NOT_ENOUGH_MEMORY_DEBUG_NO_RET_;
}
 
__BASE_CONTAINER_NAMESPACE_END
