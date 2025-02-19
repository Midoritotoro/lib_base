#include <base/core/AtomicInteger.h>


#if defined(OS_WIN)
	#include <base/core/WindowsAtomic.h>
	using PlatformAtomicImplementation = base::WindowsAtomic;
#else
	#include<base/core/UnixAtomic.h>
	using PlatformAtomicImplementation = base::UnixAtomic;
#endif

namespace base {
    
} // namespace base