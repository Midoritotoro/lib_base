#pragma once 

#include <base/core/arch/SystemDetection.h>
#include <base/core/arch/CompilerDetection.h>

#include <string>


WARNING_DISABLE_MSVC(4005)

#if defined(OS_WIN)
    #include <base/core/system/Windows.h>
    #include <winnt.h>
    
    #if !defined(PROCESSOR_X86_32)
        #ifndef UNICODE
            #define UNICODE
        #endif

        #define LIB_BASE_ENABLE_WINDOWS_UNICODE
        #include <corecrt_wstring.h>
    #endif
    

    #ifdef UNICODE
        #define _UNICODE
    #endif
#endif

#if defined(OS_WIN) && defined(LIB_BASE_ENABLE_WINDOWS_UNICODE)
    using base_string   =   std::wstring;
    using base_char     =   wchar_t;
#else
    using base_string   =   std::string;
    using base_char     =   char;
#endif


#define ENGLISH_ALPHABET_ASCII_START        97
#define ENGLISH_ALPHABET_ASCII_END          122

#define ENGLISH_ALPHABET_SIZE               26

#if defined(OS_WIN) && defined(LIB_BASE_ENABLE_WINDOWS_UNICODE)
    #define base_strstr             wcsstr
    #define base_strcmp             wcscmp
    #define base_strlen(string)     wcslen(string) - 1     
    #define base_toupper            towupper
#else
    #define base_strstr             strstr
    #define base_strcmp             strcmp
    #define base_strlen             strlen
    #define base_toupper            toupper
#endif

#if defined(OS_WIN) && defined(LIB_BASE_ENABLE_WINDOWS_UNICODE)
    inline [[nodiscard]] int ConvertWCharToUnicode(
        char* buffer,
        size_t bufferlen,
        const wchar_t* input)
    {
        return WideCharToMultiByte(
            CP_UTF8, 0, input, -1,
            buffer, (int)bufferlen,
            nullptr, nullptr);
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
