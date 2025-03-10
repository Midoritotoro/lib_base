#pragma once 


#include <base/core/string/StringConfig.h>
#include <base/core/string/String.h>

__BASE_STRING_NAMESPACE_BEGIN

class StringIterator {
public:
	CONSTEXPR_CXX20 NODISCARD_CTOR StringIterator(const String* str):
		_container(str),
		_currentChar(nullptr)
	{}

	CONSTEXPR_CXX20 NODISCARD_CTOR StringIterator(const StringIterator& other) :
		_container(other._container),
		_currentChar(other._currentChar)
	{}

	CONSTEXPR_CXX20 ~StringIterator() {
		delete _currentChar;
	}

	CONSTEXPR_CXX20 Char& operator*() noexcept {
#ifdef _DEBUG
		AssertLog(_container != nullptr, "base::string::StringIterator::operator*: Попытка доступа к элементу удаленной строки. ");
#endif
		
		return *_currentChar;
	}

	CONSTEXPR_CXX20 StringIterator& operator--() noexcept {
#ifdef _DEBUG
		AssertLog(_currentChar, "cannot decrement value-initialized string iterator");

		AssertLog(_container, "cannot decrement string iterator because the iterator was "
			"invalidated (e.g. reallocation occurred, or the string was destroyed)");

		AssertLog(_container->constData() < _currentChar, "cannot decrement string iterator before begin");
#endif

		--_currentChar;
		return *this;
	}

	CONSTEXPR_CXX20 StringIterator operator--(int) noexcept {
		StringIterator temp = *this;
		--*this;
		return temp;
	}

	CONSTEXPR_CXX20 StringIterator& operator++() noexcept {
#ifdef _DEBUG
		AssertLog(_currentChar, "cannot decrement value-initialized string iterator");

		AssertLog(_container, "cannot decrement string iterator because the iterator was "
			"invalidated (e.g. reallocation occurred, or the string was destroyed)");

		AssertLog(_container->constData() < _currentChar, "cannot decrement string iterator before begin");
#endif

		++_currentChar;
		return *this;
	}

	CONSTEXPR_CXX20 StringIterator operator++(int) noexcept {
		StringIterator temp = *this;
		++*this;
		return temp;
	}

	CONSTEXPR_CXX20 StringIterator& operator+=(const sizetype _Off) noexcept {
#if _DEBUG
		AssertLog(checkOffset(_Off), "base::string::StringIterator::operator+=(sizetype): Индекс вне диапазона. ");
#endif

		_currentChar += _Off;
		return *this;
	}

	CONSTEXPR_CXX20 NODISCARD StringIterator operator+(const sizetype _Off) const noexcept {
		StringIterator _Tmp = *this;
		_Tmp += _Off;
		return _Tmp;
	}

	friend CONSTEXPR_CXX20 NODISCARD StringIterator operator+(
		const sizetype _Off, StringIterator _Next) noexcept {
		_Next += _Off;
		return _Next;
	}

	CONSTEXPR_CXX20 NODISCARD StringIterator& operator-=(const sizetype _Off) noexcept {
		return *this += -_Off;
	}

	CONSTEXPR_CXX20 NODISCARD StringIterator operator-(const sizetype _Off) const noexcept {
		StringIterator _Tmp = *this;
		_Tmp -= _Off;
		return _Tmp;
	}

	CONSTEXPR_CXX20 NODISCARD sizetype operator-(const StringIterator& _Right) const noexcept {
		compareStrings(_Right);
		return static_cast<sizetype>(_currentChar - _Right._currentChar);
	}

	CONSTEXPR_CXX20 NODISCARD Char& operator[](const sizetype _Off) const noexcept {
		return *(*this + _Off);
	}

#if BASE_HAS_CXX20
	inline CONSTEXPR_CXX20 std::strong_ordering operator<=>(const StringIterator& other) noexcept {
		compareStrings(other);
		return (_currentChar <=> other._currentChar);
	}
#else
	CONSTEXPR_CXX20 bool operator=(const StringIterator& other) noexcept {
		_container		= other._container;
		_currentChar	= other._currentChar;
	}

	CONSTEXPR_CXX20 bool operator==(const StringIterator& other) noexcept {
		compareStrings(other);
		return (_currentChar == other._currentChar);
	}

	CONSTEXPR_CXX20 bool operator!=(const StringIterator& other) noexcept {
		return !(*this == other);
	}

	CONSTEXPR_CXX20 bool operator<(const StringIterator& other) noexcept {
		compareStrings(other);
		return (_currentChar < other._currentChar);
	}

	CONSTEXPR_CXX20 bool operator>(const StringIterator& other) noexcept {
		return !(*this < other)
	}

	CONSTEXPR_CXX20 bool operator<=(const StringIterator& other) noexcept {
		compareStrings(other);
		return (_currentChar <= other._currentChar);
	}

	CONSTEXPR_CXX20 bool operator>=(const StringIterator& other) noexcept {
		return !(*this <= other);
	}
#endif
private:
	inline CONSTEXPR_CXX20 void compareStrings(const StringIterator& other) const noexcept {
#ifdef _DEBUG
	AssertLog(_container == other._container,
		"base::string::StringIterator::compareStrings: Невозможно сравнить итераторы различных объектов String. ");
#else
	UNUSED(other);
#endif
	}

	// false if out of range
	inline CONSTEXPR_CXX20 NODISCARD bool checkOffset(sizetype offset) const noexcept {
		const auto containerDataStart	= _container->data();
		const auto containerDataEnd		= containerDataStart + _container->size();

		return (_currentChar + offset > containerDataStart 
			&& _currentChar + offset < containerDataEnd);
	}

	// false if out of range
	inline CONSTEXPR_CXX20 NODISCARD bool checkOffsetExclusive(sizetype offset) const noexcept {
		const auto containerDataStart	= _container->data();
		const auto containerDataEnd		= containerDataStart + _container->size();

		return (_currentChar + offset >= containerDataStart
			&& _currentChar + offset <= containerDataEnd);
	}

	const String* _container = nullptr;
	Char* _currentChar = nullptr;
};
__BASE_STRING_NAMESPACE_END
