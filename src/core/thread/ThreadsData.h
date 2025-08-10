#pragma once 

#include <base/core/io/WindowsSmartHandle.h>

__BASE_THREAD_NAMESPACE_BEGIN

class Thread;

class ThreadsData {
public:
#ifdef base_os_windows
	static   Thread* threadById(sizetype id);
	static   Thread* threadByHandle(const io::WindowsSmartHandle& handle);
#endif

	static   bool insert(Thread* thread);
};

__BASE_THREAD_NAMESPACE_END
