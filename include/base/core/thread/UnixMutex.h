#pragma once 

#include <base/core/thread/AbstractMutex.h>

#if defined(OS_LINUX) || defined(OS_MAC)

__BASE_THREAD_NAMESPACE_BEGIN

class Thread;
class UnixMutex : public AbstractMutex {
public:
	UnixMutex(Thread* thread);
	~UnixMutex();

	[[nodiscard]] bool lock() override;
	[[nodiscard]] bool unlock() override;

	[[nodiscard]] bool isLocked() const noexcept override;
};

__BASE_THREAD_NAMESPACE_END

#endif