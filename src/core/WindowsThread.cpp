#include <base/core/WindowsThread.h>

#ifdef OS_WIN

#include <src/core/ThreadsData.h>
#include <base/core/ThreadsConfig.h>


namespace base {
    WindowsThread::WindowsThread():
        _mutex(std::make_unique<WindowsMutex>(this))
    {}

    WindowsThread::~WindowsThread() {
        if (__terminateOnClose)
            terminate();
        else
            close();
    }

    void WindowsThread::setPriority(Priority priority) {
        if (joinable())
            MutexLocker mutex(_mutex.get());

        int prio = WinPriorityFromInternal(priority);
        _priority = priority;

        ThreadsAssert(_handle.handle() != nullptr, "base::threads::WindowsThread::setPriority: Не удалось установить приоритет для потока с дескриптором nullptr. ", unused(0));

        if (!SetThreadPriority(_handle.handle(), prio))
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
        if (joinable())
            MutexLocker mutex(_mutex.get());

        _handle.setAutoDelete(!_terminateOnClose);
        __terminateOnClose = _terminateOnClose;
    }

    bool WindowsThread::terminateOnClose() const noexcept {
        return __terminateOnClose;
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

    bool WindowsThread::joinable() const noexcept {
        return (_threadId != 0);
    }

    void WindowsThread::join() {
        ThreadsAssert(joinable() != false, "base::threads::WindowsThread: Попытка вызвать join для несуществующего потока. ", unused(0));

        checkWaitForSingleObject(
            WaitForSingleObject(
                _handle.handle(), INFINITE));
    }

    void WindowsThread::terminate() {
        if (joinable())
            MutexLocker mutex(_mutex.get());

        _isRunning = false;

        const auto result = WindowsThreadPrivate::TerminateImplementation(&_handle);
        ThreadsAssert(result != 0, "base::threads::WindowsThread: Ошибка при попытке убить поток. ", unused(0));
    }

    void WindowsThread::close() {
        if (joinable())
            MutexLocker mutex(_mutex.get());

        _isRunning = false;
        WindowsThreadPrivate::CloseImplementation(&_handle);
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

            case WindowsThread::InheritPriority:
                return GetThreadPriority(GetCurrentThread());
        }

        AssertUnreachable();
        return EGENERIC;
    }
} // namespace base

#endif