#pragma once 

#include <base/core/thread/AbstractMutex.h>


__BASE_THREAD_NAMESPACE_BEGIN

class Thread;

class Mutex {
public:
	Mutex(not_null<Thread*> thread);
	~Mutex();

	void setUnlockOnDelete(bool unlockOnDel);
	NODISCARD bool unlockOnDelete() const noexcept;

	NODISCARD bool lock();
	NODISCARD bool unlock();

	NODISCARD bool isLocked() const noexcept;
private:
	AbstractMutex* _impl;
};

__BASE_THREAD_NAMESPACE_END