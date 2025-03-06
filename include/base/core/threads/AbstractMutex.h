#pragma once 

#include <base/system/Platform.h>

namespace base {
	class AbstractMutex {
	public:
		virtual bool lock() = 0;
		virtual bool unlock() = 0;

		virtual void setUnlockOnDelete(bool unlockOnDel) = 0;
		virtual bool unlockOnDelete() const noexcept = 0;

		virtual bool isLocked() const noexcept = 0;
	};
}