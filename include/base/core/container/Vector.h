#pragma once


#include <base/core/memory/MemoryUtility.h>
#include <base/core/container/VectorIterator.h>

#include <base/core/container/CompressedPair.h>

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
		swap(_start, other._start);
		swap(_end, other._end);
		swap(_current, other._current);
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
	class		_Allocator_ = memory::DefaultMemoryAllocator<_Element_>>
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
	static constexpr _Vector_SIMD_Algorithm_Alignment _VectorSIMDAlignment =
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
			if (UNLIKELY(isEnoughMemory == false)) {
				DebugAssertLog(false, "base::container::Vector: Not enough memory to expand the Vector.\n ");
				return;
			}
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

		if (memory::MemoryCopy(_pair._secondValue._start, _Start, _Capacity) == false) {
			DebugAssertLog(false, "base::container::Vector::Vector: ������ ��� ����������� ���������. ");
			return;
		}
	}

	CONSTEXPR_CXX20 inline Vector(const std::vector<ValueType>& vector) noexcept :
		_pair(_Zero_then_variadic_args_{}, VectorValueType())
	{
		const auto _Capacity = vector.capacity();
		auto& _Start = _pair.second()._start;

		const auto isEnoughMemory = resize(_Capacity);

		if (UNLIKELY(isEnoughMemory == false)) {
			DebugAssertLog(false, "base::container::Vector: Not enough memory to expand the Vector.\n ");
			return;
		}

		if (memory::MemoryCopy(_Start, vector.data(), _Capacity) == false)
			DebugAssertLog(false, "base::container::Vector::Vector: ������ ��� ����������� ���������. ");
	}

	CONSTEXPR_CXX20 inline Vector(const SizeType _Capacity) noexcept :
		_pair(_Zero_then_variadic_args_{}, VectorValueType())
	{
		const auto isEnoughMemory = resize(_Capacity);

		if (UNLIKELY(isEnoughMemory == false))
			DebugAssertLog(false, "base::container::Vector: Not enough memory to expand the Vector.\n ");
	}

	CONSTEXPR_CXX20 inline Vector(
		const SizeType _Capacity,
		const ValueType& _Fill
	) noexcept :
		_pair(_Zero_then_variadic_args_{}, VectorValueType())
	{
		const auto isEnoughMemory = resize(_Capacity);

		if (UNLIKELY(isEnoughMemory == false)) {
			DebugAssertLog(false, "base::container::Vector: Not enough memory to expand the Vector.\n ");
			return;
		}

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
		auto& pairValue	= _pair._secondValue;

		auto& _End		= pairValue._end;
		auto& _Current	= pairValue._current;

		if (_Current != _End) {
			EmplaceBackWithUnusedCapacity()
			return;
		}


    }

	CONSTEXPR_CXX20 inline void push_front(const ValueType& element) {

    }

	CONSTEXPR_CXX20 inline void append(const ValueType& element) {
		return push_back(element);
    }

	template <class ... Args> 
	CONSTEXPR_CXX20 inline void emplace_back(Args&&... args) noexcept {
		
	}
   	
	CONSTEXPR_CXX20 inline NODISCARD ValueType pop() noexcept {
		const auto value = back();
		removeAt(size() - 1);

		return value;
	}

	inline void removeAt(BASE_GUARDOVERFLOW const size_type index) noexcept {

	}

	inline NODISCARD SizeType find(ConstReference element) const noexcept {
		for (SizeType i = 0; i < size(); ++i)
			if (at(i) == element)
				return i;

		return -1;
	}

	inline NODISCARD Vector<SizeType>
		findAll(ConstReference element) const noexcept
	{
		Vector<SizeType> result = { -1 };

		for (SizeType i = 0; i < size(); ++i)
			if (at(i) == element)
				result.push_back(i);

		return result;
	}
	
	inline NODISCARD SizeType
		findLastOf(ConstReference element) const noexcept
	{
		for (SizeType i = size() - 1; i >= 0; --i)
			if (at(i) == element)
				return i;

		return -1;
	}

	inline NODISCARD SizeType
		findFirstOf(ConstReference element) const noexcept
	{
		return find(element);
	}

	inline NODISCARD SizeType
		findLastNotOf(ConstReference element) const noexcept
	{
		for (SizeType i = size() - 1; i >= 0; --i)
			if (at(i) != element)
				return i;

		return -1;
	}
	
	inline NODISCARD SizeType
		findFirstNotOf(ConstReference element) const noexcept
	{
		for (SizeType i = 0; i < size(); ++i)
			if (at(i) != element)
				return i;

		return -1;
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
		const auto resizeSuccess = resize(size);

		if (resizeSuccess)
			fill(_Fill);

		return resizeSuccess;
	}

    inline NODISCARD SizeType indexOf(
		ConstReference element,
		SizeType from = 0) const noexcept
	{
		for (SizeType i = from; i < size(); ++i)
			if (at(i) == element)
				return i;

		return -1;
	}

	inline NODISCARD SizeType lastIndexOf(
		ConstReference element,
		SizeType from = 0) const noexcept
	{
		for (SizeType i = size() - 1; i >= from; --i)
			if (at(i) == element)
				return i;

		return -1;
	}

	inline NODISCARD bool contains(
		ConstReference element,
		SizeType from = 0) const noexcept
	{
		for (SizeType i = from; i < size(); ++i)
			if (at(i) == element)
				return true;

		return false;
	}

	inline NODISCARD bool contains(
		const Vector& subVector,
		SizeType from = 0) const noexcept
	{
		SizeType overlaps = 0;
		const auto subVectorSize = subVector.size();

		for (SizeType i = from; i < size(); ++i)
			if (at(i) == subVector[i % subVectorSize] && (++overlaps == subVectorSize))
				return true;

		return false;
	}
	
	inline NODISCARD SizeType count(
		ConstReference element) const noexcept
	{
		SizeType _Count = 0;

		for (SizeType i = 0; i < size(); ++i)
			if (at(i) == element)
				++_Count;

		return _Count;
	}
	
	inline NODISCARD size_type count(
		const Vector& subVector) const noexcept
	{
		size_type _Count = 0;
		size_type overlaps = 0;

		const auto subVectorSize = subVector.size();

		for (size_type i = 0; i < size(); ++i)
			(at(i) == subVector[i % subVectorSize]
				&& (++overlaps == subVectorSize))
					? ++_Count
					: overlaps = 0;
		
		return _Count;
	}
	
	inline void fill(const_reference _Fill) noexcept {
		for (SizeType i = 0; i < capacity(); ++i)
			insert(i, _Fill);
	}

	inline NODISCARD bool resize(const SizeType newCapacity) {
		const auto growth			= calculateGrowth(newCapacity);
		const auto isEnoughMemory	= resizeReallocate(growth);

		return isEnoughMemory;
	}

	inline void insert(
		size_type index,
		const_reference element) noexcept
	{

	}

	inline void prepend(const_reference element) noexcept {

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
		const auto bytesRequired = static_cast<SizeType>(newCapacity * sizeof(ValueType));

		if (UNLIKELY(bytesRequired <= 0))
			return false;

		auto& allocator		= _pair.first();
		auto memory			= allocator.allocate(bytesRequired);

		if (UNLIKELY(memory == nullptr))
			return false;

		const auto oldSize	= size();
		auto& pairValue		= _pair._secondValue;

		auto& blockStart	= memory;
		auto  blockEnd		= memory + newCapacity;

		if (LIKELY(oldSize != 0))
			memory::MemoryCopyCommon(
				begin(), end(),
				blockStart, blockEnd);

		pairValue._start	= blockStart;
		pairValue._end		= blockEnd;
			
		pairValue._current	= blockStart + oldSize;
		
		return true;
	}

	template <class... _Valty_>
	CONSTEXPR_CXX20 inline void EmplaceBackWithUnusedCapacity(_Valty_&&... _Val) {
		auto& pairValue		= _pair._secondValue;
		auto& allocator		= _pair.first();

		pointer& _End		= pairValue._end;
		pointer& _Current	= pairValue._current;

		DebugAssert(_Current != _End);

		if constexpr (std::conjunction_v<std::is_nothrow_constructible<ValueType, _Valty_...>,
			std::_Uses_default_construct<allocator_type, ValueType*, _Valty_...>>)
			memory::ConstructInPlace(*_Current, std::forward<_Valty_>(_Val)...);
		else
			_Alty_traits::construct(allocator, memory::UnFancy(_Current), _STD forward<_Valty>(_Val)...);
		

		++_Current;
	}

	inline void FreeAllElements() noexcept {
		auto& pairValue	= _pair._secondValue;
		auto& allocator = _pair.first();

		memory::FreeRange(pairValue._start, pairValue._end, allocator);

		pairValue._current = nullptr;
	}

	inline void insertToAdress(
		sizetype offset, 
		ValueType* value)
	{
			
	}

	CompressedPair<allocator_type, VectorValueType> _pair;
};

__BASE_CONTAINER_NAMESPACE_END
