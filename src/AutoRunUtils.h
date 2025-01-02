#pragma once

#ifdef _WIN32

#include "base/Platform.h"

#include <PathCch.h>
#include <tchar.h>

LIB_BASE [[nodiscard]] bool IsWindowsGreaterThen(int version);
LIB_BASE [[nodiscard]] bool SetAutoRunKey(LPWSTR path, LPWSTR key);

#define MINIMUM_WINDOWS_VERSION NTDDI_WIN10
#define IS_MINIMUM_WINDOWS_VERSION IsWindowsGreaterThen(MINIMUM_WINDOWS_VERSION)

#endif // _WIN32

LIB_BASE [[nodiscard]] bool addToAutoRun(LPWSTR key);
