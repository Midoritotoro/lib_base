#pragma once 

#include <base/core/AbstractMutex.h>

#ifdef OS_WIN

namespace base {
	class Thread;
	class WindowsMutex: public AbstractMutex {
	public:
		WindowsMutex(Thread* thread);
		~WindowsMutex();

		[[nodiscard]] bool lock() override;
		[[nodiscard]] bool unlock() override;

		[[nodiscard]] bool isLocked() const noexcept override;
	};
}
#endif