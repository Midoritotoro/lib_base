#pragma once 

#include <base/core/thread/AbstractThread.h>
#include <base/core/thread/MutexLocker.h>

#ifdef OS_WIN

#include <base/core/thread/AtomicInteger.h>
#include <base/core/thread/WindowsThreadPrivate.h>


__BASE_THREAD_NAMESPACE_BEGIN

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

    static io::WindowsSmartHandle currentThreadHandle() noexcept;
    static sizetype currentThreadId() noexcept;

    void terminate() override;
    void close() override;

    sizetype threadId() const noexcept override;
    void join() override;

    bool joinable() const noexcept;
    static int HardwareConcurrency() noexcept;
         
    template <
        class Function,
        class ... Args>
    void start(
        Function&& _routine,
        Args&& ... args)
    {
        if (joinable())
            MutexLocker mutex(_mutex.get());

        _isRunning = true;
        const auto prio = WinPriorityFromInternal(_priority);

        WindowsThreadPrivate::StartImplementation(
            &_threadId,
            &_handle,
            std::forward<Function>(_routine),
            std::forward<Args>(args)...);

        if (!_handle.handle()) {
            printf("base::threads::WindowsThread::start: Failed to create thread");

            _isRunning = false;
            return;
        }

        if (!SetThreadPriority(_handle.handle(), prio))
            printf("base::threads::WindowsThread::start: Failed to set thread priority\n");

        if (ResumeThread(_handle.handle()) == (DWORD)-1)
            printf("base::threads::WindowsThread::start: Failed to resume new thread\n");
    }
private:
    static int WinPriorityFromInternal(Priority _Priority);
    void checkWaitForSingleObject(DWORD waitForSingleObjectResult);

    Priority _priority = Priority::NormalPriority;
    bool _isRunning = false;

    bool __terminateOnClose = false;
    io::WindowsSmartHandle _handle;

    std::unique_ptr<WindowsMutex> _mutex = nullptr;
    AtomicInteger<sizetype> _threadId = 0;
};

__BASE_THREAD_NAMESPACE_END

#endif