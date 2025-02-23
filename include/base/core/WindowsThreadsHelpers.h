#pragma once 

#include <base/system/Platform.h>
#include <base/utility/TypeTraits.h>

#include <base/io/WindowsSmartHandle.h>

#if defined(CPP_MSVC) && !defined(_DLL)
    #include <process.h> // Для _beginthreadex
#endif


namespace base {
    template <
        class _Tuple,
        size_t... _Indices>
    static CDECL uint Invoke(void* _RawVals)
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
    static constexpr auto GetInvoke(std::index_sequence<_Indices...>) {
        return &Invoke<_Tuple, _Indices...>;
    }

    template <
        class Function,
        class... Args>
    void STDCALL StartImplementation(
        Function&& _Routine,
        Args&& ... _Args,
        sizetype* _PThreadId,
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

#if defined(CPP_MSVC) && !defined(_DLL)
        // -MT или -MTd 
        _PHandle = (HANDLE)reinterpret_cast<void*>(
            _beginthreadex(nullptr, 0, _Invoker_proc,
            _Decay_copied.get(), 0, _PThreadId));
#else
        // -MD или -MDd или сборка под MinGW

        _PHandle = CreateThread(
            nullptr, 0, // default stack sizes
            reinterpret_cast<LPTHREAD_START_ROUTINE>(invokerProc),
            decayCopied, CREATE_SUSPENDED, 
            reinterpret_cast<LPDWORD>(_PThreadId));
#endif
    }
} // namespace base
