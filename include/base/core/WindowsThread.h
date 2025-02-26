#pragma once 

#include <base/core/AbstractThread.h>

#ifdef OS_WIN

#include <base/io/WindowsSmartHandle.h>

#include <base/core/WindowsThreadsHelpers.h>
#include <base/core/AtomicInteger.h>

#include <base/core/WindowsMutex.h>


namespace base {
    class WindowsThread final :
        public AbstractThread
    {

    public:
        WindowsThread();
        ~WindowsThread();

        void setPriority(Priority priority) override;
        Priority priority() const noexcept override;

        bool isFinished() const noexcept override;
        bool isRunning() const noexcept override;

        void setTerminateOnClose(bool _terminateOnClose) override;
        bool terminateOnClose() const noexcept override;

        void waitMs(sizetype milliseconds) override;
        const io::WindowsSmartHandle& handle() const noexcept;

        static [[nodiscard]] io::WindowsSmartHandle currentThreadHandle() noexcept;
        static [[nodiscard]] sizetype currentThreadId() noexcept;

        void terminate();

        sizetype threadId() const noexcept override;
        void join();

        template <
            class Function,
            class ... Args>
        void start(
            Function&& _routine,
            Args&& ... args)
        {
            WindowsMutex mutex(this);
            mutex.lock();

            _isRunning = true;

            std::cout << "WindowsThread::start called";
            const auto prio = WinPriorityFromInternal(_priority);

            StartImplementation(
                &_threadId,
                &_handle,
                std::forward<Function>(_routine),
                std::forward<Args>(args)...);

            if (!_handle.handle()) {
                printf("base::threads::WindowsThread::start: Failed to create thread");

                _isRunning = false;
                return;
            }

            if (!SetThreadPriority(_handle, prio))
                printf("base::threads::WindowsThread::start: Failed to set thread priority\n");

            if (ResumeThread(_handle) == (DWORD)-1)
                printf("base::threads::WindowsThread::start: Failed to resume new thread\n");
        }
    private:
        [[nodiscard]] static int WinPriorityFromInternal(Priority _Priority);
        void checkWaitForSingleObject(DWORD waitForSingleObjectResult);

        static BOOL STDCALL CustomTerminate(HANDLE handle);

        Priority _priority = Priority::NormalPriority;
        bool _isRunning = false;

        bool __terminateOnClose = true;
        io::WindowsSmartHandle _handle = nullptr;

        AtomicInteger<sizetype> _threadId = 0;
    };
} // namespace base

#endif