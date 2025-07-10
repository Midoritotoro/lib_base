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

	inline CONSTEXPR_CXX20 NODISCARD_CTOR StringConstIterator(const _String* str) :
		_container(str),
		_currentChar(str->data())
	{}

	inline CONSTEXPR_CXX20 NODISCARD_CTOR StringConstIterator(const StringConstIterator& other) :
		_container(other._container),
		_currentChar(other._currentChar)
	{}

	inline CONSTEXPR_CXX20 ~StringConstIterator() {

	}

	CONSTEXPR_CXX20 Char& operator*() const noexcept {
		DebugAssertLog(_container != nullptr, "base::string::StringIterator::operator*: Попытка доступа к элементу удаленной строки. ");
		return *_currentChar;
	}

	CONSTEXPR_CXX20 StringConstIterator& operator--() noexcept {
		DebugAssertLog(_currentChar, "cannot decrement value-initialized string iterator");

		DebugAssertLog(_container, "cannot decrement string iterator because the iterator was "
			"invalidated (e.g. reallocation occurred, or the string was destroyed)");

		DebugAssertLog(_container->constData() < _currentChar, "cannot decrement string iterator before begin");

		--_currentChar;
		return *this;
	}

	CONSTEXPR_CXX20 StringConstIterator operator--(int) noexcept {
		StringConstIterator temp = *this;
		--*this;

		return temp;
	}

	CONSTEXPR_CXX20 StringConstIterator& operator++() noexcept {
		DebugAssertLog(_currentChar, "cannot decrement value-initialized string iterator");

		DebugAssertLog(_container, "cannot decrement string iterator because the iterator was "
			"invalidated (e.g. reallocation occurred, or the string was destroyed)");

		DebugAssertLog(_container->constData() < _currentChar, "cannot decrement string iterator before begin");

		++_currentChar;
		return *this;
	}

	CONSTEXPR_CXX20 StringConstIterator operator++(int) noexcept {
		StringConstIterator temp = *this;
		++*this;

		return temp;
	}

	CONSTEXPR_CXX20 StringConstIterator& operator+=(const size_type _Off) noexcept {
		DebugAssertLog(checkOffset(_Off), "base::string::StringIterator::operator+=(sizetype): Индекс вне диапазона. ");

		_currentChar += _Off;
		return *this;
	}

	CONSTEXPR_CXX20 NODISCARD StringConstIterator
		operator+(const size_type offset) const noexcept
	{
		StringConstIterator temp = *this;
		temp += offset;

		return temp;
	}

	friend CONSTEXPR_CXX20 NODISCARD
		StringConstIterator operator+(
			const size_type offset,
			StringConstIterator next) noexcept
	{
		next += offset;
		return next;
	}

	CONSTEXPR_CXX20 NODISCARD StringConstIterator& operator-=(const size_type offset) noexcept {
		return *this += -offset;
	}

	CONSTEXPR_CXX20 NODISCARD StringConstIterator operator-(const size_type offset) const noexcept {
		StringConstIterator temp = *this;
		temp -= offset;

		return temp;
	}

	CONSTEXPR_CXX20 NODISCARD size_type operator-(const StringConstIterator& _Right) const noexcept {
		compareStrings(_Right);
		return static_cast<size_type>(_currentChar - _Right._currentChar);
	}

	CONSTEXPR_CXX20 NODISCARD reference operator[](const size_type _Off) const noexcept {
		return *(*this + _Off);
	}

#if BASE_HAS_CXX20
	CONSTEXPR_CXX20 NODISCARD auto /*std::strong_ordering*/
		operator<=>(const StringConstIterator& other) const noexcept
	{
		compareStrings(other);
		return (_currentChar <=> other._currentChar);
	}
#else // !BASE_HAS_CXX20
	CONSTEXPR_CXX20 StringConstIterator& operator=(const StringConstIterator& other) const noexcept {
		_container = other._container;
		_currentChar = other._currentChar;
	}

	CONSTEXPR_CXX20 bool operator==(const StringConstIterator& other) const noexcept {
		compareStrings(other);
		return (_currentChar == other._currentChar);
	}

	CONSTEXPR_CXX20 bool operator!=(const StringConstIterator& other) const noexcept {
		return !(*this == other);
	}

	CONSTEXPR_CXX20 bool operator<(const StringConstIterator& other) const noexcept {
		compareStrings(other);
		return (_currentChar < other._currentChar);
	}

	CONSTEXPR_CXX20 bool operator>(const StringConstIterator& other) const noexcept {
		return !(*this < other);
	}

	CONSTEXPR_CXX20 bool operator<=(const StringConstIterator& other) const noexcept {
		compareStrings(other);
		return (_currentChar <= other._currentChar);
	}

	CONSTEXPR_CXX20 bool operator>=(const StringConstIterator& other) const noexcept {
		return !(*this <= other);
	}
#endif
private:
	inline CONSTEXPR_CXX20 void
		compareStrings(const StringConstIterator& other) const noexcept
	{
		DebugAssertLog(_container == other._container,
			"base::string::StringIterator::compareStrings: Невозможно сравнить итераторы различных объектов String. ");
	}

	// false if out of range
	inline CONSTEXPR_CXX20 NODISCARD
		bool checkOffset(size_type offset) const noexcept
	{
		const auto containerDataStart = _container->data();
		const auto containerDataEnd = containerDataStart + _container->size();

		return (_currentChar + offset > containerDataStart
			&& _currentChar + offset < containerDataEnd);
	}

	// false if out of range
	inline CONSTEXPR_CXX20 NODISCARD
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
