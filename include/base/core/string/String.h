#pragma once 

#include <base/core/string/StringConfig.h>
#include <base/core/string/Char.h>

#include <type_traits>

__BASE_STRING_NAMESPACE_BEGIN


class StringIterator;

class String {
public:
	using Iterator = StringIterator;

	String();
	~String();

	String(const String& string);
	String(const Char* chs);

	NODISCARD CONSTEXPR_CXX20 Char& operator[](const sizetype index) noexcept;
	NODISCARD CONSTEXPR_CXX20 Char at(const sizetype index) noexcept;

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

	NODISCARD CONSTEXPR_CXX20 Char* data() const noexcept;
	NODISCARD CONSTEXPR_CXX20 const Char* constData() const noexcept;
private:
	Char* _data = nullptr;
};

//#if defined(OS_WIN) && defined(LIB_BASE_ENABLE_WINDOWS_UNICODE)
//    inline [[nodiscard]] int ConvertWCharToUnicode(
//        char* buffer,
//        size_t bufferlen,
//        const wchar_t* input)
//    {
//        return WideCharToMultiByte(
//            CP_UTF8, 0, input, -1,
//            buffer, (int)bufferlen,
//            nullptr, nullptr);
//    }
//
//    inline [[nodiscard]] int ConvertUnicodeToWChar(
//        wchar_t* buffer,
//        size_t bufferlen,
//        const char* input)
//    {
//        return MultiByteToWideChar(
//            CP_UTF8, 0, input, -1,
//            buffer, (int)bufferlen);
//    }
//#endif

__BASE_STRING_NAMESPACE_END

