#pragma once

#include "base/concurrent/common/CommonConfig.h"

#include "base/concurrent/win/WinapiList.h"
#include "base/concurrent/common/CommonUtils.h"

#include <atomic>

#ifndef USE_COMMON_QUEUE
#define USE_COMMON_QUEUE
#endif // !USE_COMMON_QUEUE

namespace base::concurrent {
	namespace details {
		class main_queue_pointer;
	} // namespace details

class queue {
public:
	queue();
	queue(const queue &other) = delete;
	queue &operator=(const queue &other) = delete;

	template <typename Callable>
	void async(Callable &&callable) {
		if (_list.push_is_first(std::forward<Callable>(callable)))
			wake_async();
	}

	template <typename Callable>
	void sync(Callable &&callable) {
		semaphore waiter;

		async([&] {
			const auto guard = details::finally([&] { waiter.release(); });
			callable();
		});

		waiter.acquire();
	}

private:
	friend class details::main_queue_pointer;
	static void ProcessCallback(void *that);

	queue(main_queue_processor processor);

	void wake_async();
	void process();

	main_queue_processor _main_processor = nullptr;
	details::list _list;
	std::atomic_flag _queued = ATOMIC_FLAG_INIT;
};

} // namespace base::concurrent
