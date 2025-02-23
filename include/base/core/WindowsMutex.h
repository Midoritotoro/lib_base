#pragma once 

#include <base/core/AbstractMutex.h>

#ifdef OS_WIN

namespace base {
	class Thread;
	class WindowsMutex: public AbstractMutex {
	public:
		WindowsMutex(not_null<Thread*> thread);
		~WindowsMutex();

		void setUnlockOnDelete(bool unlockOnDel) override;
		bool unlockOnDelete() const noexcept override;

		[[nodiscard]] bool lock() override;
		[[nodiscard]] bool unlock() override;

		[[nodiscard]] bool isLocked() const noexcept override;
	private:
		bool _unlockOnDelete = true;
		bool _isLocked = false;

		Thread* _thread = nullptr;
	};
}
#endif