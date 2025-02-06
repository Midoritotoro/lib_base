#pragma once 

#include <base/system/SystemDetection.h>
#include <base/system/CompilerDetection.h>



#if defined(OS_WIN)
    #include <base/system/Windows.h>
    #include <winnt.h>
    
    // При компиляции не на процессоре x86 включается UNICODE.
    #if !defined(PROCESSOR_X86_32)
        #ifndef UNICODE
            #define UNICODE
        #endif

        #define LIB_BASE_ENABLE_WINDOWS_UNICODE
        #include <corecrt_wstring.h>
    #endif
    
    // При включении UNICODE, стоит также включить C-шные функции для работы с ним
    #ifdef UNICODE
        #define _UNICODE
    #endif
#endif

// При включении UNICODE на Windows используется 16-битный Unicode
// https://ru.wikipedia.org/wiki/UTF-16
#if defined(OS_WIN)
    #if defined(LIB_BASE_ENABLE_WINDOWS_UNICODE)
        using base_string   =   std::wstring;
        using base_char     =   wchar_t;
    #else 
        using base_string   =   std::string;
        using base_char     =   char;
    #endif
#else
    using base_string       =   std::string;
    using base_char         =   char;
#endif

// Упрощение использования функций для работы со строками и уменьшение
// количества проверок в коде
#if defined(OS_WIN) && defined(LIB_BASE_ENABLE_WINDOWS_UNICODE)
    #define base_strstr             wcsstr
    #define base_strcmp             wcscmp
    #define base_strlen(string)     wcslen(string) - 1
    #define base_strcmp             lstrcmpW         
#else
    #define base_strstr             strstr
    #define base_strcmp             strcmp
    #define base_strlen             strlen
    #define base_strcmp             strcmp
#endif


#if defined(OS_WIN) && defined(LIB_BASE_ENABLE_WINDOWS_UNICODE)
    extern "C" __declspec(dllimport) int __stdcall 
        MultiByteToWideChar(
            _SAL2_In_ unsigned int CodePage,
            _SAL2_In_ unsigned long dwFlags,
            _SAL2_In_NLS_string_(cbMultiByte) const char* lpMultiByteStr,
            _SAL2_In_ int cbMultiByte,
            _SAL2_Out_writes_to_opt_(cchWideChar, return) wchar_t* lpWideCharStr,
            _SAL2_In_ int cchWideChar);

	extern "C" __declspec(dllimport) int __stdcall 
        WideCharToMultiByte(
            _SAL2_In_ unsigned int CodePage,
            _SAL2_In_ unsigned long dwFlags,
            _SAL2_In_NLS_string_(cchWideChar) const wchar_t* lpWideCharStr,
            _SAL2_In_ int cchWideChar,
            _SAL2_Out_writes_bytes_to_opt_(cbMultiByte, return) char* lpMultiByteStr,
            _SAL2_In_ int cbMultiByte,
            _SAL2_In_opt_ const char* lpDefaultChar,
            _SAL2_Out_opt_ int* lpUsedDefaultChar);
#endif


#if defined(OS_WIN) && defined(LIB_BASE_ENABLE_WINDOWS_UNICODE)
    inline [[nodiscard]] int ConvertWCharToUnicode(
        char* buffer,
        size_t bufferlen,
        const wchar_t* input)
    {
        return WideCharToMultiByte(
            CP_UTF8, 0, input, -1,
            buffer, (int)bufferlen, NULL, NULL);
    }

    inline [[nodiscard]] int ConvertUnicodeToWChar(
        wchar_t* buffer,
        size_t bufferlen,
        const char* input)
    {
        return MultiByteToWideChar(
            CP_UTF8, 0, input, -1,
            buffer, (int)bufferlen);
    }
#endif


#if defined(OS_WIN)
    #if defined(LIB_BASE_ENABLE_WINDOWS_UNICODE)
        wchar_t* ConvertString(char* str) {
            if (!str) 
                return nullptr;

            size_t len = strlen(str) + 1;
            wchar_t* wstr = new wchar_t[len];

            if (ConvertUnicodeToWChar(wstr, (int)len, str) == 0) {
                delete[] wstr;
                return nullptr;
            }

            return wstr;
        }

        std::wstring ConvertString(const std::string& str) {
            if (str.empty()) 
                return L"";

            size_t len = str.length() + 1;
            std::wstring wstr;

            wstr.resize(len - 1);

            if (ConvertUnicodeToWChar(&wstr[0], (int)len, str.c_str()) == 0)
                return L"";
        
            wstr.resize(wcslen(wstr.c_str()));
            return wstr;
        }
    #else
        char* ConvertString(wchar_t* wstr) {
            if (!wstr) 
                return nullptr;

            size_t len = wcslen(wstr) + 1;
            char* str = new char[len];

            if (ConvertWCharToUnicode(str, (int)len, wstr) == 0) {
                delete[] str;
                return nullptr;
            }

            return str;
        }

        std::string ConvertString(const std::wstring& wstr) {
            if (wstr.empty()) 
                return "";

            size_t len = wstr.length() + 1;
            std::string str;

            str.resize(len - 1);

            if (ConvertWCharToUnicode(&str[0], (int)len, wstr.c_str()) == 0)
                return "";
        
            str.resize(strlen(str.c_str()));
            return str;
        }

    #endif
#endif