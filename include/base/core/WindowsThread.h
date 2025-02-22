#pragma once 

#include <base/core/AbstractThread.h>

#ifdef OS_WIN

#include <base/io/WindowsSmartHandle.h>


namespace base {
	class WindowsThread final :
        public AbstractThread
    {
	public:
        ~WindowsThread();

        void setPriority(Priority priority) override;
        Priority priority() const noexcept override;

        bool isFinished() const noexcept override;
        bool isRunning() const noexcept override;

        void setTerminateOnClose(bool _terminateOnClose) override;
        bool terminateOnClose() const noexcept override;

        void setStackSize(sizetype size);
        sizetype stackSize() const noexcept;

        void waitMs(sizetype milliseconds) override;

        static [[nodiscard]] io::WindowsSmartHandle currentThreadHandle() noexcept;
        static [[nodiscard]] sizetype currentThreadId() noexcept;

        template <
            typename Function,
            typename ... Args>
        void create(
            Function&& callback,
            Args&& ... args);
    private:
        Priority _priority;

        bool _isRunning;

        bool __terminateOnClose;
        io::WindowsSmartHandle _handle;

        sizetype _stackSize = 0;
	};
} // namespace base

#endif