#pragma once 

#include <base/core/string/StringConfig.h>
#include <base/core/string/Char.h>

#include <base/core/string/StringIterator.h>
#include <base/core/string/StringConstIterator.h>

#include <base/core/utility/TypeTraits.h>

__BASE_STRING_NAMESPACE_BEGIN


class String {
public:
    using iterator                  = StringIterator;
    using const_iterator            = StringConstIterator;
    using Iterator                  = iterator;
    using ConstIterator             = const_iterator;
    using reverse_iterator          = std::reverse_iterator<iterator>;
    using const_reverse_iterator    = std::reverse_iterator<const_iterator>;
  
    using size_type                 = sizetype;
    using difference_type           = ptrdiff;
    using const_reference           = const Char&;
    using reference                 = Char&;
    using pointer                   = Char*;
    using const_pointer             = const Char*;
    using value_type                = Char;

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

	NODISCARD void append(Char ch) noexcept;
	NODISCARD void insert(
		Char ch,
		sizetype index) noexcept;

	NODISCARD Char pop() noexcept;

	NODISCARD StringIterator begin() const noexcept;
	NODISCARD StringIterator end() const noexcept;

	NODISCARD sizetype find(const Char* chs) const noexcept;

	NODISCARD sizetype findLastOf(const Char* chs) const noexcept;
	NODISCARD sizetype findFirstOf(const Char* chs) const noexcept;

	NODISCARD sizetype findLastNotOf(const Char* chs) const noexcept;
	NODISCARD sizetype findFirstNotOf(const Char* chs) const noexcept;

	iterator erase(
		const StringIterator& first,
		const StringIterator& last);
	void clear();

	NODISCARD bool isEmpty() const noexcept;
	NODISCARD bool isNull() const noexcept;

	NODISCARD Char* data() const noexcept;
	NODISCARD const Char* constData() const noexcept;

	NODISCARD CONSTEXPR_CXX20 iterator begin() noexcept;

	NODISCARD CONSTEXPR_CXX20 const_iterator begin() const noexcept;
	NODISCARD CONSTEXPR_CXX20 const_iterator cbegin() const noexcept;
	NODISCARD CONSTEXPR_CXX20 const_iterator constBegin() const noexcept;

	NODISCARD CONSTEXPR_CXX20 iterator end() noexcept;

	NODISCARD CONSTEXPR_CXX20 const_iterator end() const noexcept;
	NODISCARD CONSTEXPR_CXX20 const_iterator cend() const noexcept;
	NODISCARD CONSTEXPR_CXX20 const_iterator constEnd() const noexcept;

	NODISCARD CONSTEXPR_CXX20 reverse_iterator rbegin() noexcept;
	NODISCARD CONSTEXPR_CXX20 reverse_iterator rend() noexcept;

	NODISCARD CONSTEXPR_CXX20 const_reverse_iterator rbegin() const noexcept;
	NODISCARD CONSTEXPR_CXX20 const_reverse_iterator rend() const noexcept;

	NODISCARD CONSTEXPR_CXX20 const_reverse_iterator crbegin() const noexcept;
	NODISCARD CONSTEXPR_CXX20 const_reverse_iterator crend() const noexcept;

	void push_back(Char c);
	void push_back(const String& s);

	void push_front(Char c);
	void push_front(const String& s);

	NODISCARD iterator erase(
		const_iterator first, 
		const_iterator last);
	NODISCARD iterator erase(const_iterator it);
private:
	std::vector<Char> _data; // ѕотом заменю на кастомный контейнер
};

__BASE_STRING_NAMESPACE_END

