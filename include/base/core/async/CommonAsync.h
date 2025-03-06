#pragma once

#include <base/concurrent/common/CommonConfig.h>

#include <base/concurrent/common/CommonQueue.h>
#include <atomic>


#ifndef USE_COMMON_QUEUE
#define USE_COMMON_QUEUE
#endif // !USE_COMMON_QUEUE


namespace base::concurrent::details {
	extern base::concurrent::queue* MainQueue;
	extern ::std::atomic<int> MainQueueCounter;

	class main_queue_pointer {
	public:
		main_queue_pointer() {
			grab();
		}

		void create(base::concurrent::main_queue_processor processor);

		explicit operator bool() const {
			return _pointer != nullptr;
		}

		base::concurrent::queue* operator->() const {
			return _pointer;
		}

		~main_queue_pointer() {
			ungrab();
		}

	private:
		void grab();
		void ungrab();

		base::concurrent::queue* _pointer = nullptr;

	};

} // namespace base::concurrent::details

namespace base::concurrent {
	void init_main_queue(main_queue_processor processor);
	inline void wrap_main_queue(main_queue_wrapper wrapper) {
		
	}

	template <typename Callable>
	inline void invokeAsync(Callable&& callable) {
		if (const auto main = details::main_queue_pointer())
			main->async(::std::forward<Callable>(callable));
	}

	template <typename Callable>
	inline void invokeSync(Callable&& callable) {
		if (const auto main = details::main_queue_pointer())
			main->sync(::std::forward<Callable>(callable));
	}
} // namespace base::concurrent