#include "base/concurrent/common/CommonQueue.h"

#ifdef USE_COMMON_QUEUE

#include "base/concurrent/win/WinapiAsync.h"


namespace base::concurrent {
	queue::queue() = default;
	queue::queue(main_queue_processor processor)
		: _main_processor(processor) 
	{}

	void queue::wake_async() {
		if (!_queued.test_and_set())
			(_main_processor 
				? _main_processor 
				: details::async_plain)
			(
				ProcessCallback,
				static_cast<void*>(this)
			);
	}

	void queue::process() {
		if (!_list.process())
			return;

		_queued.clear();

		if (!_list.empty())
			wake_async();
	}

	void queue::ProcessCallback(void *that) {
		static_cast<queue*>(that)->process();
	}
} // namespace base::concurrent

#endif // USE_COMMON_QUEUE
