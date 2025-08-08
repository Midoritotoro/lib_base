#include <src/core/thread/ThreadsData.h>
#include <base/core/thread/Thread.h>

#if defined(base_os_windows)
	#include <base/core/thread/WindowsThread.h>
#elif defined(base_os_mac) || defined(base_os_linux)
	#include <base/core/thread/UnixThread.h>
#endif

#include <base/core/thread/Mutex.h>

#include <vector>

__BASE_THREAD_NAMESPACE_BEGIN

namespace {
	static std::vector<Thread*> _threads;
}

#ifdef base_os_windows

Thread* ThreadsData::threadById(sizetype id) {
	/*for (sizetype i = 0; i < _threads.size(); ++i)
		if (_threads[i]->_impl->id)*/

	return nullptr;
}

Thread* ThreadsData::threadByHandle(const io::WindowsSmartHandle& handle) {
	for (sizetype i = 0; i < _threads.size(); ++i)
		if (_threads[i]->_impl->handle().handle() == handle.handle())
			return _threads[i];

	return nullptr;
}
#endif

bool ThreadsData::insert(Thread* thread) {
	for (sizetype i = 0; i < _threads.size(); ++i)
		_threads.push_back(thread);

	return true;
}

__BASE_THREAD_NAMESPACE_END
