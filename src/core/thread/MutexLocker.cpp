#include <base/core/thread/MutexLocker.h>

base_disable_warning_msvc(6031) // Пропуск возвращаемого значения


__BASE_THREAD_NAMESPACE_BEGIN

MutexLocker::MutexLocker(not_null<Mutex*> mutex):
	_mutex(mutex),
	_tag(GeneralMutex)
{
	_mutex->lock();
}

MutexLocker::MutexLocker(not_null<AbstractMutex*> mutex):
	_abstractMutex(mutex),
	_tag(PlatformMutex)
{
	_abstractMutex->lock();
}

MutexLocker::~MutexLocker() {
	if (isLocked())
		_tag == GeneralMutex ? 
			_mutex->unlock()
			: _abstractMutex->unlock();
}

bool MutexLocker::isLocked() const noexcept {
	return 
		_tag == GeneralMutex ?
			_mutex->isLocked()
			: _abstractMutex->isLocked();
}

__BASE_THREAD_NAMESPACE_END