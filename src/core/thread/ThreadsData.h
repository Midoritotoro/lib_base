#pragma once 

#include <base/core/io/WindowsSmartHandle.h>

__BASE_THREAD_NAMESPACE_BEGIN

class Thread;

class ThreadsData {
public:
#ifdef OS_WIN
	static [[nodiscard]] Thread* threadById(sizetype id);
	static [[nodiscard]] Thread* threadByHandle(const io::WindowsSmartHandle& handle);
#endif

	static [[nodiscard]] bool insert(Thread* thread);
};

__BASE_THREAD_NAMESPACE_END
