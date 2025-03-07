#include <base/core/thread/Mutex.h>

#include <base/core/thread/Thread.h>

#if defined(OS_WIN)
	#include <base/core/thread/WindowsMutex.h>
	using PlatformMutexImplementation = base::thread::WindowsMutex;
#elif defined(OS_MAC) || defined(OS_LINUX)
	#include <base/core/thread/UnixMutex.h>
	using PlatformMutexImplementation = base::thread::UnixMutex;
#endif


__BASE_THREAD_NAMESPACE_BEGIN

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

__BASE_THREAD_NAMESPACE_END
