#include <base/core/thread/AtomicInteger.h>


#if defined(base_os_windows)
	#include <base/core/thread/WindowsAtomic.h>
	using PlatformAtomicImplementation = base::thread::WindowsAtomic;
#elif defined(base_os_mac) || defined(base_os_linux)
	#include<base/core/thread/UnixAtomic.h>
	using PlatformAtomicImplementation = base::thread::UnixAtomic;
#endif

__BASE_THREAD_NAMESPACE_BEGIN

__BASE_THREAD_NAMESPACE_END