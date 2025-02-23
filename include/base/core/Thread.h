#pragma once 

#include <base/core/AbstractThread.h>

#if defined(OS_WIN)
	#include <base/core/WindowsThread.h>
	using ThreadPlatformImplementation = base::WindowsThread;
#elif defined(OS_MAC) || defined(OS_LINUX)
	#include <base/core/UnixThread.h>
	using ThreadPlatformImplementation = base::UnixThread;
#endif


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

		template <
			class Function,
			class ... Args>
		void start(
			Function&& _routine,
			Args&& ... args) 
		{
			((ThreadPlatformImplementation*)_impl)->start(
				std::forward<Function>(_routine),
				std::forward<Args>(args)...);
		} 
		
		static [[nodiscard]] int getIdealThreadCount() noexcept;
		static [[nodiscard]] Thread* currentThread() noexcept;
	private:
		friend class ThreadsData;
		AbstractThread* _impl = nullptr;
	};
} // namespace base
