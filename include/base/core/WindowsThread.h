#pragma once 

#include <base/core/AbstractThread.h>

#ifdef OS_WIN

#include <base/io/WindowsSmartHandle.h>
#include <base/core/AtomicInteger.h>


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
        const io::WindowsSmartHandle& handle() const noexcept;

        static [[nodiscard]] io::WindowsSmartHandle currentThreadHandle() noexcept;
        static [[nodiscard]] sizetype currentThreadId() noexcept;

        sizetype threadId() const noexcept override;
        void join();
            
        template <
            class Function,
            class ... Args>
        void start(
            Function&& _routine,
            Args&& ... args);
    private:
        [[nodiscard]] static int WinPriorityFromInternal(Priority _Priority);
        void checkWaitForSingleObject(DWORD waitForSingleObjectResult);

        Priority _priority;

        bool _isRunning = false;

        bool __terminateOnClose = true;
        io::WindowsSmartHandle _handle = nullptr;

        sizetype _stackSize = 0;
        AtomicInteger<sizetype> _threadId = 0;
	};
} // namespace base

#endif