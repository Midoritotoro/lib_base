#pragma once

#include <base/Platform.h>

#if defined _MSC_VER || defined __MINGW32__
#define USE_WINAPI_TIME
#endif // !_MSC_VER && !__MINGW32__

#if defined _MSC_VER

#if defined _WIN64
#define USE_WINAPI
#define WINAPI_X64
#elif defined _M_IX86 // _WIN64
#define USE_WINAPI
#define WINAPI_X86
//#define THROW_FP_EXCEPTIONS
#else // _M_IX86
#error "Configuration is not supported."
#endif // !_WIN64 && !_M_IX86

#define USE_WINAPI_LIST
#endif // _WIN64