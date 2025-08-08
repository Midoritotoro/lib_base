#pragma once

#include <base/core/arch/Platform.h>

#if defined base_cpp_msvc || defined __MINGW32__
#define USE_WINAPI_TIME
#endif // !_MSC_VER && !__MINGW32__

#if defined base_cpp_msvc

#if defined base_os_win64
#define USE_WINAPI
#define WINAPI_X64
#elif defined _M_IX86 // base_os_win64
#define USE_WINAPI
#define WINAPI_X86
//#define THROW_FP_EXCEPTIONS
#else // _M_IX86
#error "Configuration is not supported."
#endif // !_WIN64 && !_M_IX86

#define USE_WINAPI_LIST
#endif // base_os_win64