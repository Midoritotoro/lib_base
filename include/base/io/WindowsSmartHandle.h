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
			bool autoDelete = true,
			Fn<BOOL(HANDLE)> deleteCallback = CloseHandle);

		WindowsSmartHandle(
			HANDLE handle,
			bool autoDelete = true);

		~WindowsSmartHandle();

		void setDeleteCallback(Fn<BOOL(HANDLE)> deleteCallback);
		[[nodiscard]] Fn<BOOL(HANDLE)> deleteCallback() const noexcept;

		[[nodiscard]] bool isValid() const noexcept;

		void setAutoDelete(bool autoDelete);
		[[nodiscard]] bool autoDelete() const noexcept;

		WindowsSmartHandle& operator=(const WindowsSmartHandle& handle);
		WindowsSmartHandle& operator=(HANDLE handle);

		void setHandle(
			not_null<HANDLE> handle, 
			bool autoDeletePrevious = true);
		[[nodiscard]] HANDLE handle() const noexcept;

		operator HANDLE() const;
	private:
		HANDLE _handle = nullptr;
		Fn<BOOL(HANDLE)> _deleteCallback = nullptr;

		bool _autoDelete = true;
	};
} // namespace base::io

#endif
