#pragma once 

#include <base/core/memory/MemoryAllocator.h>

__BASE_CONTAINER_NAMESPACE_BEGIN

template <class _Vector> 
class VectorConstIterator {
public:
	using iterator_category = std::random_access_iterator_tag;

	using value_type		= typename _Vector::value_type;
	using difference_type	= sizetype;

	using pointer			= const value_type*;
	using const_pointer		= pointer;

	using size_type			= sizetype;

	using reference			= value_type&;
	using const_reference	= reference;

	inline constexpr NODISCARD_CTOR VectorConstIterator() noexcept = default;

	inline CONSTEXPR_CXX20 NODISCARD_CTOR VectorConstIterator(const _Vector* vector) :
		_container(vector),
		_currentElement(vector->data())
	{}

	inline CONSTEXPR_CXX20 NODISCARD_CTOR VectorConstIterator(const VectorConstIterator& other) :
		_container(other._container),
		_currentElement(other._currentElement)
	{}

	inline CONSTEXPR_CXX20 ~VectorConstIterator() {

	}

	CONSTEXPR_CXX20 const_reference operator*() const noexcept {
		DebugAssertLog(_container != nullptr, "base::container::VectorConstIterator::operator*: Попытка доступа к элементу удаленного вектора. ");
		return *_currentElement;
	}

	CONSTEXPR_CXX20 const_pointer operator->() const noexcept {
		DebugAssertLog(_container != nullptr, "base::container::VectorConstIterator::operator*: Попытка доступа к элементу удаленного вектора. ");
		return _currentElement;
	}

	CONSTEXPR_CXX20 VectorConstIterator& operator--() noexcept {
		DebugAssertLog(_currentElement, "cannot decrement value-initialized string iterator");

		DebugAssertLog(_container, "cannot decrement string iterator because the iterator was "
			"invalidated (e.g. reallocation occurred, or the string was destroyed)");

		DebugAssertLog(_container->constData() < _currentElement, "cannot decrement string iterator before begin");

		--_currentElement;
		return *this;
	}

	CONSTEXPR_CXX20 VectorConstIterator operator--(int) noexcept {
		VectorConstIterator temp = *this;
		--*this;

		return temp;
	}

	CONSTEXPR_CXX20 VectorConstIterator& operator++() noexcept {
		DebugAssertLog(_currentElement, "cannot decrement value-initialized string iterator");

		DebugAssertLog(_container, "cannot decrement string iterator because the iterator was "
			"invalidated (e.g. reallocation occurred, or the string was destroyed)");

		DebugAssertLog(_container->constData() < _currentElement, "cannot decrement string iterator before begin");

		++_currentElement;
		return *this;
	}

	CONSTEXPR_CXX20 VectorConstIterator operator++(int) noexcept {
		VectorConstIterator temp = *this;
		++*this;

		return temp;
	}

	CONSTEXPR_CXX20 VectorConstIterator& operator+=(const size_type _Off) noexcept {
		DebugAssertLog(checkOffsetExclusive(_Off), "base::string::StringIterator::operator+=(sizetype): Индекс вне диапазона. ");

		_currentElement += _Off;
		return *this;
	}

	CONSTEXPR_CXX20 NODISCARD VectorConstIterator
		operator+(const size_type offset) const noexcept
	{
		VectorConstIterator temp = *this;
		temp += offset;

		return temp;
	}

	friend CONSTEXPR_CXX20 NODISCARD
		VectorConstIterator operator+(
			const size_type offset,
			VectorConstIterator next) noexcept
	{
		next += offset;
		return next;
	}

	CONSTEXPR_CXX20 NODISCARD VectorConstIterator& operator-=(const size_type offset) noexcept {
		return *this += -offset;
	}

	CONSTEXPR_CXX20 NODISCARD VectorConstIterator operator-(const size_type offset) const noexcept {
		VectorConstIterator temp = *this;
		temp -= offset;

		return temp;
	}

	CONSTEXPR_CXX20 NODISCARD size_type operator-(const VectorConstIterator& _Right) const noexcept {
		compareStrings(_Right);
		return static_cast<size_type>(_currentElement - _Right._currentElement);
	}

	CONSTEXPR_CXX20 NODISCARD reference operator[](const size_type _Off) const noexcept {
		return *(*this + _Off);
	}

#if BASE_HAS_CXX20
	CONSTEXPR_CXX20 NODISCARD auto /*std::strong_ordering*/
		operator<=>(const VectorConstIterator& other) const noexcept
	{
		compareVectors(other);
		return (_currentElement <=> other._currentElement);
	}
#else // !BASE_HAS_CXX20
	CONSTEXPR_CXX20 VectorConstIterator& operator=(const VectorConstIterator& other) const noexcept {
		_container = other._container;
		_currentElement = other._currentElement;
	}

	CONSTEXPR_CXX20 bool operator==(const VectorConstIterator& other) const noexcept {
		compareVectors(other);
		return (_currentElement == other._currentElement);
	}

	CONSTEXPR_CXX20 bool operator!=(const VectorConstIterator& other) const noexcept {
		return !(*this == other);
	}

	CONSTEXPR_CXX20 bool operator<(const VectorConstIterator& other) const noexcept {
		compareVectors(other);
		return (_currentElement < other._currentElement);
	}

	CONSTEXPR_CXX20 bool operator>(const VectorConstIterator& other) const noexcept {
		return !(*this < other)
	}

	CONSTEXPR_CXX20 bool operator<=(const VectorConstIterator& other) const noexcept {
		compareVectors(other);
		return (_currentElement <= other._currentElement);
	}

	CONSTEXPR_CXX20 bool operator>=(const VectorConstIterator& other) const noexcept {
		return !(*this <= other);
	}
#endif
private:
	inline CONSTEXPR_CXX20 void
		compareVectors(const VectorConstIterator& other) const noexcept
	{
		DebugAssertLog(_container == other._container,
			"base::string::StringIterator::compareStrings: Невозможно сравнить итераторы различных объектов String. ");
	}

	// false if out of range
	inline CONSTEXPR_CXX20 NODISCARD
		bool checkOffset(size_type offset) const noexcept
	{
		const auto containerDataStart	= _container->data();
		const auto containerDataEnd		= containerDataStart + _container->size();

		return (_currentElement + offset > containerDataStart
			&& _currentElement + offset < containerDataEnd);
	}

	// false if out of range
	inline CONSTEXPR_CXX20 NODISCARD
		bool checkOffsetExclusive(size_type offset) const noexcept
	{
		const auto containerDataStart	= _container->data();
		const auto containerDataEnd		= containerDataStart + _container->size();

		return (_currentElement + offset >= containerDataStart
			&& _currentElement + offset <= containerDataEnd);
	}

	const _Vector* _container		= nullptr;
	const_pointer _currentElement	= nullptr;
};

__BASE_CONTAINER_NAMESPACE_END
