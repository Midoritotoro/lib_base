#pragma once 

#include <base/core/string/StringConfig.h>
#include <base/core/string/Char.h>

#include <base/core/string/StringIterator.h>
#include <base/core/string/StringConstIterator.h>

#include <base/core/utility/TypeTraits.h>

__BASE_STRING_NAMESPACE_BEGIN


class String {
public:
    using iterator                  = StringIterator<String>;
    using const_iterator            = StringConstIterator<String>;

    using Iterator                  = iterator;
    using ConstIterator             = const_iterator;

    using reverse_iterator          = std::reverse_iterator<iterator>;
    using const_reverse_iterator    = std::reverse_iterator<const_iterator>;

	using ReverseIterator			= reverse_iterator;
	using ConstReverseIterator		= const_reverse_iterator;
  
    using size_type                 = sizetype;
    using difference_type           = ptrdiff;

    using const_reference           = const Char&;
    using reference                 = Char&;

    using pointer                   = Char*;
    using const_pointer             = const Char*;

    using value_type                = Char;
	using CaseSensitivity			= Char::CaseSensitivity;

	CONSTEXPR_CXX20 String();
	~String();

	String(const String& string);
	String(const Char* chs);

	NODISCARD Char& operator[](const sizetype index) noexcept;
	NODISCARD Char at(const sizetype index) const noexcept;

	NODISCARD NativeString toNativeString() const noexcept;

	NODISCARD std::wstring toStdWString() const noexcept;
	NODISCARD std::string toStdString() const noexcept;

	NODISCARD sizetype size() const noexcept;
	NODISCARD sizetype length() const noexcept;

	NODISCARD sizetype capacity() const noexcept;

	NODISCARD void append(Char ch) noexcept;
	NODISCARD void insert(
		Char ch,
		sizetype index) noexcept;

	NODISCARD Char pop() noexcept;
	NODISCARD sizetype find(const Char* chs) const noexcept;

	NODISCARD sizetype findLastOf(const Char* chs) const noexcept;
	NODISCARD sizetype findFirstOf(const Char* chs) const noexcept;

	NODISCARD sizetype findLastNotOf(const Char* chs) const noexcept;
	NODISCARD sizetype findFirstNotOf(const Char* chs) const noexcept;

	NODISCARD bool isEmpty() const noexcept;
	NODISCARD bool isNull() const noexcept;

	NODISCARD Char* data() const noexcept;
	NODISCARD const Char* constData() const noexcept;

	NODISCARD CONSTEXPR_CXX20 Iterator begin() noexcept;
	NODISCARD CONSTEXPR_CXX20 ConstIterator begin() const noexcept;

	NODISCARD CONSTEXPR_CXX20 ConstIterator cbegin() const noexcept;
	NODISCARD CONSTEXPR_CXX20 ConstIterator constBegin() const noexcept;

	NODISCARD CONSTEXPR_CXX20 Iterator end() noexcept;
	NODISCARD CONSTEXPR_CXX20 ConstIterator end() const noexcept;

	NODISCARD CONSTEXPR_CXX20 ConstIterator cend() const noexcept;
	NODISCARD CONSTEXPR_CXX20 ConstIterator constEnd() const noexcept;

	NODISCARD CONSTEXPR_CXX20 ReverseIterator rbegin() noexcept;
	NODISCARD CONSTEXPR_CXX20 ReverseIterator rend() noexcept;

	NODISCARD CONSTEXPR_CXX20 ConstReverseIterator rbegin() const noexcept;
	NODISCARD CONSTEXPR_CXX20 ConstReverseIterator rend() const noexcept;

	NODISCARD CONSTEXPR_CXX20 ConstReverseIterator crbegin() const noexcept;
	NODISCARD CONSTEXPR_CXX20 ConstReverseIterator crend() const noexcept;

	void push_back(Char c);
	void push_back(const String& s);

	void push_front(Char c);
	void push_front(const String& s);

	void clear();

	NODISCARD Iterator erase(
		ConstIterator first,
		ConstIterator last);
	NODISCARD Iterator erase(ConstIterator it);


	NODISCARD Char front() const;
	NODISCARD Char& front();

	NODISCARD Char back() const;
	NODISCARD Char& back();

	void resize(sizetype size);
	void resize(
		sizetype size,
		Char fill);


	NODISCARD sizetype indexOf(
		Char ch,
		sizetype from = 0, 
		CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;
	NODISCARD sizetype indexOf(
		const String& string,
		sizetype from = 0, 
		CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;

	NODISCARD sizetype lastIndexOf(
		Char c,
		CaseSensitivity cs = CaseSensitivity::CaseSensitive) const noexcept;
	NODISCARD sizetype lastIndexOf(
		Char c, 
		sizetype from, 
		CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;

	NODISCARD sizetype lastIndexOf(
		const String& string,
		CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;
	NODISCARD sizetype lastIndexOf(
		const String& string,
		sizetype from,
		CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;


	NODISCARD bool contains(
		Char ch, 
		CaseSensitivity caseSensitivity = CaseSensitivity::CaseSensitive) const;
	NODISCARD bool contains(
		const String& string,
		CaseSensitivity caseSensitivity = CaseSensitivity::CaseSensitive) const;
;
	NODISCARD sizetype count(
		Char ch, 
		CaseSensitivity caseSensitivity = CaseSensitivity::CaseSensitive) const;
	NODISCARD sizetype count(
		const String& string,
		CaseSensitivity caseSensitivity = CaseSensitivity::CaseSensitive) const;

	NODISCARD String& fill(
		Char ch, 
		sizetype size = -1);
private:
	std::vector<Char> _data; // ѕотом заменю на кастомный контейнер
};

__BASE_STRING_NAMESPACE_END

