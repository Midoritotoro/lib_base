#include <base/core/io/WindowsSmartHandle.h>

#if defined(base_os_windows)

__BASE_IO_NAMESPACE_BEGIN

WindowsSmartHandle::WindowsSmartHandle()
{}

WindowsSmartHandle::WindowsSmartHandle(
	HANDLE handle,
	Fn<BOOL(HANDLE)> deleteCallback
) :
	_handle(handle),
	_deleteCallback(std::move(deleteCallback))
{}

WindowsSmartHandle::~WindowsSmartHandle() {
	if (_autoDelete && _handle != nullptr)
		_deleteCallback(_handle);
}

void WindowsSmartHandle::setDeleteCallback(Fn<BOOL(HANDLE)> deleteCallback) {
	_deleteCallback = std::move(deleteCallback);
}

HANDLE WindowsSmartHandle::handle() const noexcept {
	return _handle;
}

HANDLE WindowsSmartHandle::constHandle() const noexcept {
	return _handle;
}

HANDLE WindowsSmartHandle::handle() noexcept {
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

void WindowsSmartHandle::forceDelete() {
	if (isValid())
		_deleteCallback(_handle);

	_handle = INVALID_HANDLE_VALUE;
}

bool WindowsSmartHandle::isValid() const noexcept {
	return 
		(_handle != nullptr 
		&& _handle != INVALID_HANDLE_VALUE);
}

WindowsSmartHandle& WindowsSmartHandle::operator=(HANDLE handle) {
	_handle = handle;
	return *this;
}

bool WindowsSmartHandle::operator==(HANDLE handle) {
	return (_handle == handle);
}

__BASE_IO_NAMESPACE_END

#endif
