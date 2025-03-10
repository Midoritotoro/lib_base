#pragma once 


#include <base/core/string/StringConfig.h>
#include <base/core/string/String.h>



__BASE_STRING_NAMESPACE_BEGIN

template <
	typename Char = NativeChar, 
	std::enable_if_t<
		std::is_same_v<Char, char> 
		|| std::is_same_v<Char, wchar_t>>>
class StringIterator {
public:
	NODISCARD_CTOR StringIterator(const String<Char>* str):
		_containter(str),
		_currentChar(str[0])
	{}

	NODISCARD_CTOR StringIterator(const StringIterator& other) :
		_containter(other._containter),
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
		AssertLog(checkOffsetExclusive(_currentChar), "base::string::StringIterator::operator--(int): Индекс вне диапазона. ");
#endif

		--_currentChar;
		return *this;
	}

	inline CONSTEXPR_CXX20 StringIterator operator++() noexcept {
		StringIteratpr temp = *this;
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

#if BASE_HAS_CXX20
	inline CONSTEXPR_CXX20 std::strong_ordering operator<=>(const StringIterator& other) noexcept {
		compareStrings(other);
		return (_currentChar <=> other._currentChar);
	}
#else
	inline bool operator=(const StringIterator& other) noexcept {
		_container		= other._container;
		_currentChar	= other._currentChar;
	}

	inline bool operator==(const StringIterator& other) noexcept {
		compareStrings(other);
		return (_currentChar == other._currentChar);
	}

	inline bool operator!=(const StringIterator& other) noexcept {
		return !(*this == other);
	}

	inline bool operator<(const StringIterator& other) noexcept {
		compareStrings(other);
		return (_currentChar < other._currentChar);
	}

	inline bool operator>(const StringIterator& other) noexcept {
		return !(*this < other)
	}

	inline bool operator<=(const StringIterator& other) noexcept {
		compareStrings(other);
		return (_currentChar <= other._currentChar);
	}

	inline bool operator>=(const StringIterator& other) noexcept {
		return !(*this <= other);
	}
#endif
private:
	CONSTEXPR_CXX20 void compareStrings(const StringIterator& other) noexcept {
#ifdef _DEBUG
	AssertLog(_container == _other._container,
		"base::string::StringIterator::compareStrings: Невозможно сравнить итераторы различных объектов String. ")
#else
	UNUSED(other);
#endif
	}

	// false if out of range
	CONSTEXPR_CXX20 NODISCARD bool checkOffset(sizetype offset) noexcept {
		const auto containerDataStart	= _container->data();
		const auto containerDataEnd		= containerDataStart + _container->size();

		return (_currentChar + offset > containerDataStart 
			&& _currentChar + offset < containerDataEnd);
	}

	// false if out of range
	CONSTEXPR_CXX20 NODISCARD bool checkOffsetExclusive(sizetype offset) noexcept {
		const auto containerDataStart	= _container->data();
		const auto containerDataEnd		= containerDataStart + _container->size();

		return (_currentChar + offset >= containerDataStart
			&& _currentChar + offset <= containerDataEnd);
	}

	String<Char>* _container = nullptr;
	Char* _currentChar = nullptr;
};
__BASE_STRING_NAMESPACE_END