#include <base/core/thread/UnixMutex.h>

#if defined(base_os_mac) || defined(base_os_linux)

__BASE_THREAD_NAMESPACE_BEGIN

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

__BASE_THREAD_NAMESPACE_END

#endif