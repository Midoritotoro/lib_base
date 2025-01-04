#pragma once

#include "../common/CommonConfig.h"

#ifdef USE_WINAPI

#include "../common/CommonUtils.h"
#include "../common/CommonSync.h"

#include <type_traits>

namespace base::concurrent::details {
	void async_plain(void (*callable)(void*), void *argument);
} // namespace base::concurrent::details

namespace base::concurrent {
	template <
		typename Callable,
		typename Return = decltype(std::declval<Callable>()())>

	inline void async(Callable &&callable) {
		using Function = std::decay_t<Callable>;

		if constexpr (details::is_plain_function_v<Function, Return>) {
			using Plain = Return(*)();

			const auto copy = static_cast<Plain>(callable);
			details::async_plain([](void *passed) {
				const auto callable = reinterpret_cast<Plain>(passed);
				(*callable)();
			}, reinterpret_cast<void*>(copy));
		} 
		else {
			const auto copy = new Function(std::forward<Callable>(callable));

			details::async_plain([](void *passed) {
				const auto callable = static_cast<Function*>(passed);
				const auto guard = details::finally([=] { delete callable; });
				(*callable)();
			}, static_cast<void*>(copy));
		}
	}
} // namespace base::concurrent

#endif // USE_WINAPI