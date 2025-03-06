#include <base/core/UnixMutex.h>

#if defined(OS_MAC) || defined(OS_LINUX)

namespace base {
	UnixMutex::UnixMutex(Thread* thread) {

	}

	UnixMutex::~UnixMutex() {

	}

	bool UnixMutex::lock() {

	}

	bool UnixMutex::unlock() {

	}

	bool UnixMutex::isLocked() const noexcept {

	}
} // namespace base

#endif