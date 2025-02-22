#include <base/core/Mutex.h>

#include <base/core/Thread.h>

#if defined(OS_WIN)
	#include <base/core/WindowsMutex.h>
	using PlatformMutexImplementation = base::WindowsMutex;
#elif defined(OS_MAC) || defined(OS_LINUX)
	#include <base/core/UnixMutex.h>
	using PlatformMutexImplementation = base::UnixMutex;
#endif


namespace base {
	Mutex::Mutex(not_null<Thread*> thread) {
		_impl = new PlatformMutexImplementation(thread);
	}

	Mutex::~Mutex() {
		if (isLocked())
			unlock();
	}

	bool Mutex::lock() {

	}

	bool Mutex::unlock() {

	}

	bool Mutex::isLocked() const noexcept {

	}
} // namespace base