#pragma once

#include <base/system/Platform.h>


#if defined(OS_WIN)

namespace base::system {
	class WindowsSystemInfo {
	public:
		static [[nodiscard]] int GetCpuCount();
	};
} // namespace base::system

#endif
