#include <base/system/WindowsSystemInfo.h>


#if defined(OS_WIN)

namespace base::system {
	int WindowsSystemInfo::GetCpuCount() {
		SYSTEM_INFO sysInfo;
		GetSystemInfo(&sysInfo);

		return sysInfo.dwNumberOfProcessors;
	}
} // namespace base::system

#endif
