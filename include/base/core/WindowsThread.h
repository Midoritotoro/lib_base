#pragma once 

#include <base/core/AbstractThread.h>

#ifdef OS_WIN

namespace base {
	class WindowsThread: public AbstractThread {
	public:
        void setPriority(Priority priority) override;
        Priority priority() const noexcept override;

        bool isFinished() const noexcept override;
        bool isRunning() const noexcept override;

        template <
            typename Function,
            typename ... Args>
        void create(
            Function&& callback,
            Args&& ... args);
    private:
        Priority _priority;

	};
} // namespace base

#endif