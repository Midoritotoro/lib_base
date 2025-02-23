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
        const io::WindowsSmartHandle& handle() const noexcept;

        static [[nodiscard]] io::WindowsSmartHandle currentThreadHandle() noexcept;
        static [[nodiscard]] sizetype currentThreadId() noexcept;

        sizetype threadId() const noexcept override;
        void join();

        template <class _Fn, class... _Args>
        void start(_Fn&& _Fx, _Args&&... _Ax) {
            __Start(std::forward<_Fn>(_Fx), std::forward<_Args>(_Ax)...);
        }

        void start();
        void start(Priority priority);

        template <
            typename Function,
            typename ... Args>
        void create(
            Function&& callback,
            Args&& ... args);
    private:
        template <
            class _Tuple,
            size_t... _Indices>
        [[nodiscard]] static CDECL unsigned int Invoke(void* _RawVals);

        template <
            class _Tuple,
            size_t... _Indices>
        [[nodiscard]] static constexpr auto GetInvoke(std::index_sequence<_Indices...>);

        template <
            class _Fn,
            class... _Args>
        void Start(
            _Fn&& _Fx,
            _Args&& ... _Ax) noexcept;

        void checkWaitForSingleObject(DWORD waitForSingleObjectResult);
        static void STDCALL winThreadStartRoutine() noexcept;

        Priority _priority;

        bool _isRunning = false;

        bool __terminateOnClose = true;
        io::WindowsSmartHandle _handle = nullptr;

        sizetype _stackSize = 0;
        sizetype _threadId = 0;
	};
} // namespace base

#endif