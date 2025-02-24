#include <base/system/SystemInfo.h>


#if defined(OS_WIN)
	#include <base/system/WindowsSystemInfo.h>
	using PlatformSystemInfoImplementation = base::system::WindowsSystemInfo;
#elif defined(OS_MAC) || defined(OS_LINUX)
	#include <base/system/UnixSystemInfo.h>
	using PlatformSystemInfoImplementation = base::system::UnixSystemInfo;
#endif


namespace base::system {
	int SystemInfo::GetCpuCount() {
		return PlatformSystemInfoImplementation::GetCpuCount();
	}
} // namespace base::system
