#include <base/core/WindowsThread.h>

#include <src/core/ThreadsData.h>
#include <base/core/ThreadsConfig.h>


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

    void WindowsThread::start(Priority _Priority_) {
#if defined(CPP_MSVC) && !defined(_DLL)
        // -MT или -MTd 
        _handle = (HANDLE)_beginthreadex(
            NULL, _stackSize, WindowsThread::winThreadStartRoutine,
            this, CREATE_SUSPENDED, &_threadId);
#else
        // -MD или -MDd или сборка под MinGW

       _handle = CreateThread(
            nullptr, _stackSize,
            reinterpret_cast<LPTHREAD_START_ROUTINE>(WindowsThread::winThreadStartRoutine),
            this, CREATE_SUSPENDED, reinterpret_cast<LPDWORD>(&_threadId));
#endif

        if (!_handle) {
            printf("QThread::start: Failed to create thread");

            _isRunning = false;
            return;
        }

        int prio;
        _priority = _Priority_;

        switch (_Priority_) {
            case IdlePriority:
                prio = THREAD_PRIORITY_IDLE;
                break;

            case LowestPriority:
                prio = THREAD_PRIORITY_LOWEST;
                break;

            case LowPriority:
                prio = THREAD_PRIORITY_BELOW_NORMAL;
                break;

            case NormalPriority:
                prio = THREAD_PRIORITY_NORMAL;
                break;

            case HighPriority:
                prio = THREAD_PRIORITY_ABOVE_NORMAL;
                break;

            case HighestPriority:
                prio = THREAD_PRIORITY_HIGHEST;
                break;

            case TimeCriticalPriority:
                prio = THREAD_PRIORITY_TIME_CRITICAL;
                break;

            case InheritPriority:
            default:
                prio = GetThreadPriority(GetCurrentThread());
                break;
        }

        if (!SetThreadPriority(_handle, prio))
            printf("QThread::start: Failed to set thread priority\n");

        if (ResumeThread(_handle) == (DWORD)-1)
            printf("QThread::start: Failed to resume new thread\n");
    }

    void WindowsThread::start() {
        return start(_priority);
    }

    template <
        typename Function,
        typename ... Args>
    void WindowsThread::create(
        Function&& callback,
        Args&& ... args)
    {
      

    }


    void WindowsThread::checkWaitForSingleObject(DWORD waitForSingleObjectResult) {
        ThreadsAssert(waitForSingleObjectResult != WAIT_FAILED, "base::threads::WindowsThread::join: Ошибка при ожидании выполнения потока", unused(0));
    }

    void STDCALL WindowsThread::winThreadStartRoutine() noexcept {
        
    }

    template <
        class _Tuple,
        size_t... _Indices>
    CDECL unsigned int WindowsThread::Invoke(void* _RawVals)
    {
        const std::unique_ptr<_Tuple> _FnVals(static_cast<_Tuple*>(_RawVals));

        _Tuple& _Tup = *_FnVals.get(); // avoid ADL, handle incomplete types
        std::invoke(std::move(std::get<_Indices>(_Tup))...);

        return 0;
    }


    template <
        class _Tuple,
        size_t... _Indices>
    constexpr auto WindowsThread::GetInvoke(std::index_sequence<_Indices...>) {
        return &Invoke<_Tuple, _Indices...>;
    }

    template <
        class _Fn,
        class... _Args>
    void __Start(
        _Fn&& _Fx,
        _Args&& ... _Ax) noexcept
    {
        using _Tuple = std::tuple<
            std::decay_t<_Fn>, 
            std::decay_t<_Args>...>;

        auto _Decay_copied = std::make_unique<_Tuple>(
            std::forward<_Fn>(_Fx),
            std::forward<_Args>(_Ax)...);

        constexpr auto _Invoker_proc = GetInvoke<_Tuple>(
            std::make_index_sequence<1 + sizeof...(_Args)>{});
    }
} // namespace base

#endif