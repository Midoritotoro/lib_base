#pragma once

#include <base/core/arch/Platform.h>


#if defined(OS_WIN)

__BASE_SYSTEM_NAMESPACE_BEGIN

class WindowsSystemInfo {
public:
	static NODISCARD int GetCpuCount();
};

__BASE_SYSTEM_NAMESPACE_END

#endif
