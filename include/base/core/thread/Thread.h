#pragma once 

#include <base/core/thread/AbstractThread.h>
#include <base/core/utility/ClassHelperMacros.h>

#include <base/core/utility/TypeTraits.h>

#if defined(OS_WIN)
	#include <base/core/thread/WindowsThread.h>
	using ThreadPlatformImplementation = base::thread::WindowsThread;
#elif defined(OS_MAC) || defined(OS_LINUX)
	#include <base/core/thread/UnixThread.h>
	using ThreadPlatformImplementation = base::thread::UnixThread;
#endif


__BASE_THREAD_NAMESPACE_BEGIN

class Thread final {
public:
	NODISCARD_THREAD_CTOR Thread() noexcept;
	NODISCARD_THREAD_CTOR Thread(const Thread& other) noexcept;

	NODISCARD_THREAD_CTOR Thread(Thread&& rOther) noexcept;


	template <
		class Function,
		class ... Args>
	NODISCARD_THREAD_CTOR Thread(
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
	NODISCARD_THREAD_CTOR Thread(
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
	NODISCARD AbstractThread::Priority priority() const noexcept;

	void setTerminateOnClose(bool terminateOnClose);
	NODISCARD bool terminateOnClose() const noexcept;

	NODISCARD bool isFinished() const noexcept;
	NODISCARD bool isRunning() const noexcept;

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

	NODISCARD ThreadPlatformImplementation* impl() const noexcept;
		
	static NODISCARD int getIdealThreadCount() noexcept;
	static NODISCARD Thread* currentThread() noexcept;
private:
	friend class ThreadsData;
	std::unique_ptr<ThreadPlatformImplementation> _impl = nullptr;
};

__BASE_THREAD_NAMESPACE_END
