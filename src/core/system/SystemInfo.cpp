#include <base/core/system/SystemInfo.h>


#if defined(OS_WIN)
	#include <base/core/system/WindowsSystemInfo.h>
	using PlatformSystemInfoImplementation = base::system::WindowsSystemInfo;
#elif defined(OS_MAC) || defined(OS_LINUX)
	#include <base/core/system/UnixSystemInfo.h>
	using PlatformSystemInfoImplementation = base::system::UnixSystemInfo;
#endif


__BASE_SYSTEM_NAMESPACE_BEGIN

int SystemInfo::GetCpuCount() {
	return PlatformSystemInfoImplementation::GetCpuCount();
}

__BASE_SYSTEM_NAMESPACE_END
