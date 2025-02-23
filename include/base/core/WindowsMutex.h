#pragma once 

#include <base/core/AbstractMutex.h>

#ifdef OS_WIN

namespace base {
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
	};
}
#endif