#include <base/core/WindowsThread.h>

#ifdef OS_WIN

namespace base {
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

        if (!SetThreadPriority(handle, prio)) {
            qErrnoWarning("QThread::setPriority: Failed to set thread priority");
        }
    }

    template <
        typename Function,
        typename ... Args>
    void WindowsThread::create(
        Function&& callback,
        Args&& ... args)
    {

    }
} // namespace base

#endif