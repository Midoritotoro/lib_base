#pragma once 

#include <base/system/Platform.h>
#include <base/utility/TypeTraits.h>

#include <base/io/WindowsSmartHandle.h>

#include <base/core/AtomicInteger.h>

#if defined(CPP_MSVC) && !defined(_DLL)
    #include <process.h> // Для _beginthreadex
#endif


namespace base {
    template <
        class _Tuple,
        size_t... _Indices>
    static inline CDECL uint Invoke(void* _RawVals)
    {
        const std::unique_ptr<_Tuple> 
            _FnVals(static_cast<_Tuple*>(_RawVals));

        _Tuple& _Tup = *_FnVals.get();
        std::invoke(std::move(
            std::get<_Indices>(_Tup))...);

        return 0;
    }

    template <
        class _Tuple,
        size_t... _Indices>
    static constexpr inline auto GetInvoke(std::index_sequence<_Indices...>) {
        return &Invoke<_Tuple, _Indices...>;
    }

    template <typename IdType>
    concept IsValidIdType = (sizeof(IdType) == sizeof(DWORD));

    template <
        class Function,
        class... Args,
        class IdType = sizetype,
        class = std::enable_if<
            std::is_integral_v<IdType>
            && IsValidIdType<IdType>>>
    void inline STDCALL StartImplementation(
        Function&& _Routine,
        Args&& ... _Args,
        AtomicInteger<IdType>* _PThreadId,
        io::WindowsSmartHandle* _PHandle) noexcept
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
        _PHandle = (HANDLE)reinterpret_cast<void*>(
            _beginthreadex(nullptr, 0, _Invoker_proc,
            _Decay_copied.get(), 0, &threadId));
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
    }
} // namespace base
