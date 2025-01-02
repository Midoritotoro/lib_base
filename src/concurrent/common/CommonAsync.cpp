#include "CommonAsync.h"

#ifdef USE_COMMON_QUEUE

#include <exception>

namespace {

concurrent::queue *Queue/* = nullptr*/;
std::atomic<int> Counter/* = 0*/;
concurrent::details::main_queue_pointer Lifetime;

} // namespace

namespace concurrent::details {

void main_queue_pointer::grab() {
	auto counter = Counter.load(std::memory_order_acquire);
	while (true) {
		if (!counter) {
			return;
		} else if (Counter.compare_exchange_weak(counter, counter + 1)) {
			_pointer = Queue;
			return;
		}
	}
}

void main_queue_pointer::ungrab() {
	if (_pointer) {
		if (--Counter == 0) {
			delete _pointer;
		}
		_pointer = nullptr;
	}
}

void main_queue_pointer::create(main_queue_processor processor) {
	if (Counter.load(std::memory_order_acquire) != 0) {
		std::terminate();
	}
	Queue = new queue(processor);
	Counter.store(1, std::memory_order_release);
	_pointer = Queue;
}

} // namespace concurrent::details

namespace concurrent {

void init_main_queue(main_queue_processor processor) {
	Lifetime.create(processor);
}

} // namespace concurrent

#endif // USE_COMMON_QUEUE
