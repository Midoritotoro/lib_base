#pragma once 

#include <base/core/thread/AbstractMutex.h>

#if defined(base_os_linux) || defined(base_os_mac)

__BASE_THREAD_NAMESPACE_BEGIN

class Thread;
class UnixMutex : public AbstractMutex {
public:
	UnixMutex(Thread* thread);
	~UnixMutex();

	base_nodiscard bool lock() override;
	base_nodiscard bool unlock() override;

	base_nodiscard bool isLocked() const noexcept override;
};

__BASE_THREAD_NAMESPACE_END

#endif