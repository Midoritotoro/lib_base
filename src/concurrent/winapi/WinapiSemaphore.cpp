#include "WinapiSemaphore.h"
#include <gsl/gsl>

#ifdef USE_WINAPI

namespace concurrent {
	auto semaphore::implementation::create() -> pointer {
		auto result = CreateSemaphore(nullptr, 0, 1, nullptr);
		Expects(result);
	
		return result;
	}

	void semaphore::implementation::operator()(pointer value) {
		if (value)
			CloseHandle(value);
	};

	void semaphore::acquire() {
		WaitForSingleObject(_handle.get(), INFINITE);
	}

	void semaphore::release() {
		ReleaseSemaphore(_handle.get(), 1, nullptr);
	}
} // namespace concurrent

#endif // USE_WINAPI
