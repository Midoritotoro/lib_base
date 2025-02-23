#pragma once 

#include <base/core/AbstractThread.h>
#include <thread> 

namespace base {
	class Thread {
	public:
		Thread();
		~Thread();

	    void setPriority(AbstractThread::Priority priority);
		[[nodiscard]] AbstractThread::Priority priority() const noexcept;

		[[nodiscard]] bool isFinished() const noexcept;
		[[nodiscard]] bool isRunning() const noexcept;

		void waitMs(int ms);
		void waitS(int sec);

		void join();

		
		static [[nodiscard]] int getIdealThreadCount() noexcept;
		static [[nodiscard]] Thread* currentThread() noexcept;
	private:
		friend class ThreadsData;
		AbstractThread* _impl = nullptr;
	};
} // namespace base
