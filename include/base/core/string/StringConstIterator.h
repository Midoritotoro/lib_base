#pragma once 


#include <base/core/string/StringConfig.h>
#include <base/core/string/String.h>

__BASE_STRING_NAMESPACE_BEGIN

class StringConstIterator {
public:
	inline CONSTEXPR_CXX20 NODISCARD_CTOR StringConstIterator(const String* str) :
		_container(str),
		_currentChar(str->data())
	{}

	inline CONSTEXPR_CXX20 NODISCARD_CTOR StringConstIterator(const StringConstIterator& other) :
		_container(other._container),
		_currentChar(other._currentChar)
	{}

	inline CONSTEXPR_CXX20 ~StringConstIterator() {

	}

	CONSTEXPR_CXX20 Char& operator*() noexcept {
		DebugAssertLog(_container != nullptr, "base::string::StringIterator::operator*: Попытка доступа к элементу удаленной строки. ");
		return *_currentChar;
	}

private:
	inline CONSTEXPR_CXX20 void
		compareStrings(const StringConstIterator& other) const noexcept
	{
		DebugAssertLog(_container == other._container,
			"base::string::StringIterator::compareStrings: Невозможно сравнить итераторы различных объектов String. ");
	}

	// false if out of range
	inline CONSTEXPR_CXX20 NODISCARD
		bool checkOffset(sizetype offset) const noexcept
	{
		const auto containerDataStart = _container->data();
		const auto containerDataEnd = containerDataStart + _container->size();

		return (_currentChar + offset > containerDataStart
			&& _currentChar + offset < containerDataEnd);
	}

	// false if out of range
	inline CONSTEXPR_CXX20 NODISCARD
		bool checkOffsetExclusive(sizetype offset) const noexcept
	{
		const auto containerDataStart = _container->data();
		const auto containerDataEnd = containerDataStart + _container->size();

		return (_currentChar + offset >= containerDataStart
			&& _currentChar + offset <= containerDataEnd);
	}

	const String* _container = nullptr;
	Char* _currentChar = nullptr;
};
__BASE_STRING_NAMESPACE_END
