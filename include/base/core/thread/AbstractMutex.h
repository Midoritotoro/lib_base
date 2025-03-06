#pragma once 

#include <base/core/arch/Platform.h>

__BASE_THREAD_NAMESPACE_BEGIN

class AbstractMutex {
public:
	virtual bool lock() = 0;
	virtual bool unlock() = 0;

	virtual void setUnlockOnDelete(bool unlockOnDel) = 0;
	virtual bool unlockOnDelete() const noexcept = 0;

	virtual bool isLocked() const noexcept = 0;
};

__BASE_THREAD_NAMESPACE_END
