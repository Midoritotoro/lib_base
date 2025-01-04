#pragma once

#include <base/concurrent/common/CommonConfig.h>

#ifdef USE_WINAPI

#include <base/Windows.h>
#include <memory>

namespace base::concurrent {

class semaphore {
public:
	semaphore():
		_handle(implementation::create()) 
	{}

	semaphore(semaphore&& other) noexcept
		: _handle(std::move(other._handle))
	{}

	semaphore(const semaphore &other) = delete;
	semaphore &operator=(const semaphore &other) = delete;

	semaphore &operator=(semaphore &&other) noexcept {
		_handle = std::move(other._handle);
		return *this;
	}

	void acquire();
	void release();

private:
	struct implementation {
		using pointer = void*;
		static pointer create();
		void operator()(pointer value);
	};

	std::unique_ptr<implementation::pointer, implementation> _handle;

};

} // namespace base::concurrent

#endif // USE_WINAPI
