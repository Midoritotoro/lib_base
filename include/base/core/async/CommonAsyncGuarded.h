#pragma once

#include <base/core/async/CommonConfig.h>
#include <base/core/async/CommonGuards.h>

#include <base/core/async/CommonAsync.h>

#include <memory>
#include <type_traits>

namespace base::concurrent {
namespace details {

template <typename T>
constexpr ::std::size_t dependent_zero = 0;

} // namespace details

template <typename T, typename Enable = void>
struct guard_traits;

template <typename Callable>
struct deduce_call_type_traits {
	using type = decltype(&Callable::operator());
};

template <typename Callable>
using deduced_call_type = typename deduce_call_type_traits<
	::std::decay_t<Callable>>::type;

template <typename Guard, typename Callable>
class guarded_wrap {
public:
	using ClearCallable = ::std::decay_t<Callable>;
	using GuardTraits = guard_traits<::std::decay_t<Guard>>;
	using GuardType = decltype(GuardTraits::create(::std::declval<Guard>()));

	guarded_wrap(Guard &&object, Callable &&callable)
	: _guard(GuardTraits::create(std::forward<Guard>(object)))
	, _callable(::std::forward<Callable>(callable)) {
	}

	template <
		typename ...OtherArgs,
		typename Return = decltype(
			::std::declval<ClearCallable>()(::std::declval<OtherArgs>()...))>
	Return operator()(OtherArgs &&...args) {
		return GuardTraits::check(_guard)
			? _callable(::std::forward<OtherArgs>(args)...)
			: Return();
	}

	template <
		typename ...OtherArgs,
		typename Return = decltype(
			std::declval<ClearCallable>()(std::declval<OtherArgs>()...))>
	Return operator()(OtherArgs &&...args) const {
		return GuardTraits::check(_guard)
			? _callable(std::forward<OtherArgs>(args)...)
			: Return();
	}

private:
	GuardType _guard;
	ClearCallable _callable;

};

template <typename Guard, typename Callable>
struct deduce_call_type_traits<guarded_wrap<Guard, Callable>> {
	using type = deduced_call_type<Callable>;
};

template <
	typename Guard,
	typename Callable,
	typename GuardTraits = guard_traits<::std::decay_t<Guard>>,
	typename = ::std::enable_if_t<
		sizeof(GuardTraits) != details::dependent_zero<GuardTraits>>>
inline auto guard(Guard &&object, Callable &&callable)
-> guarded_wrap<Guard, Callable> {
	return {
		::std::forward<Guard>(object),
		::std::forward<Callable>(callable)
	};
}

template <
	typename Guard,
	typename Callable,
	typename GuardTraits = guard_traits<::std::decay_t<Guard>>,
	typename = ::std::enable_if_t<
		sizeof(GuardTraits) != details::dependent_zero<GuardTraits>>>
inline void invokeAsync(Guard &&object, Callable &&callable) {
	return invokeAsync(guard(
		::std::forward<Guard>(object),
		::std::forward<Callable>(callable)));
}

template <
	typename Guard,
	typename Callable,
	typename GuardTraits = guard_traits<::std::decay_t<Guard>>,
	typename = ::std::enable_if_t<
		sizeof(GuardTraits) != details::dependent_zero<GuardTraits>>>
inline void invokeSync(Guard &&object, Callable &&callable) {
	return invokeSync(guard(
		::std::forward<Guard>(object),
		::std::forward<Callable>(callable)));
}

} // namespace base::concurrent
