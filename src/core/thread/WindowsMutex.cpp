#include <base/core/thread/WindowsMutex.h>


#ifdef OS_WIN

#include <base/core/thread/ThreadsConfig.h>


__BASE_THREAD_NAMESPACE_BEGIN

WindowsMutex::WindowsMutex(not_null<WindowsThread*> thread):
	_thread(thread)
{
	_handle = CreateMutexA(
		nullptr, false, nullptr);

	AssertLog(_handle.handle() != nullptr, "base::thread::WindowsMutex: Не удалось создать мьютекс. ");
}

WindowsMutex::~WindowsMutex() {
	if (_unlockOnDelete)
		unlock();
}

bool WindowsMutex::lock() {
	_isLocked = true;
	WaitForSingleObject(_handle.handle(), INFINITE);

	return _isLocked;
}

void WindowsMutex::setUnlockOnDelete(bool unlockOnDel) {
	_unlockOnDelete = unlockOnDel;
}

bool WindowsMutex::unlockOnDelete() const noexcept {
	return _unlockOnDelete;
}

bool WindowsMutex::unlock() {
	_isLocked = false;

	ReleaseMutex(_handle.handle());
	return _isLocked;
}

bool WindowsMutex::isLocked() const noexcept {
	return _isLocked;
}

__BASE_THREAD_NAMESPACE_END

#endif