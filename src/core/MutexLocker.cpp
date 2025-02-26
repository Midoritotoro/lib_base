#include <base/core/MutexLocker.h>

WARNING_DISABLE_MSVC(6031) // Пропуск возвращаемого значения


namespace base {
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
} // namespace base