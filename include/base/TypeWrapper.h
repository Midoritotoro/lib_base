#pragma once 

#include <functional>
#include <type_traits>

#include <memory>


namespace base {
	template <
		typename T,
		T t>
	struct ValueToType
	{};

	template<
		typename C,
		class T>
	class ClassifiedWrapper
	{};

	template<typename C,
		C c,
		class T>
	class ClassifiedWrapper<
		ValueToType<C, c>,
		T> : private T
	{
	public:
		ClassifiedWrapper(T&& fn): 
			T(std::forward<T>(fn)) {};

		ClassifiedWrapper() = delete;
		virtual ~ClassifiedWrapper() {};

		template <typename ... Args>
		std::result_of<T(Args...)>::type
			operator()(
				ValueToType<C, c>&&, 
				Args&& ... args)
		{
			return T::operator()(
				std::forward<Args>(args) ...
			);
		};
	};

	template <class ... T>
	class Wrapper
	{};

	template <typename C,
		C c,
		class Func,
		class ... T>
	class Wrapper<ValueToType<C, c>, Func, T ...> :
		protected ClassifiedWrapper<ValueToType<C, c>, Func>,
		protected Wrapper<T ...>
	{
	protected:
		using ClassifiedWrapper<ValueToType<C, c>, Func>::operator();
		using Wrapper<T ...>::operator();
	public:
		Wrapper(
			ValueToType<C, c>&& vt, 
			Func&& func,
			T&& ... t
		):
			ClassifiedWrapper<ValueToType<C, c>,
				Func>(std::forward<Func>(func)),
			Wrapper<T ...>(std::forward<T>(t) ...)
				{};

		Wrapper() = delete;
		virtual ~Wrapper() {};

		template <typename Cmd, Cmd cmd, typename ... Args>
		std::result_of<Func(Args...)>::type
			call(Args&& ... args)
		{
			return this->operator()(ValueToType<Cmd, cmd>(), std::forward<Args>(args)...);
		};
	};

	template <typename C, C c, class Func>
	class Wrapper<ValueToType<C, c>, Func>:
		protected ClassifiedWrapper<ValueToType<C, c>, Func>
	{
	public:
		Wrapper(
			ValueToType<C, c>&&,
			Func&& func
		):
			ClassifiedWrapper<ValueToType<C, c>, Func>
				(std::forward<Func>(func)) {};

		Wrapper() = delete;
		virtual ~Wrapper() {};
	};
}