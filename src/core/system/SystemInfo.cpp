#include <base/core/system/SystemInfo.h>


#if defined(base_os_windows)
	#include <base/core/system/WindowsSystemInfo.h>
	using PlatformSystemInfoImplementation = base::system::WindowsSystemInfo;
#elif defined(base_os_mac) || defined(base_os_linux)
	#include <base/core/system/UnixSystemInfo.h>
	using PlatformSystemInfoImplementation = base::system::UnixSystemInfo;
#endif


__BASE_SYSTEM_NAMESPACE_BEGIN

int SystemInfo::GetCpuCount() {
	return PlatformSystemInfoImplementation::GetCpuCount();
}

__BASE_SYSTEM_NAMESPACE_END
