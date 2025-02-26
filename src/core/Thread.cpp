#include <base/core/Thread.h>

#include <base/system/SystemInfo.h>
#include <src/core/ThreadsData.h> 


namespace base {
	Thread::Thread() {
		_impl = std::make_unique<ThreadPlatformImplementation>();
	}

	Thread::~Thread() {

	}

	void Thread::setPriority(AbstractThread::Priority priority) {
		_impl->setPriority(priority);
	}

	AbstractThread::Priority Thread::priority() const noexcept {
		return _impl->priority();
	}

	void Thread::setTerminateOnClose(bool terminateOnClose) {
		_impl->setTerminateOnClose(terminateOnClose);
	}

	bool Thread::terminateOnClose() const noexcept {
		return _impl->terminateOnClose();
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

	void Thread::close() {
		_impl->close();
	}

	void Thread::terminate() {
		_impl->terminate();
	}

	void Thread::join() {
		_impl->join();
	}

	ThreadPlatformImplementation* Thread::impl() const noexcept {
		return _impl.get();
	}

	int Thread::getIdealThreadCount() noexcept {
		return system::SystemInfo::GetCpuCount();
	}

	Thread* Thread::currentThread() noexcept {
		return 
			ThreadsData::threadById(
				ThreadPlatformImplementation::currentThreadId());
	}
} // namespace base
