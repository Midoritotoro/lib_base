#include <base/core/WindowsThread.h>

#ifdef OS_WIN

#include <src/core/ThreadsData.h>
#include <base/core/ThreadsConfig.h>

#include <base/core/WindowsMutex.h>
#include <base/core/WindowsThreadsHelpers.h>


namespace base {
    WindowsThread::~WindowsThread() {
        if (__terminateOnClose) {

        }
    }

    void WindowsThread::setPriority(Priority priority) {
        int prio = WinPriorityFromInternal(priority);
        _priority = priority;

        ThreadsAssert(_handle != nullptr, "base::threads::WindowsThread::setPriority: Не удалось установить приоритет для потока с дескриптором nullptr. ", unused(0));

        if (!SetThreadPriority(_handle, prio))
            printf("base::Thread::setPriority: Не удалось установить приоритет потока\n");
    }

    AbstractThread::Priority WindowsThread::priority() const noexcept {
        return _priority;
    }

    bool WindowsThread::isFinished() const noexcept {
        return !_isRunning;
    }

    bool WindowsThread::isRunning() const noexcept {
        return _isRunning;
    }

    io::WindowsSmartHandle WindowsThread::currentThreadHandle() noexcept {
        return GetCurrentThread();
    }

    sizetype WindowsThread::currentThreadId() noexcept {
        return GetCurrentThreadId();
    }

    void WindowsThread::setTerminateOnClose(bool _terminateOnClose) {
        WindowsMutex mutex(this);
        mutex.lock();

        __terminateOnClose = _terminateOnClose;
    }

    bool WindowsThread::terminateOnClose() const noexcept {
        return __terminateOnClose;
    }

    void WindowsThread::setStackSize(sizetype size) {
        WindowsMutex mutex(this);
        mutex.lock();

        _stackSize = size;
    }

    sizetype WindowsThread::stackSize() const noexcept {
        return _stackSize;
    }

    void WindowsThread::waitMs(sizetype milliseconds) {
        Sleep(milliseconds);
    }

    const io::WindowsSmartHandle& WindowsThread::handle() const noexcept {
        return _handle;
    }

    sizetype WindowsThread::threadId() const noexcept {
        return _threadId;
    }

    void WindowsThread::join() {
        ThreadsAssert(_handle != nullptr, "base::threads::WindowsThread: Попытка вызвать join для несуществующего потока. ", unused(0));
        ThreadsAssert(_isRunning != false, "base::threads::WindowsThread: Попытка вызвать join для неактивного потока. ", unused(0));

        checkWaitForSingleObject(WaitForSingleObject(_handle, INFINITE));
    }

    template <
        class Function,
        class ... Args>
    void WindowsThread::start(
        Function&& _routine,
        Args&& ... args)
    {
        std::cout << "WindowsThread::start called";
        const auto prio = WinPriorityFromInternal(priority);

        StartImplementation(
            std::forward<Function>(_routine),
            std::forward<Args>(args)...,
            _threadId,
            _handle);

        if (!_handle) {
            printf("QThread::start: Failed to create thread");

            _isRunning = false;
            return;
        }

        if (!SetThreadPriority(_handle, prio))
            printf("base::threads::WindowsThread::start: Failed to set thread priority\n");

        if (ResumeThread(_handle) == (DWORD)-1)
            printf("base::threads::WindowsThread::start: Failed to resume new thread\n");
    }

    void WindowsThread::checkWaitForSingleObject(DWORD waitForSingleObjectResult) {
        ThreadsAssert(waitForSingleObjectResult != WAIT_FAILED, "base::threads::WindowsThread::join: Ошибка при ожидании выполнения потока", unused(0));
    }

    int WindowsThread::WinPriorityFromInternal(Priority _Priority) {
        switch (_Priority) {
            case WindowsThread::IdlePriority:
                return THREAD_PRIORITY_IDLE;

            case WindowsThread::LowestPriority:
                return THREAD_PRIORITY_LOWEST;

            case WindowsThread::LowPriority:
                return THREAD_PRIORITY_BELOW_NORMAL;

            case WindowsThread::NormalPriority:
                return THREAD_PRIORITY_NORMAL;

            case WindowsThread::HighPriority:
                return THREAD_PRIORITY_ABOVE_NORMAL;

            case WindowsThread::HighestPriority:
                return THREAD_PRIORITY_HIGHEST;

            case WindowsThread::TimeCriticalPriority:
                return THREAD_PRIORITY_TIME_CRITICAL;
        }

        return EGENERIC;
    }
} // namespace base

#endif