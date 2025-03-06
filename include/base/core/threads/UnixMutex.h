#pragma once 

#include <base/core/AbstractMutex.h>

#if defined(OS_LINUX) || defined(OS_MAC)

namespace base {
	class Thread;
	class UnixMutex : public AbstractMutex {
	public:
		UnixMutex(Thread* thread);
		~UnixMutex();

		[[nodiscard]] bool lock() override;
		[[nodiscard]] bool unlock() override;

		[[nodiscard]] bool isLocked() const noexcept override;
	};
}
#endif