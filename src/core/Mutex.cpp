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
		_impl = new PlatformMutexImplementation(
			reinterpret_cast<ThreadPlatformImplementation*>(thread->impl()));
	}

	Mutex::~Mutex() {
		delete _impl;
		_impl = nullptr;
	}

	void Mutex::setUnlockOnDelete(bool unlockOnDel) {
		_impl->setUnlockOnDelete(unlockOnDel);
	}

	bool Mutex::unlockOnDelete() const noexcept {
		return _impl->unlockOnDelete();
	}

	bool Mutex::lock() {
		return _impl->lock();
	}

	bool Mutex::unlock() {
		return _impl->unlock();
	}

	bool Mutex::isLocked() const noexcept {
		return _impl->isLocked();
	}
} // namespace base