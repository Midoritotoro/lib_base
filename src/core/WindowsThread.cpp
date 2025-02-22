#include <base/core/WindowsThread.h>

#include <src/core/ThreadsData.h>


#ifdef OS_WIN

namespace base {
    WindowsThread::~WindowsThread() {
        if (__terminateOnClose) {

        }
    }

    void WindowsThread::setPriority(Priority priority) {
        int prio;
        _priority = priority;

        switch (priority) {
            case WindowsThread::IdlePriority:
                prio = THREAD_PRIORITY_IDLE;
                break;

            case WindowsThread::LowestPriority:
                prio = THREAD_PRIORITY_LOWEST;
                break;

            case WindowsThread::LowPriority:
                prio = THREAD_PRIORITY_BELOW_NORMAL;
                break;

            case WindowsThread::NormalPriority:
                prio = THREAD_PRIORITY_NORMAL;
                break;

            case WindowsThread::HighPriority:
                prio = THREAD_PRIORITY_ABOVE_NORMAL;
                break;

            case WindowsThread::HighestPriority:
                prio = THREAD_PRIORITY_HIGHEST;
                break;

            case WindowsThread::TimeCriticalPriority:
                prio = THREAD_PRIORITY_TIME_CRITICAL;
                break;

            default:
                return;
        }

        if (!SetThreadPriority(_handle, prio))
            printf("base::Thread::setPriority: Ќе удалось установить приоритет потока\n");
    }

    AbstractThread::Priority WindowsThread::priority() const noexcept {
        return _priority;
    }

    bool WindowsThread::isFinished() const noexcept {

    }

    bool WindowsThread::isRunning() const noexcept {
        
    }

    io::WindowsSmartHandle WindowsThread::currentThreadHandle() noexcept {
        return io::WindowsSmartHandle();
    }

    sizetype WindowsThread::currentThreadId() noexcept {
        return GetCurrentThread();
    }

    void WindowsThread::setTerminateOnClose(bool _terminateOnClose) {
        __terminateOnClose = _terminateOnClose;
    }

    bool WindowsThread::terminateOnClose() const noexcept {
        return __terminateOnClose;
    }

    void WindowsThread::setStackSize(sizetype size) {
        _stackSize = size;
    }

    sizetype WindowsThread::stackSize() const noexcept {
        return _stackSize;
    }


    void WindowsThread::waitMs(sizetype milliseconds) {
        Sleep(milliseconds);
    }

    template <
        typename Function,
        typename ... Args>
    void WindowsThread::create(
        Function&& callback,
        Args&& ... args)
    {
        _handle = CreateThread(
            NULL, 
            _stackSize, std::move(callback),
            std::move(args), CREATE_SUSPENDED,
            NULL);
    }
} // namespace base

#endif