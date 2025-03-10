#pragma once 


#include <base/core/string/StringConfig.h>
#include <base/core/string/String.h>

__BASE_STRING_NAMESPACE_BEGIN

// std::string
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

	~StringIterator() {
		delete _currentChar;
	}

	inline CONSTEXPR_CXX20 Char operator*() noexcept {
#ifdef _DEBUG
		AssertLog(_container != nullptr, "base::string::StringIterator::operator*: Попытка доступа к элементу удаленной строки. ");
#endif
		
		return *_currentChar;
	}

	inline CONSTEXPR_CXX20 StringIterator operator--() noexcept {
		StringIterator temp = *this;
		--*this;
		return temp;
	}

	inline CONSTEXPR_CXX20 StringIterator operator--(int) noexcept {
#ifdef _DEBUG
	AssertLog(_currentChar, "cannot decrement value-initialized string iterator");

	AssertLog(_container, "cannot decrement string iterator because the iterator was "
		"invalidated (e.g. reallocation occurred, or the string was destroyed)");

	AssertLog(_container.data() < _currentChar, "cannot decrement string iterator before begin");
#endif

		--_currentChar;
		return *this;
	}

	inline CONSTEXPR_CXX20 StringIterator operator++() noexcept {
		StringIterator temp = *this;
		++*this;
		return temp;
	}

	inline CONSTEXPR_CXX20 StringIterator operator++(int) noexcept {
#ifdef _DEBUG
		AssertLog(checkOffsetExclusive(_currentChar), "base::string::StringIterator::operator--(int): Индекс вне диапазона. ");
#endif

		++_currentChar;
		return *this;
	}

	CONSTEXPR_CXX20 StringIterator& operator+=(const sizetype _Off) noexcept {
#if _DEBUG
		AssertLog(checkOffset(_currentChar), "base::string::StringIterator::operator+=(sizetype): Индекс вне диапазона. ");
#endif // _ITERATOR_DEBUG_LEVEL >= 1
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
		return static_cast<sizetype>(_Ptr - _Right._Ptr);
	}

	CONSTEXPR_CXX20 NODISCARD reference operator[](const sizetype _Off) const noexcept {
		return *(*this + _Off);
	}

#if BASE_HAS_CXX20
	inline CONSTEXPR_CXX20 std::strong_ordering operator<=>(const StringIterator& other) noexcept {
		compareStrings(other);
		return (_currentChar <=> other._currentChar);
	}
#else
	inline CONSTEXPR_CXX20 bool operator=(const StringIterator& other) noexcept {
		_container		= other._container;
		_currentChar	= other._currentChar;
	}

	inline CONSTEXPR_CXX20 bool operator==(const StringIterator& other) noexcept {
		compareStrings(other);
		return (_currentChar == other._currentChar);
	}

	inline CONSTEXPR_CXX20 bool operator!=(const StringIterator& other) noexcept {
		return !(*this == other);
	}

	inline CONSTEXPR_CXX20 bool operator<(const StringIterator& other) noexcept {
		compareStrings(other);
		return (_currentChar < other._currentChar);
	}

	inline CONSTEXPR_CXX20 bool operator>(const StringIterator& other) noexcept {
		return !(*this < other)
	}

	inline CONSTEXPR_CXX20 bool operator<=(const StringIterator& other) noexcept {
		compareStrings(other);
		return (_currentChar <= other._currentChar);
	}

	inline CONSTEXPR_CXX20 bool operator>=(const StringIterator& other) noexcept {
		return !(*this <= other);
	}
#endif
private:
	inline CONSTEXPR_CXX20 void compareStrings(const StringIterator& other) noexcept {
#ifdef _DEBUG
	AssertLog(_container == other._container,
		"base::string::StringIterator::compareStrings: Невозможно сравнить итераторы различных объектов String. ");
#else
	UNUSED(other);
#endif
	}

	// false if out of range
	inline CONSTEXPR_CXX20 NODISCARD bool checkOffset(sizetype offset) noexcept {
		const auto containerDataStart	= _container->data();
		const auto containerDataEnd		= containerDataStart + _container->size();

		return (_currentChar + offset > containerDataStart 
			&& _currentChar + offset < containerDataEnd);
	}

	// false if out of range
	inline CONSTEXPR_CXX20 NODISCARD bool checkOffsetExclusive(sizetype offset) noexcept {
		const auto containerDataStart	= _container->data();
		const auto containerDataEnd		= containerDataStart + _container->size();

		return (_currentChar + offset >= containerDataStart
			&& _currentChar + offset <= containerDataEnd);
	}

	const String* _container = nullptr;
	Char* _currentChar = nullptr;
};
__BASE_STRING_NAMESPACE_END