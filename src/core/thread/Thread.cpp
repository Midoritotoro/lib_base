#include <base/core/thread/Thread.h>

#include <base/core/system/SystemInfo.h>
#include <src/core/thread/ThreadsData.h> 


__BASE_THREAD_NAMESPACE_BEGIN

Thread::Thread() noexcept {
	_impl = std::make_unique<ThreadPlatformImplementation>();
}

Thread::~Thread() {

}

Thread::Thread(const Thread& other) noexcept {

}

Thread::Thread(Thread&& rOther) noexcept {

}

Thread& Thread::operator=(const Thread& other) noexcept {
	return *this;
}

Thread& Thread::operator=(Thread&& other) noexcept {
	return *this;
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

__BASE_THREAD_NAMESPACE_END
