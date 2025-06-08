#pragma once 

#include <base/core/arch/Platform.h>
#include <base/core/Types.h>

#include <base/core/utility/ClassHelperMacros.h>

#if defined(OS_WIN)

__BASE_IO_NAMESPACE_BEGIN

class WindowsSmartHandle {
public:
	WindowsSmartHandle();
	WindowsSmartHandle(
		HANDLE handle,
		Fn<BOOL(HANDLE)> deleteCallback = CloseHandle);

	~WindowsSmartHandle();

	void setDeleteCallback(Fn<BOOL(HANDLE)> deleteCallback);
	NODISCARD Fn<BOOL(HANDLE)> deleteCallback() const noexcept;

	void forceDelete();

	NODISCARD bool isValid() const noexcept;

	void setAutoDelete(bool autoDelete);
	NODISCARD bool autoDelete() const noexcept;

	WindowsSmartHandle& operator=(HANDLE handle);
	bool operator==(HANDLE handle);

	void setHandle(
		not_null<HANDLE> handle, 
		bool autoDeletePrevious = true);

	NODISCARD HANDLE handle() const noexcept;
	NODISCARD HANDLE constHandle() const noexcept;

	NODISCARD HANDLE handle() noexcept;

	DISABLE_COPY(WindowsSmartHandle);
private:
	HANDLE _handle = nullptr;
	Fn<BOOL(HANDLE)> _deleteCallback = CloseHandle;

	bool _autoDelete = false;
};

__BASE_IO_NAMESPACE_END

#endif
