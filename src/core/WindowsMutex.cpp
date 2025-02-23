#include <base/core/WindowsMutex.h>


namespace base {
	WindowsMutex::WindowsMutex(not_null<WindowsThread*> thread):
		_thread(thread)
	{}

	WindowsMutex::~WindowsMutex() {
		if (_unlockOnDelete)
			unlock();
	}

	bool WindowsMutex::lock() {
		_isLocked = true;
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
		return _isLocked;
	}

	bool WindowsMutex::isLocked() const noexcept {
		return _isLocked;
	}
} // namespace base