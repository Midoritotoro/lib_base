#pragma once

#include <base/core/arch/Platform.h>


#if defined(base_os_windows)

__BASE_SYSTEM_NAMESPACE_BEGIN

class WindowsSystemInfo {
public:
	static   int GetCpuCount();
};

__BASE_SYSTEM_NAMESPACE_END

#endif
