#pragma once

#include <base/core/arch/KeywordSupport.h>
#include <base/core/BaseNamespace.h>

#include <base/core/utility/TypeTraits.h>
#include <base/core/memory/MemoryAllocator.h>

#include <base/core/container/VectorIterator.h> 
#include <base/core/memory/MemoryUtility.h>


__BASE_CONTAINER_NAMESPACE_BEGIN

template <
	typename _Element_,
	class _Allocator_ = memory::RawMemoryAllocator<_Element_>>
class Vector {
public:
	using pointer					= typename _Allocator_::pointer;
    using const_pointer				= typename _Allocator_::const_pointer;

    using size_type					= typename _Allocator_::size_type;
    using difference_type			= typename _Allocator_::difference_type;

    using value_type				= _Element_;
    using allocator_type			= _Allocator_;

    using reference					= value_type&;
    using const_reference			= const value_type&;

    using iterator					= VectorIterator<value_type>;
    using const_iterator			= VectorConstIterator<value_type>;

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
	

    constexpr Vector() noexcept {};
    constexpr ~Vector() noexcept = default;

    constexpr inline Vector(std::initializer_list<ValueType> elements) noexcept
	{
		const auto _Capacity	= capacity();
		const auto elementsSize = elements.size();

		if (_Capacity < elementsSize) {
			const auto isEnoughMemory = resize(elementsSize);
			if (UNLIKELY(isEnoughMemory == false))
				AssertReturn(false, "base::Not enough memory to expand the Vector\n", UNUSED(0));
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
    
	constexpr inline NODISCARD SizeType size() const noexcept {
		return length();
	}

	constexpr inline NODISCARD SizeType length() const noexcept {
		return SizeType(_current - _start);
	}

	inline NODISCARD SizeType capacity() const noexcept {
		return SizeType(_end - _start);
	}
	
	inline NODISCARD ValueType pop() noexcept {

	}

	inline NODISCARD SizeType find(ConstReference element) const noexcept {

	}

	inline NODISCARD Vector<size_type> 
		findAll(ConstReference element) const noexcept
	{

	}
	
	inline NODISCARD size_type
		findLastOf(ConstReference element) const noexcept
	{

	}

	inline NODISCARD size_type 
		findFirstOf(const_reference element) const noexcept
	{

	}

	inline NODISCARD size_type
		findLastNotOf(const_reference element) const noexcept
	{

	}
	
	inline NODISCARD size_type 
		findFirstNotOf(const_reference element) const noexcept
	{

	}

	inline NODISCARD bool isEmpty() const noexcept {

	}

	inline NODISCARD Pointer data() const noexcept {
		return 
	}

	inline NODISCARD const Char* data() const noexcept {
		return
	}

	inline NODISCARD const Char* constData() const noexcept {

	}

	inline CONSTEXPR_CXX20 Iterator begin() noexcept {

	}

	inline CONSTEXPR_CXX20 ConstIterator begin() const noexcept {

	}

	inline CONSTEXPR_CXX20 ConstIterator cbegin() const noexcept {

	}

	inline CONSTEXPR_CXX20 ConstIterator constBegin() const noexcept {

	}

	inline CONSTEXPR_CXX20 Iterator end() noexcept {

	}

	inline CONSTEXPR_CXX20 ConstIterator end() const noexcept {

	}

	inline CONSTEXPR_CXX20 ConstIterator cend() const {

	}

	inline CONSTEXPR_CXX20 ConstIterator constEnd() const noexcept {

	}

	inline CONSTEXPR_CXX20 ReverseIterator rbegin() noexcept {

	}

	inline CONSTEXPR_CXX20 ReverseIterator rend() noexcept {

	}

	inline CONSTEXPR_CXX20 ConstReverseIterator rbegin() const noexcept {

	}

	inline CONSTEXPR_CXX20 ConstReverseIterator rend() const noexcept {

	}

	inline CONSTEXPR_CXX20 ConstReverseIterator crbegin() const noexcept {

	}

	inline CONSTEXPR_CXX20 ConstReverseIterator crend() const noexcept {

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

	inline NODISCARD value_type front() const {
		return at(0);
	}

	inline NODISCARD reference front() {
		return at(0);
	}

	inline NODISCARD value_type back() const {
		return at(size() - 1);
	}

	inline NODISCARD reference back() {
		return at(size() - 1);
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

	}

	inline NODISCARD bool contains(
		const Vector<value_type> subVector,
		size_type from = 0) const noexcept
	{

	}
	
	inline NODISCARD size_type count(
		const_reference element) const noexcept
	{

	}
	
	inline NODISCARD size_type count(
		const Vector<value_type>& subVector) const noexcept
	{

	}
	
	inline void fill(const_reference fill) noexcept {
		
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
private:
	Pointer _start		= nullptr;
	Pointer _end		= nullptr;

	Pointer _current	= nullptr;
};

template <class _Allocator_>
class Vector<bool, _Allocator_> {
public:
	using value_type = bool;
	using allocator_type = _Allocator_;

	using reference = value_type&;
	using const_reference = const value_type&;

	using iterator = VectorIterator<value_type>;
	using const_iterator = VectorConstIterator<value_type>;

	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	using pointer = typename _Allocator_::pointer;
	using const_pointer = typename _Allocator_::const_pointer;

	using size_type = typename _Allocator_::size_type;
	using difference_type = typename _Allocator_::difference_type;

	using Iterator = iterator;
	using ConstIterator = const_iterator;

	using ValueType = value_type;

	using ReverseIterator = reverse_iterator;
	using ConstReverseIterator = const_reverse_iterator;

private:
	ValueType* _start	= nullptr;
	ValueType* _end		= nullptr;
	SizeType _length	= 0;
};

__BASE_CONTAINER_NAMESPACE_END