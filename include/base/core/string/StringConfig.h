#pragma once 

#include <base/core/arch/Platform.h>
#include <string>


base_disable_warning_msvc(4005)

#if defined(base_os_windows)
    #include <base/core/system/Windows.h>
    #include <winnt.h>
    
    #if !defined(base_processor_x86_32)
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


__BASE_STRING_NAMESPACE_BEGIN

#if defined(base_os_windows) && defined(LIB_BASE_ENABLE_WINDOWS_UNICODE)
    using NativeString      =   std::wstring;
    using NativeStringView  =   std::wstring_view;
    using NativeChar        =   wchar_t;
#else
    using NativeString      =   std::string;
    using NativeStringView  =   std::string_view;
    using NativeChar        =   char;
#endif

enum _CaseSensitivity : uchar {
    CaseSensitive,
    CaseInsensitive
};

DECLARE_FLAGS_ENUM(CaseSensitivity, _CaseSensitivity)


__BASE_STRING_NAMESPACE_END

#define ENGLISH_ALPHABET_ASCII_START        97
#define ENGLISH_ALPHABET_ASCII_END          122

#define ENGLISH_ALPHABET_SIZE               26