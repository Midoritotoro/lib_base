#include <base/io/WindowsSmartHandle.h>

#if defined(OS_WIN)

namespace base::io {
	WindowsSmartHandle::WindowsSmartHandle()
	{}

	WindowsSmartHandle::WindowsSmartHandle(const WindowsSmartHandle& handle) :
		_handle(handle._handle)
	{}

	WindowsSmartHandle::WindowsSmartHandle(HANDLE handle) :
		_handle(handle)
	{}

	WindowsSmartHandle::~WindowsSmartHandle() {
		if (_handle != nullptr)
			CloseHandle(_handle);
	}

	WindowsSmartHandle& WindowsSmartHandle::operator=(const WindowsSmartHandle& handle) {
		_handle = handle._handle;
		return *this;
	}

	WindowsSmartHandle& WindowsSmartHandle::operator=(HANDLE handle) {
		_handle = handle;
		return *this;
	}

	constexpr WindowsSmartHandle::operator HANDLE() {
		return _handle;
	}
} // namespace base::io

#endif
