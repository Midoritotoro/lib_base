#pragma once 

#include <base/system/Platform.h>
#include <base/utility/TypeTraits.h>

#include <memory>

#include <base/io/WindowsSmartHandle.h>

#include <base/core/AtomicInteger.h>

#if defined(CPP_MSVC) && !defined(_DLL)
#include <process.h> // Для _beginthreadex
#endif


namespace base {
    template <
        class _Tuple,
        size_t... _Indices>
    NODISCARD uint STDCALL Invoke(void* _RawVals) noexcept
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
    NODISCARD constexpr auto GetInvoke(std::index_sequence<_Indices...>) noexcept {
        return &Invoke<_Tuple, _Indices...>;
    }

    template <class IdType>
    concept IsValidIdType = 
        (sizeof(IdType) >= sizeof(DWORD))
        && std::is_integral_v<IdType>;

    template <
        class Function,
        class... Args,
        class IdType = sizetype,
        class = std::enable_if<IsValidIdType<IdType>>>
        void STDCALL StartImplementation(
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

        if (LIKELY(_PHandle != nullptr))
            unused(decayCopied.release());
        else // failed to start thread
            _PThreadId->storeSequentiallyConsistent(0);
    }
} // namespace base