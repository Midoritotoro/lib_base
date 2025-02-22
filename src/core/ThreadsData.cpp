#include <src/core/ThreadsData.h>
#include <base/core/Thread.h>

#include <vector>

namespace base {
	namespace {
		static std::vector<Thread*> _threads;
	}
	
	Thread* ThreadsData::threadById(sizetype id) {

	}

	Thread* ThreadsData::threadByHandle(const io::WindowsSmartHandle& handle) {

	}

	bool ThreadsData::insert(Thread* thread) {
		_threads.insert(thread)
	}
}