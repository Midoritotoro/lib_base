#pragma once 

#include <base/core/AbstractMutex.h>


namespace base {
	class Thread;

	class Mutex {
	public:
		Mutex(not_null<Thread*> thread);
		~Mutex();

		[[nodiscard]] bool lock();
		[[nodiscard]] bool unlock();

		[[nodiscard]] bool isLocked() const noexcept;
	private:
		AbstractMutex* _impl;
	};
} // namespace base