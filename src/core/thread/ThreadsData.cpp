#include <src/core/ThreadsData.h>
#include <base/core/Thread.h>

#if defined(OS_WIN)
	#include <base/core/WindowsThread.h>
#elif defined(OS_MAC) || defined(OS_LINUX)
	#include <base/core/UnixThread.h>
#endif

#include <base/core/Mutex.h>

#include <vector>

namespace base {
	namespace {
		static std::vector<Thread*> _threads;
	}

#ifdef OS_WIN

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
}