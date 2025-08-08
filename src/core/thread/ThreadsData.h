#pragma once 

#include <base/core/io/WindowsSmartHandle.h>

__BASE_THREAD_NAMESPACE_BEGIN

class Thread;

class ThreadsData {
public:
#ifdef base_os_windows
	static base_nodiscard Thread* threadById(sizetype id);
	static base_nodiscard Thread* threadByHandle(const io::WindowsSmartHandle& handle);
#endif

	static base_nodiscard bool insert(Thread* thread);
};

__BASE_THREAD_NAMESPACE_END
