#pragma once 

#include <base/core/thread/Mutex.h>


__BASE_THREAD_NAMESPACE_BEGIN

class Thread;

class MutexLocker {
public:
	MutexLocker(not_null<Mutex*> mutex);
	MutexLocker(not_null<AbstractMutex*> mutex);

	~MutexLocker();

	base_nodiscard bool isLocked() const noexcept;
private:
	enum {
		PlatformMutex,
		GeneralMutex
	} _tag;

	union {
		Mutex* _mutex;
		AbstractMutex* _abstractMutex;
	};
};

__BASE_THREAD_NAMESPACE_END