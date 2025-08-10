#pragma once 

#include <base/core/string/StringConfig.h>
#include <xmemory>


__BASE_STRING_NAMESPACE_BEGIN

template <class _String>
class StringConstIterator {
public:
	using iterator_category	= std::random_access_iterator_tag;

	using value_type		= typename _String::value_type;
	using difference_type	= typename _String::difference_type;

	using pointer			= typename _String::const_pointer;
	using size_type			= typename _String::size_type;

	using reference			= const value_type&;

	inline base_constexpr_cxx20  _constructor StringConstIterator(const _String* str) :
		_container(str),
		_currentChar(str->data())
	{}

	inline base_constexpr_cxx20  _constructor StringConstIterator(const StringConstIterator& other) :
		_container(other._container),
		_currentChar(other._currentChar)
	{}

	inline base_constexpr_cxx20 ~StringConstIterator() {

	}

	base_constexpr_cxx20 Char& operator*() const noexcept {
		DebugAssertLog(_container != nullptr, "base::string::StringIterator::operator*: Попытка доступа к элементу удаленной строки. ");
		return *_currentChar;
	}

	base_constexpr_cxx20 StringConstIterator& operator--() noexcept {
		DebugAssertLog(_currentChar, "cannot decrement value-initialized string iterator");

		DebugAssertLog(_container, "cannot decrement string iterator because the iterator was "
			"invalidated (e.g. reallocation occurred, or the string was destroyed)");

		DebugAssertLog(_container->constData() < _currentChar, "cannot decrement string iterator before begin");

		--_currentChar;
		return *this;
	}

	base_constexpr_cxx20 StringConstIterator operator--(int) noexcept {
		StringConstIterator temp = *this;
		--*this;

		return temp;
	}

	base_constexpr_cxx20 StringConstIterator& operator++() noexcept {
		DebugAssertLog(_currentChar, "cannot decrement value-initialized string iterator");

		DebugAssertLog(_container, "cannot decrement string iterator because the iterator was "
			"invalidated (e.g. reallocation occurred, or the string was destroyed)");

		DebugAssertLog(_container->constData() < _currentChar, "cannot decrement string iterator before begin");

		++_currentChar;
		return *this;
	}

	base_constexpr_cxx20 StringConstIterator operator++(int) noexcept {
		StringConstIterator temp = *this;
		++*this;

		return temp;
	}

	base_constexpr_cxx20 StringConstIterator& operator+=(const size_type _Off) noexcept {
		DebugAssertLog(checkOffset(_Off), "base::string::StringIterator::operator+=(sizetype): Индекс вне диапазона. ");

		_currentChar += _Off;
		return *this;
	}

	base_constexpr_cxx20   StringConstIterator
		operator+(const size_type offset) const noexcept
	{
		StringConstIterator temp = *this;
		temp += offset;

		return temp;
	}

	friend base_constexpr_cxx20  
		StringConstIterator operator+(
			const size_type offset,
			StringConstIterator next) noexcept
	{
		next += offset;
		return next;
	}

	base_constexpr_cxx20   StringConstIterator& operator-=(const size_type offset) noexcept {
		return *this += -offset;
	}

	base_constexpr_cxx20   StringConstIterator operator-(const size_type offset) const noexcept {
		StringConstIterator temp = *this;
		temp -= offset;

		return temp;
	}

	base_constexpr_cxx20   size_type operator-(const StringConstIterator& _Right) const noexcept {
		compareStrings(_Right);
		return static_cast<size_type>(_currentChar - _Right._currentChar);
	}

	base_constexpr_cxx20   reference operator[](const size_type _Off) const noexcept {
		return *(*this + _Off);
	}

#if BASE_HAS_CXX20
	base_constexpr_cxx20   auto /*std::strong_ordering*/
		operator<=>(const StringConstIterator& other) const noexcept
	{
		compareStrings(other);
		return (_currentChar <=> other._currentChar);
	}
#else // !BASE_HAS_CXX20
	base_constexpr_cxx20 StringConstIterator& operator=(const StringConstIterator& other) const noexcept {
		_container = other._container;
		_currentChar = other._currentChar;
	}

	base_constexpr_cxx20 bool operator==(const StringConstIterator& other) const noexcept {
		compareStrings(other);
		return (_currentChar == other._currentChar);
	}

	base_constexpr_cxx20 bool operator!=(const StringConstIterator& other) const noexcept {
		return !(*this == other);
	}

	base_constexpr_cxx20 bool operator<(const StringConstIterator& other) const noexcept {
		compareStrings(other);
		return (_currentChar < other._currentChar);
	}

	base_constexpr_cxx20 bool operator>(const StringConstIterator& other) const noexcept {
		return !(*this < other);
	}

	base_constexpr_cxx20 bool operator<=(const StringConstIterator& other) const noexcept {
		compareStrings(other);
		return (_currentChar <= other._currentChar);
	}

	base_constexpr_cxx20 bool operator>=(const StringConstIterator& other) const noexcept {
		return !(*this <= other);
	}
#endif
private:
	inline base_constexpr_cxx20 void
		compareStrings(const StringConstIterator& other) const noexcept
	{
		DebugAssertLog(_container == other._container,
			"base::string::StringIterator::compareStrings: Невозможно сравнить итераторы различных объектов String. ");
	}

	// false if out of range
	inline base_constexpr_cxx20  
		bool checkOffset(size_type offset) const noexcept
	{
		const auto containerDataStart = _container->data();
		const auto containerDataEnd = containerDataStart + _container->size();

		return (_currentChar + offset > containerDataStart
			&& _currentChar + offset < containerDataEnd);
	}

	// false if out of range
	inline base_constexpr_cxx20  
		bool checkOffsetExclusive(size_type offset) const noexcept
	{
		const auto containerDataStart = _container->data();
		const auto containerDataEnd = containerDataStart + _container->size();

		return (_currentChar + offset >= containerDataStart
			&& _currentChar + offset <= containerDataEnd);
	}

	const _String* _container = nullptr;
	_String::pointer _currentChar = nullptr;

	friend typename _String;
};

__BASE_STRING_NAMESPACE_END
