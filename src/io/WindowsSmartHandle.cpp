#include <base/io/WindowsSmartHandle.h>

#if defined(OS_WIN)

namespace base::io {
	WindowsSmartHandle::WindowsSmartHandle()
	{}

	WindowsSmartHandle::WindowsSmartHandle(
		const WindowsSmartHandle& handle,
		Fn<BOOL(HANDLE)> deleteCallback
	):
		_handle(handle._handle),
		_deleteCallback(std::move(deleteCallback))
	{}

	WindowsSmartHandle::WindowsSmartHandle(HANDLE handle) :
		_handle(handle)
	{}

	WindowsSmartHandle::~WindowsSmartHandle() {
		if (_handle != nullptr)
			_deleteCallback(_handle);
	}

	void WindowsSmartHandle::setDeleteCallback(Fn<BOOL(HANDLE)> deleteCallback) {
		_deleteCallback = std::move(deleteCallback);
	}
	
	Fn<BOOL(HANDLE)> WindowsSmartHandle::deleteCallback() const noexcept {
		return _deleteCallback;
	}

	WindowsSmartHandle& WindowsSmartHandle::operator=(const WindowsSmartHandle& handle) {
		_handle = handle._handle;
		return *this;
	}

	WindowsSmartHandle& WindowsSmartHandle::operator=(HANDLE handle) {
		_handle = handle;
		return *this;
	}

	WindowsSmartHandle::operator HANDLE() const {
		return _handle;
	}
} // namespace base::io

#endif
