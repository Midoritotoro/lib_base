#pragma once 

#include <base/system/Platform.h>

#if defined(OS_WIN)

namespace base::io {
	class WindowsSmartHandle {
	public:
		WindowsSmartHandle();
		WindowsSmartHandle(const WindowsSmartHandle& handle);

		WindowsSmartHandle(HANDLE handle);
		~WindowsSmartHandle();

		WindowsSmartHandle& operator=(const WindowsSmartHandle& handle);
		WindowsSmartHandle& operator=(HANDLE handle);

		constexpr operator HANDLE();
	private:
		HANDLE _handle = nullptr;
	};
} // namespace base::io

#endif
