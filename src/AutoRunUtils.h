#pragma once

#ifdef _WIN32

#include "Windows.h"
#include "Common.h"

#include <tchar.h>

// #include <pathcch.h>
// #pragma comment(lib, "Pathcch.lib")

LIB_BASE [[nodiscard]] bool IsWindowsGreaterThen(int version);
LIB_BASE [[nodiscard]] bool SetAutoRunKey(LPWSTR path);

#define MINIMUM_WINDOWS_VERSION NTDDI_WIN10
#define IS_MINIMUM_WINDOWS_VERSION IsWindowsGreaterThen(MINIMUM_WINDOWS_VERSION)

#endif // _WIN32

LIB_BASE [[nodiscard]] bool addToAutoRun();
