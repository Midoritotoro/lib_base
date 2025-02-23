//#include <tuple>
//#include <iostream>
//
//#include <utility>
//#include <type_traits> 
//
//#include <memory> 
//
//#if defined(_WIN32) || defined(_WIN64)
//#include <Windows.h>
//#endif
//
//#ifndef CDECL 
//#  if defined(_WIN32) || defined(_WIN64)
//#    define CDECL __cdecl
//#  else
//#  define CDECL
//#  endif
//#endif
//
//
//class Thread {
//    template <
//        class _Tuple,
//        size_t... _Indices>
//    [[nodiscard]] static CDECL unsigned int __InvokeMethod(void* _RawVals) {
//        const std::unique_ptr<_Tuple> _FnVals(static_cast<_Tuple*>(_RawVals));
//        _Tuple& _Tup = *_FnVals.get(); // avoid ADL, handle incomplete types
//        std::invoke(std::move(std::get<_Indices>(_Tup))...);
//        return 0;
//    }
//
//
//    template <
//        class _Tuple,
//        size_t... _Indices>
//    [[nodiscard]] static constexpr auto __Get_Invoke(std::index_sequence<_Indices...>) {
//        return &__InvokeMethod<_Tuple, _Indices...>;
//    }
//
//    template <
//        class _Fn,
//        class... _Args>
//    void __Start(
//        _Fn&& _Fx,
//        _Args&& ... _Ax) noexcept
//    {
//        using _Tuple = std::tuple<std::decay_t<_Fn>, std::decay_t<_Args>...>;
//        auto _Decay_copied = std::make_unique<_Tuple>(std::forward<_Fn>(_Fx), std::forward<_Args>(_Ax)...);
//        constexpr auto _Invoker_proc = __Get_Invoke<_Tuple>(std::make_index_sequence<1 + sizeof...(_Args)>{});
//
//        _Invoker_proc(_Decay_copied.get());
//
//    }
//
//public:
//    Thread()
//    {}
//
//    template <class _Fn, class... _Args>
//    explicit Thread(_Fn&& _Fx, _Args&&... _Ax) {
//        __Start(std::forward<_Fn>(_Fx), std::forward<_Args>(_Ax)...);
//    }
//
//    ~Thread() {
//    }
//private:
//
//
//};
