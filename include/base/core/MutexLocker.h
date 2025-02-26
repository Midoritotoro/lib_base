#pragma once 

#include <base/core/Mutex.h>


namespace base {
	class Thread;

	class MutexLocker {
	public:
		MutexLocker(not_null<Mutex*> mutex);
		MutexLocker(not_null<AbstractMutex*> mutex);

		~MutexLocker();

		[[nodiscard]] bool isLocked() const noexcept;
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
} // namespace base