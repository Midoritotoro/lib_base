#include <base/core/Thread.h>

#include <base/system/SystemInfo.h>

#if defined(OS_WIN)
	#include <base/core/WindowsThread.h>
	using ThreadPlatformImplementation = base::WindowsThread;
#elif defined(OS_MAC) || defined(OS_LINUX)
	#include <base/core/UnixThread.h>
	using ThreadPlatformImplementation = base::UnixThread;
#endif


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
		_impl->wait(ms);
	}

	void Thread::waitS(int sec) {
		_impl->waisS(sec);
	}

	int Thread::getIdealThreadCount() noexcept {
		return system::SystemInfo::GetCpuCount();
	}

	Thread* Thread::currentThread() noexcept {
		return ThreadPlatformImplementation::currentThread();
	}

} // namespace base
