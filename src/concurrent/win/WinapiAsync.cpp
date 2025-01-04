#include "base/concurrent/win/WinapiAsync.h"

#ifdef USE_WINAPI

#include <concrt.h>

namespace base::concurrent::details {
	void async_plain(void (*callable)(void*), void *argument) {
		Concurrency::CurrentScheduler::ScheduleTask(callable, argument);
	}
} // namespace base::concurrent::details

#endif // USE_WINAPI
