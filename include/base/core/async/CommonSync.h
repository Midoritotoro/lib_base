#pragma once

#include <base/core/async/WinapiSemaphore.h>

namespace base::concurrent {

template <typename Callable>
inline void sync(Callable &&callable) {
	semaphore waiter;

	async([&] {
		const auto guard = details::finally([&] { waiter.release(); });
		callable();
	});

	waiter.acquire();
}

} // namespace base::concurrent
