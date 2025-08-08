#pragma once 

#include <base/core/arch/Platform.h>
#include <base/core/Types.h>

#include <base/core/utility/ClassHelperMacros.h>

#if defined(base_os_windows)

__BASE_NAMESPACE_BEGIN

using handle_t = HANDLE;

__BASE_NAMESPACE_END

__BASE_IO_NAMESPACE_BEGIN


class WindowsSmartHandle {
public:
	WindowsSmartHandle();
	WindowsSmartHandle(
		HANDLE handle,
		Fn<BOOL(HANDLE)> deleteCallback = CloseHandle);

	~WindowsSmartHandle();

	void setDeleteCallback(Fn<BOOL(HANDLE)> deleteCallback);
	base_nodiscard Fn<BOOL(HANDLE)> deleteCallback() const noexcept;

	void forceDelete();

	base_nodiscard bool isValid() const noexcept;

	void setAutoDelete(bool autoDelete);
	base_nodiscard bool autoDelete() const noexcept;

	WindowsSmartHandle& operator=(HANDLE handle);
	bool operator==(HANDLE handle);

	void setHandle(
		not_null<HANDLE> handle, 
		bool autoDeletePrevious = true);

	base_nodiscard HANDLE handle() const noexcept;
	base_nodiscard HANDLE constHandle() const noexcept;

	base_nodiscard HANDLE handle() noexcept;

	DISABLE_COPY(WindowsSmartHandle);
private:
	HANDLE _handle = nullptr;
	Fn<BOOL(HANDLE)> _deleteCallback = CloseHandle;

	bool _autoDelete = false;
};

__BASE_IO_NAMESPACE_END

#endif
