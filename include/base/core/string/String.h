#pragma once 


#include <base/core/string/StringConfig.h>
#include <type_traits>

__BASE_STRING_NAMESPACE_BEGIN

std::string 

template <
	typename Char = NativeChar,
	typename = std::enable_if_t<
		std::is_same_v<Char, wchar_t> 
		|| std::is_same_v<Char, char>>>
class String {
public:
	String();
	~String();

	String(const String& string);
	String(const Char* chs);

	String(const wchar_t* wch);
	String(const char* ch);

	String(const std::string& str);
	String(const std::wstring& str)

	NODISCARD constexpr Char& operator[](const sizetype index) noexcept;
	NODISCARD constexpr Char at(const sizetype index) noexcept;

	NODISCARD NativeString toNativeString() const noexcept;

	NODISCARD std::wstring toStdWString() const noexcept;
	NODISCARD std::string toStdString() const noexcept;

	NODISCARD constexpr sizetype size() const noexcept;
	NODISCARD constexpr sizetype length() const noexcept;

	NODISCARD constexpr void append(const Char* ch) noexcept;
	NODISCARD constexpr void insert(
		const Char* ch,
		sizetype index) noexcept;

	NODISCARD constexpr Char pop() noexcept;
	NODISCARD constexpr 
private:
	Char* data = nullptr;
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

