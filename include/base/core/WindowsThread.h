#pragma once 

#include <base/system/Platform.h>

#ifdef OS_WIN

namespace base {
	class WindowsThread {
	public:
        void setPriority(Priority priority);
        Priority priority() const;
		[[nodiscard]] bool isRunning() const noexcept;
		[[nodiscard]] bool isFinished() const noexcept;

	};
} // namespace base

#endif