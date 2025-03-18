#pragma once 

#include <base/core/BaseNamespace.h>
#include <base/core/Types.h>

__BASE_CONTAINER_NAMESPACE_BEGIN

template <
	typename _Iterator_>
class STLCompatibleContainerBase {
public:
	using pointer				= typename _Iterator_::pointer;
	using const_pointer			= typename _Iterator_::const_pointer;

	using size_type				= typename _Iterator_::size_type;
	using difference_type		= typename _Iterator_::difference_type;

	using value_type			= typename _Iterator_::value_type;

	using reference				= value_type&;
	using const_reference		= const value_type&;

	using iterator				= _Iterator_;
	using const_iterator		= _ConstIterator_;

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

__BASE_CONTAINER_NAMESPACE_END