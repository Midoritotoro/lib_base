#pragma once 

#include <base/core/AbstractThread.h>
#include <base/utility/ClassHelperMacros.h>

#if defined(OS_WIN)
	#include <base/core/WindowsThread.h>
	using ThreadPlatformImplementation = base::WindowsThread;
#elif defined(OS_MAC) || defined(OS_LINUX)
	#include <base/core/UnixThread.h>
	using ThreadPlatformImplementation = base::UnixThread;
#endif


namespace base {
	class Thread final {
	public:
		Thread();
		~Thread();

	    void setPriority(AbstractThread::Priority priority);
		[[nodiscard]] AbstractThread::Priority priority() const noexcept;

		void setTerminateOnClose(bool terminateOnClose);
		bool terminateOnClose() const noexcept;

		[[nodiscard]] bool isFinished() const noexcept;
		[[nodiscard]] bool isRunning() const noexcept;

		void waitMs(int ms);
		void waitS(int sec);

		void join();

		void close();
		void terminate();

		template <
			class Function,
			class ... Args>
		inline void start(
			Function&& _routine,
			Args&& ... args) 
		{
			_impl->start(
				std::forward<Function>(_routine),
				std::forward<Args>(args)...);
		} 

		[[nodiscard]] ThreadPlatformImplementation* impl() const noexcept;
		
		static [[nodiscard]] int getIdealThreadCount() noexcept;
		static [[nodiscard]] Thread* currentThread() noexcept;

		DISABLE_COPY(Thread)
	private:
		friend class ThreadsData;
		std::unique_ptr<ThreadPlatformImplementation> _impl = nullptr;
	};
} // namespace base
