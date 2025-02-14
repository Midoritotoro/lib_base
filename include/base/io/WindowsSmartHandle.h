#pragma once 

#include <base/system/Platform.h>
#include <base/core/Types.h>

#if defined(OS_WIN)

namespace base::io {
	class WindowsSmartHandle {
	public:
		WindowsSmartHandle();
		WindowsSmartHandle(
			const WindowsSmartHandle& handle,
			Fn<BOOL(HANDLE)> deleteCallback = CloseHandle);

		WindowsSmartHandle(HANDLE handle);
		~WindowsSmartHandle();

		void setDeleteCallback(Fn<BOOL(HANDLE)> deleteCallback);
		[[nodiscard]] Fn<BOOL(HANDLE)> deleteCallback() const noexcept;

		WindowsSmartHandle& operator=(const WindowsSmartHandle& handle);
		WindowsSmartHandle& operator=(HANDLE handle);

		operator HANDLE() const;
	private:
		HANDLE _handle = nullptr;
		Fn<BOOL(HANDLE)> _deleteCallback = nullptr;
	};
} // namespace base::io

#endif
