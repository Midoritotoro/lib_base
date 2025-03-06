#pragma once 

#include <base/core/thread/ThreadsConfig.h>

#ifdef OS_WIN 

#include <base/core/thread/AtomicInteger.h>


#if defined(CPP_MSVC) && !defined(_DLL)
    #include <process.h> // Для _beginthreadex и _endthreadex
#else
    WARNING_DISABLE_MSVC(6258) // Использование TerminateThread не разрешает правильную очистку потока.
#endif


__BASE_THREAD_NAMESPACE_BEGIN

template <class IdType>
concept IsValidIdType =
    (sizeof(IdType) >= sizeof(DWORD))
    && std::is_integral_v<IdType>;

class WindowsThreadPrivate {
public:
        template <
        class _Tuple,
        size_t... _Indices>
    static NODISCARD uint
        STDCALL Invoke(void* _RawVals) noexcept
    {
        const std::unique_ptr<_Tuple>
            _FnVals(static_cast<_Tuple*>(_RawVals));

        _Tuple& _Tup = *_FnVals.get();

        std::invoke(std::move(
            std::get<_Indices>(_Tup))...);

#if defined(CPP_MSVC) && !defined(_DLL)
        _Cnd_do_broadcast_at_thread_exit();
#endif

        return 0;
    }

    template <
        class _Tuple,
        size_t... _Indices>
    static NODISCARD constexpr
        auto GetInvoke(std::index_sequence<_Indices...>) noexcept 
    {
        return &Invoke<_Tuple, _Indices...>;
    }

    template <
        class Function,
        class... Args,
        class IdType = sizetype,
        class = std::enable_if<IsValidIdType<IdType>>>
    static void STDCALL StartImplementation(
        AtomicInteger<IdType>* _PThreadId,
        io::WindowsSmartHandle* _PHandle,
        Function&& _Routine,
        Args&& ... _Args) noexcept
    {
        using _Tuple = std::tuple<
            std::decay_t<Function>,
            std::decay_t<Args>...>;

        auto decayCopied = std::make_unique<_Tuple>(
            std::forward<Function>(_Routine),
            std::forward<Args>(_Args)...);

        constexpr auto invokerProc = GetInvoke<_Tuple>(
            std::make_index_sequence<1 + sizeof...(Args)>{});

        IdType threadId;

#if defined(CPP_MSVC) && !defined(_DLL)
        // -MT или -MTd 

        *_PHandle = (HANDLE)reinterpret_cast<void*>(
            _beginthreadex(nullptr, 0, invokerProc,
                decayCopied.get(), 0, 
                reinterpret_cast<uint*>(&threadId)));
#else
        // -MD или -MDd или сборка под MinGW

        *_PHandle = CreateThread(
            nullptr, 0, // default stack size
            reinterpret_cast<LPTHREAD_START_ROUTINE>(invokerProc),
            reinterpret_cast<LPVOID>(decayCopied.get()),
            CREATE_SUSPENDED,
            reinterpret_cast<LPDWORD>(&threadId));

        _PThreadId->storeSequentiallyConsistent(threadId);
#endif

        if (LIKELY(_PHandle->handle() != nullptr))
            unused(decayCopied.release()); // Управление перешло к потоку
        else // Не удалось создать поток
            _PThreadId->storeSequentiallyConsistent(0);
    }

    static NODISCARD DWORD GetThreadExitCode(const io::WindowsSmartHandle& handle) {
        DWORD exitCode = 0;
        GetExitCodeThread(handle.handle(), &exitCode);

        return exitCode;
    }

    static void STDCALL CloseImplementation(
        io::WindowsSmartHandle* _PHandle) noexcept
    {
#if defined(CPP_MSVC) && !defined(_DLL)
        // -MT или -MTd 
        _endthreadex(GetThreadExitCode(_PHandle->handle()));

        _PHandle->setAutoDelete(false);
        CloseHandle(_PHandle->handle());
#else
        // -MD или -MDd или сборка под MinGW
        ExitThread(GetThreadExitCode(_PHandle->handle()));
#endif
    }   

    static NODISCARD DWORD 
        STDCALL TerminateImplementation(
            io::WindowsSmartHandle* _PHandle) noexcept
    {
#if defined(CPP_MSVC) && !defined(_DLL)
        // -MT или -MTd 
        CloseImplementation(_PHandle);
        return true;
#else
        // -MD или -MDd или сборка под MinGW

        return TerminateThread(
            _PHandle->handle(),
            GetThreadExitCode(_PHandle));
#endif

        return false;
    }
};

__BASE_THREAD_NAMESPACE_END

#endif