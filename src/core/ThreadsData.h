#pragma once 

#include <base/io/WindowsSmartHandle.h>

namespace base {
	class Thread;

	class ThreadsData {
	public:
#ifdef OS_WIN
		static [[nodiscard]] Thread* threadById(sizetype id);
		static [[nodiscard]] Thread* threadByHandle(const io::WindowsSmartHandle& handle);
#endif

		static [[nodiscard]] bool insert(Thread* thread);
	};
} // namespace base::threads