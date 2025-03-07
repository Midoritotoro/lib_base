#include <base/core/thread/AtomicInteger.h>


#if defined(OS_WIN)
	#include <base/core/thread/WindowsAtomic.h>
	using PlatformAtomicImplementation = base::thread::WindowsAtomic;
#else
	#include<base/core/thread/UnixAtomic.h>
	using PlatformAtomicImplementation = base::thread::UnixAtomic;
#endif

__BASE_THREAD_NAMESPACE_BEGIN

__BASE_THREAD_NAMESPACE_END