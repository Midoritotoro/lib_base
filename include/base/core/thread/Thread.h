#pragma once 

#include <base/core/thread/AbstractThread.h>
#include <base/core/utility/ClassHelperMacros.h>

#include <base/core/utility/TypeTraits.h>

#if defined(base_os_windows)
	#include <base/core/thread/WindowsThread.h>
	using ThreadPlatformImplementation = base::thread::WindowsThread;
#elif defined(base_os_mac) || defined(base_os_linux)
	#include <base/core/thread/UnixThread.h>
	using ThreadPlatformImplementation = base::thread::UnixThread;
#endif


__BASE_THREAD_NAMESPACE_BEGIN

class Thread final {
public:
	base_nodiscard_thread_constructor Thread() noexcept;
	base_nodiscard_thread_constructor Thread(const Thread& other) noexcept;

	base_nodiscard_thread_constructor Thread(Thread&& rOther) noexcept;


	template <
		class Function,
		class ... Args>
	base_nodiscard_thread_constructor Thread(
		Function&& _routine,
		Args&& ... args)
	{
		_impl->start(
			std::forward<Function>(_routine),
			std::forward<Args>(args)...);
	}

	// For methods
	template <
		class Owner,
		class Method,
		class ... Args>
	base_nodiscard_thread_constructor Thread(
		Owner* _owner,
		Method&& _routine,
		Args&& ... args)
	{
		static_assert(std::is_invocable_r_v<void, Method, Owner*, Args...>,
			"Method must be a callable type that can be called with Owner* as the first argument.");
		_impl->start(
			std::bind(
				std::forward<Method>(_routine),
				_owner,
				std::forward<Args>(args)...)
		);
	}

	Thread& operator=(const Thread& other) noexcept;
	Thread& operator=(Thread&& other) noexcept;

	~Thread();

	void setPriority(AbstractThread::Priority priority);
	base_nodiscard AbstractThread::Priority priority() const noexcept;

	void setTerminateOnClose(bool terminateOnClose);
	base_nodiscard bool terminateOnClose() const noexcept;

	base_nodiscard bool isFinished() const noexcept;
	base_nodiscard bool isRunning() const noexcept;

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

	// For methods
	template <
		class Owner,
		class Method,
		class ... Args>
	inline void start(
		Owner* _owner,
		Method&& _routine,
		Args&& ... args)
	{
		static_assert(std::is_invocable_r_v<void, Method, Owner*, Args...>,
			"Method must be a callable type that can be called with Owner* as the first argument.");
		_impl->start(
			std::bind(
				std::forward<Method>(_routine),
				_owner,
				std::forward<Args>(args)...)
		);
	}

	base_nodiscard ThreadPlatformImplementation* impl() const noexcept;
		
	static base_nodiscard int getIdealThreadCount() noexcept;
	static base_nodiscard Thread* currentThread() noexcept;
private:
	friend class ThreadsData;
	std::unique_ptr<ThreadPlatformImplementation> _impl = nullptr;
};

__BASE_THREAD_NAMESPACE_END
