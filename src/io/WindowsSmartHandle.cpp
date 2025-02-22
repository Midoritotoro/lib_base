#include <base/io/WindowsSmartHandle.h>

#if defined(OS_WIN)

namespace base::io {
	WindowsSmartHandle::WindowsSmartHandle()
	{}

	WindowsSmartHandle::WindowsSmartHandle(
		const WindowsSmartHandle& handle,
		bool autoDelete,
		Fn<BOOL(HANDLE)> deleteCallback
	) :
		_handle(handle._handle),
		_deleteCallback(std::move(deleteCallback)),
		_autoDelete(autoDelete)
	{}

	WindowsSmartHandle::WindowsSmartHandle(
		HANDLE handle,
		bool autoDelete
	) :
		_handle(handle),
		_autoDelete(autoDelete)
	{}

	WindowsSmartHandle::~WindowsSmartHandle() {
		if (_handle != nullptr)
			_deleteCallback(_handle);
	}

	void WindowsSmartHandle::setDeleteCallback(Fn<BOOL(HANDLE)> deleteCallback) {
		_deleteCallback = std::move(deleteCallback);
	}

	HANDLE WindowsSmartHandle::handle() const noexcept {
		return _handle;
	}

	void WindowsSmartHandle::setAutoDelete(bool autoDelete) {
		_autoDelete = autoDelete;
	}

	bool WindowsSmartHandle::autoDelete() const noexcept {
		return _autoDelete;
	}

	void WindowsSmartHandle::setHandle(
		not_null<HANDLE> handle,
		bool autoDeletePrevious) 
	{
		if (autoDeletePrevious && _handle != nullptr)
			_deleteCallback(_handle);

		_handle = handle;
	}
	
	Fn<BOOL(HANDLE)> WindowsSmartHandle::deleteCallback() const noexcept {
		return _deleteCallback;
	}

	bool WindowsSmartHandle::isValid() const noexcept {
		return 
			(_handle != nullptr 
			&& _handle != INVALID_HANDLE_VALUE);
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
		return handle();
	}
} // namespace base::io

#endif
