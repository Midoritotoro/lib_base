#include <base/core/Thread.h>

#include <base/system/SystemInfo.h>
#include <src/core/ThreadsData.h> 




namespace base {
	Thread::Thread() {
		_impl = new ThreadPlatformImplementation();
	}

	Thread::~Thread() {
		delete _impl;
		_impl = nullptr;
	}

	void Thread::setPriority(AbstractThread::Priority priority) {
		_impl->setPriority(priority);
	}

	AbstractThread::Priority Thread::priority() const noexcept {
		return _impl->priority();
	}

	bool Thread::isFinished() const noexcept {
		return _impl->isFinished();
	}

	bool Thread::isRunning() const noexcept {
		return _impl->isRunning();
	}

	void Thread::waitMs(int ms) {
		_impl->waitMs(ms);
	}

	void Thread::waitS(int sec) {
		_impl->waitMs(sec / 1000);
	}

	void Thread::join() {
		_impl->join();
	}

	//template <
	//	class Function,
	//	class ... Args>
	//void Thread::start(
	//	Function&& _routine,
	//	Args&& ... args) 
	//{
	//	((ThreadPlatformImplementation*)_impl)->start(
	//		std::forward<Function>(_routine),
	//		std::forward<Args>(args)...);
	//}

	int Thread::getIdealThreadCount() noexcept {
		return system::SystemInfo::GetCpuCount();
	}

	Thread* Thread::currentThread() noexcept {
		return 
			ThreadsData::threadById(
				ThreadPlatformImplementation::currentThreadId());
	}

} // namespace base
