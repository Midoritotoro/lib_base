#pragma once


#include "base/Platform.h"

#ifdef _WIN32

#include <PathCch.h>
#include <tchar.h>

namespace base {
	[[nodiscard]] bool IsWindowsGreaterThen(int version);
	[[nodiscard]] bool SetAutoRunKey(LPWSTR path, LPWSTR key);

	[[nodiscard]] bool addToAutoRun(LPWSTR key);
} // namespace base

#endif // _WIN32

