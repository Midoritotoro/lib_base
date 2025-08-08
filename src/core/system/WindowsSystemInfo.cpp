#include <base/core/system/WindowsSystemInfo.h>


#if defined(base_os_windows)

__BASE_SYSTEM_NAMESPACE_BEGIN

int WindowsSystemInfo::GetCpuCount() {
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	return sysInfo.dwNumberOfProcessors;
}

__BASE_SYSTEM_NAMESPACE_END
#endif
