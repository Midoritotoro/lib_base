#pragma once 

#include <base/core/arch/Platform.h>
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
    using NativeString      =   std::wstring;
    using NativeStringView  =   std::wstring_view;
    using NativeChar        =   wchar_t;
#else
    using NativeString      =   std::string;
    using NativeStringView  =   std::string_view;
    using NativeChar        =   char;
#endif


#define ENGLISH_ALPHABET_ASCII_START        97
#define ENGLISH_ALPHABET_ASCII_END          122

#define ENGLISH_ALPHABET_SIZE               26