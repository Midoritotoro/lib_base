#pragma once 

#include <base/core/thread/AbstractMutex.h>
#include <base/core/utility/NotNull.h>

__BASE_THREAD_NAMESPACE_BEGIN

class Thread;

class Mutex {
public:
	Mutex(not_null<Thread*> thread);
	~Mutex();

	void setUnlockOnDelete(bool unlockOnDel);
	base_nodiscard bool unlockOnDelete() const noexcept;

	base_nodiscard bool lock();
	base_nodiscard bool unlock();

	base_nodiscard bool isLocked() const noexcept;
private:
	AbstractMutex* _impl;
};

__BASE_THREAD_NAMESPACE_END