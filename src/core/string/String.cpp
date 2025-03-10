#include <base/core/string/String.h>

__BASE_STRING_NAMESPACE_BEGIN

String::String() {

}

String::~String() {

}

String::String(const String& string) {

}

String::String(const Char* chs) {

}

String::String(const wchar_t* wch) {

}

String::String(const char* ch) {

}

String::String(const std::string& str) {

}

String::String(const std::wstring& str) {

}

CONSTEXPR_CXX20 Char& String::operator[](const sizetype index) noexcept {

}

CONSTEXPR_CXX20 Char String::at(const sizetype index) noexcept {

}

NODISCARD NativeString toNativeString() const noexcept;

NODISCARD std::wstring toStdWString() const noexcept;
NODISCARD std::string toStdString() const noexcept;

NODISCARD CONSTEXPR_CXX20 sizetype size() const noexcept;
NODISCARD CONSTEXPR_CXX20 sizetype length() const noexcept;

NODISCARD CONSTEXPR_CXX20 void append(const Char* ch) noexcept;
NODISCARD CONSTEXPR_CXX20 void insert(
	const Char* ch,
	sizetype index) noexcept;

NODISCARD CONSTEXPR_CXX20 Char pop() noexcept;

NODISCARD CONSTEXPR_CXX20 Iterator begin() noexcept;
NODISCARD CONSTEXPR_CXX20 Iterator end() noexcept;

NODISCARD CONSTEXPR_CXX20 sizetype find(const Char* chs) noexcept;
NODISCARD CONSTEXPR_CXX20 sizetype findLastOf(const Char* chs) noexcept;

__BASE_STRING_NAMESPACE_END