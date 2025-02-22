#pragma once 

#include <base/io/WindowsSmartHandle.h>

namespace base {
	class Thread;

	class ThreadsData {
	public:
		static [[nodiscard]] Thread* threadById(sizetype id);
		static [[nodiscard]] Thread* threadByHandle(const io::WindowsSmartHandle& handle);

		static [[nodiscard]] bool insert(Thread* thread);
	};
} // namespace base::threads