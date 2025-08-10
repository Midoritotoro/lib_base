#pragma once 

#include <base/core/utility/TypeTraits.h>

__BASE_CONTAINER_NAMESPACE_BEGIN

template <class _Vector> 
class VectorConstIterator {
public:
	using iterator_category = std::random_access_iterator_tag;

	using value_type		= typename _Vector::value_type;
	using difference_type	= sizetype;

	using pointer			= const typename _Vector::pointer;
	using const_pointer		= pointer;

	using size_type			= sizetype;

	using reference			= const typename _Vector::reference;
	using const_reference	= reference;

	inline constexpr  _constructor VectorConstIterator() noexcept = default;

	inline base_constexpr_cxx20  _constructor VectorConstIterator(const _Vector* vector) :
		_currentElement(vector->data())
	{}

	inline base_constexpr_cxx20  _constructor VectorConstIterator(const VectorConstIterator& other) :
		_currentElement(other._currentElement)
	{}

	inline base_constexpr_cxx20 ~VectorConstIterator() {

	}

	base_constexpr_cxx20 const_reference operator*() const noexcept {
		return *_currentElement;
	}

	base_constexpr_cxx20 const_pointer operator->() const noexcept {
		return _currentElement;
	}

	base_constexpr_cxx20 VectorConstIterator& operator--() noexcept {
		--_currentElement;
		return *this;
	}

	base_constexpr_cxx20 VectorConstIterator operator--(int) noexcept {
		VectorConstIterator temp = *this;
		--*this;

		return temp;
	}

	base_constexpr_cxx20 VectorConstIterator& operator++() noexcept {
		++_currentElement;
		return *this;
	}

	base_constexpr_cxx20 VectorConstIterator operator++(int) noexcept {
		VectorConstIterator temp = *this;
		++*this;

		return temp;
	}

	base_constexpr_cxx20 VectorConstIterator& operator+=(const size_type _Off) noexcept {
		_currentElement += _Off;
		return *this;
	}

	base_constexpr_cxx20   VectorConstIterator
		operator+(const size_type offset) const noexcept
	{
		VectorConstIterator temp = *this;
		temp += offset;

		return temp;
	}

	friend base_constexpr_cxx20  
		VectorConstIterator operator+(
			VectorConstIterator next,
			const size_type offset) noexcept
	{
		next += offset;
		return next;
	}

	base_constexpr_cxx20   VectorConstIterator& operator-=(const size_type offset) noexcept {
		return *this += -offset;
	}

	base_constexpr_cxx20   VectorConstIterator operator-(const size_type offset) const noexcept {
		VectorConstIterator temp = *this;
		temp -= offset;

		return temp;
	}

	base_constexpr_cxx20   size_type operator-(const VectorConstIterator& _Right) const noexcept {
		return static_cast<size_type>(_currentElement - _Right._currentElement);
	}

	base_constexpr_cxx20   reference operator[](const size_type _Off) const noexcept {
		return *(*this + _Off);
	}

#if BASE_HAS_CXX20
	base_constexpr_cxx20   auto /*std::strong_ordering*/
		operator<=>(const VectorConstIterator& other) const noexcept
	{
		return (_currentElement <=> other._currentElement);
	}
#endif

	base_constexpr_cxx20 VectorConstIterator& operator=(const VectorConstIterator& other) const noexcept {
		_currentElement = other._currentElement;
	}

	base_constexpr_cxx20 bool operator==(const VectorConstIterator& other) const noexcept {
		return (_currentElement == other._currentElement);
	}

	base_constexpr_cxx20 bool operator!=(const VectorConstIterator& other) const noexcept {
		return !(*this == other);
	}

	base_constexpr_cxx20 bool operator<(const VectorConstIterator& other) const noexcept {
		return (_currentElement < other._currentElement);
	}

	base_constexpr_cxx20 bool operator>(const VectorConstIterator& other) const noexcept {
		return !(*this < other);
	}

	base_constexpr_cxx20 bool operator<=(const VectorConstIterator& other) const noexcept {
		return (_currentElement <= other._currentElement);
	}

	base_constexpr_cxx20 bool operator>=(const VectorConstIterator& other) const noexcept {
		return !(*this <= other);
	}
private:
	value_type* _currentElement	= nullptr;
	friend typename _Vector;
};

__BASE_CONTAINER_NAMESPACE_END
