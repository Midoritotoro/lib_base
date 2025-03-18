#pragma once

#include <base/core/arch/KeywordSupport.h>
#include <base/core/BaseNamespace.h>

#include <base/core/utility/TypeTraits.h>
#include <base/core/memory/MemoryAllocator.h>

#include <base/core/container/VectorIterator.h> 
#include <base/core/memory/MemoryUtility.h>


__BASE_CONTAINER_NAMESPACE_BEGIN

struct _Vector_Scalar_Algorithm_Tag_	{ };
struct _Vector_SIMD_Algorithm_Tag_		{ };

enum class _Vector_SIMD_Algorithm_Alignment : sizetype { };

static constexpr _Vector_SIMD_Algorithm_Alignment _VectorSIMDAlignment =
	_Vector_SIMD_Algorithm_Alignment
#if LIB_BASE_SIMD_ENABLE
	{ MINIMUM_ACCEPTABLE_SIMD_ALIGNMENT };
#else
	{ 0 }
#endif
	;

template <
	typename _Element_,
	class _Allocator_ = memory::RawMemoryAllocator<_Element_>>
class VectorBase {
public:
	using pointer = typename _Allocator_::pointer;
	using const_pointer = typename _Allocator_::const_pointer;

	using size_type = typename _Allocator_::size_type;
	using difference_type = typename _Allocator_::difference_type;

	using value_type = _Element_;
	using allocator_type = _Allocator_;

	using reference = value_type&;
	using const_reference = const value_type&;

	using iterator = VectorIterator<VectorBase<
		_Element_, _Allocator_>>;

	using const_iterator = VectorConstIterator<VectorBase<
		_Element_, _Allocator_>>;

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

	constexpr inline Reference operator[](const SizeType offset) noexcept {
		return *(_current + offset);
	}

	constexpr inline NODISCARD Reference at(const SizeType index) noexcept {
		const auto isValidOffset = (_current + offset > _start
			&& _current + offset < _end);

		DebugAssertLog(!isValidOffset, "base::container::VectorBase::operator[]: Index out of range. ");
		return (*this)[index];
	}

	constexpr inline NODISCARD ValueType at(const SizeType index) const noexcept {
		return at(index);
	}

	constexpr inline NODISCARD SizeType size() const noexcept {
		return length();
	}

	constexpr inline NODISCARD SizeType length() const noexcept {
		return SizeType(_current - _start);
	}

	constexpr inline NODISCARD SizeType capacity() const noexcept {
		return SizeType(_end - _start);
	}

	constexpr inline NODISCARD bool isEmpty() const noexcept {
		return (length() == 0);
	}

	constexpr inline NODISCARD Pointer data() const noexcept {
		return _start;
	}

	constexpr inline NODISCARD ConstPointer data() const noexcept {
		return _start;
	}

	constexpr inline NODISCARD ConstPointer constData() const noexcept {
		return _start;
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
protected:
	Pointer _start	 = nullptr;
	Pointer _end	 = nullptr;

	Pointer _current = nullptr;
};

template <
	typename	_Element_,
	class		_Allocator_,
	class		_AlgorithmTag_>
class Vector :
	public VectorBase<_Element_, _Allocator_>
{};

template <
	typename	_Element_,
	class		_Allocator_>
class Vector<_Element_, _Allocator_, _Vector_Scalar_Algorithm_Tag_>:
	public VectorBase<_Element_, _Allocator_>
{
public:
	using _MyBase_					= VectorBase<_Element_, _Allocator_>;

	using pointer					= typename _MyBase_::pointer;
	using const_pointer				= typename _MyBase_::const_pointer;

	using size_type					= typename _MyBase_::size_type;
	using difference_type			= typename _MyBase_::difference_type;

	using value_type				= typename _MyBase_::value_type;
	using allocator_type			= typename _MyBase_::allocator_type;

	using reference					= typename _MyBase_::reference;
	using const_reference			= typename _MyBase_::const_reference;

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

    constexpr inline Vector(std::initializer_list<ValueType> elements) noexcept
	{
		const auto _Capacity	= capacity();
		const auto elementsSize = elements.size();

		if (_Capacity < elementsSize) {
			const auto isEnoughMemory = resize(elementsSize);
			if (UNLIKELY(isEnoughMemory == false))
				AssertReturn(false, "base::container::Vector: Not enough memory to expand the Vector\n", UNUSED(0));
		}

		for (SizeType i = 0; i < elementsSize; ++i) {
			const auto cell = SizeType(_start + i);


			
		}
			
    }

    inline Vector(const Vector<ValueType>& other) {

    }

    inline Vector(const std::vector<ValueType>& vector) {

    }

    inline Vector(const SizeType capacity) {

    }

    inline Vector(
        const SizeType _Capacity,
        const ValueType& fill)
    {

    }
    
    inline Vector(Vector<ValueType>&& rOther) noexcept {

    }

    inline void push_back(const ValueType& element) {

    }

    inline void push_front(const ValueType& element) {

    }

    inline void append(const ValueType& element) {

    }
   	
	inline NODISCARD ValueType pop() noexcept {
		
	}

	inline NODISCARD SizeType find(ConstReference element) const noexcept {
		return _Algorithm_::find(element);
	}

	inline NODISCARD Vector<size_type> 
		findAll(ConstReference element) const noexcept
	{
		return _Algorithm_::findAll(element);
	}
	
	inline NODISCARD size_type
		findLastOf(ConstReference element) const noexcept
	{
		return _Algorithm_::findLastOf(element);
	}

	inline NODISCARD size_type 
		findFirstOf(const_reference element) const noexcept
	{
		return _Algorithm_::findFirstOf(element);
	}

	inline NODISCARD size_type
		findLastNotOf(const_reference element) const noexcept
	{
		return _Algorithm_::findLastNotOf(element);
	}
	
	inline NODISCARD size_type 
		findFirstNotOf(const_reference element) const noexcept
	{
		return _Algorithm_::findFirstNotOf(element);
	}

	inline void clear() {
		UNUSED(erase(constBegin(), constEnd()));
	}

	inline NODISCARD Iterator erase(
		ConstIterator first,
		ConstIterator last)
	{

	}

	inline NODISCARD Iterator erase(ConstIterator it) {

	}

	inline NODISCARD bool resize(
		size_type size,
		const_reference _Fill)
	{
		const auto resizeSuccess = resize(size);
		const auto fillSuccess   = fill(_Fill);

		const auto success		 = (resizeSuccess == true && fillSuccess == true);
		return success;
	}

    inline NODISCARD size_type indexOf(
		const_reference element,
		size_type from = 0) const noexcept
	{
		return _Algorithm_::indexOf(element, from);
	}

	inline NODISCARD size_type lastIndexOf(
		const_reference element,
		size_type from = 0) const noexcept
	{
		return _Algorithm_::lastIndexOf(element, from);
	}

	inline NODISCARD bool contains(
		const_reference element,
		size_type from = 0) const noexcept
	{
		return _Algorithm_::contains(element, from);
	}

	inline NODISCARD bool contains(
		const Vector<value_type> subVector,
		size_type from = 0) const noexcept
	{
		return _Algorithm_::contains(subVector, from);
	}
	
	inline NODISCARD size_type count(
		const_reference element) const noexcept
	{
		return _Algorithm_::count(element);
	}
	
	inline NODISCARD size_type count(
		const Vector<value_type>& subVector) const noexcept
	{
		return _Algorithm_::count(subVector);
	}
	
	inline void fill(const_reference fill) noexcept {
		return _Algorithm_::fill(subVector);
	}

	inline NODISCARD bool resize(const SizeType _Capacity) {
		const auto bytesRequired = _Capacity * sizeof(ValueType);
		if (UNLIKELY(bytesRequired <= 0))
			return false;

		_Allocator_ allocator = {};

		const auto memory = allocator.Allocate(bytesRequired);

		if (UNLIKELY(memory == nullptr))
			return false;
	}

	inline void insert(
		size_type index,
		const_reference element) noexcept
	{

	}

	inline void prepend(const_reference element) noexcept {

	}
};

template <
	typename _Element_,
	class _Allocator_>
class Vector<_Element_, _Allocator_, _Vector_SIMD_Algorithm_Tag_> :
	public VectorBase<_Element_, _Allocator_>
{};

//
//template <class _Allocator_>
//class Vector<bool, _Allocator_> {
//public:
//	using value_type = bool;
//	using allocator_type = _Allocator_;
//
//	using reference = value_type&;
//	using const_reference = const value_type&;
//
//	using iterator = VectorIterator<value_type>;
//	using const_iterator = VectorConstIterator<value_type>;
//
//	using reverse_iterator = std::reverse_iterator<iterator>;
//	using const_reverse_iterator = std::reverse_iterator<const_iterator>;
//
//	using pointer = typename _Allocator_::pointer;
//	using const_pointer = typename _Allocator_::const_pointer;
//
//	using size_type = typename _Allocator_::size_type;
//	using difference_type = typename _Allocator_::difference_type;
//
//	using Iterator = iterator;
//	using ConstIterator = const_iterator;
//
//	using ValueType = value_type;
//
//	using ReverseIterator = reverse_iterator;
//	using ConstReverseIterator = const_reverse_iterator;
//
//private:
//	ValueType* _start	= nullptr;
//	ValueType* _end		= nullptr;
//
//	SizeType _length	= 0;
//};

__BASE_CONTAINER_NAMESPACE_END