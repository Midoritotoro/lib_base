#pragma once 

#include <base/core/thread/AbstractMutex.h>

#ifdef base_os_windows

#include <base/core/io/WindowsSmartHandle.h>


__BASE_THREAD_NAMESPACE_BEGIN

class Thread;
class WindowsThread;

class WindowsMutex: public AbstractMutex {
public:
	WindowsMutex(not_null<WindowsThread*> thread);
	~WindowsMutex();

	void setUnlockOnDelete(bool unlockOnDel) override;
	bool unlockOnDelete() const noexcept override;

	bool lock() override;
	bool unlock() override;

	bool isLocked() const noexcept override;
private:
	bool _unlockOnDelete = true;
	bool _isLocked = false;

	WindowsThread* _thread = nullptr;
	io::WindowsSmartHandle _handle;
};

__BASE_THREAD_NAMESPACE_END

#endif