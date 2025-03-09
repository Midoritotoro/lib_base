#pragma once 


#include <base/core/string/StringConfig.h>

__BASE_STRING_NAMESPACE_BEGIN

template <
	class Char = NativeChar,
	std::enable_if_t<
		std::is_same_v<Char, wchar_t> 
		|| std::is_same_v<Char, char>>
class String {
public:
	String();

	String(const String& string);
	String(const Char* chars);



	~String();
private:
	Char* data = nullptr;
};
//#if defined(OS_WIN) && defined(LIB_BASE_ENABLE_WINDOWS_UNICODE)
//    #define base_strstr             wcsstr
//    #define base_strcmp             wcscmp
//    #define base_strlen(string)     wcslen(string) - 1     
//    #define base_toupper            towupper
//#else
//    #define base_strstr             strstr
//    #define base_strcmp             strcmp
//    #define base_strlen             strlen
//    #define base_toupper            toupper
//#endif
//
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

