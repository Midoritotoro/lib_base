#pragma once 

#include <type_traits>
#include <ranges>



namespace base {
	using namespace ::std::ranges;

	template <typename F, typename Type, typename Type2>
	concept indirectly_binary_invocable =
		::std::indirectly_readable<Type> &&
		::std::indirectly_readable<Type2> &&
		::std::copy_constructible<F>;

	struct plus
	{
		template<
			typename T,
			typename U>
		constexpr auto operator()(T&& t, U&& u)
			const -> decltype((T&&)t + (U&&)u)
		{
			return (T&&)t + (U&&)u;
		}

		using is_transparent = void;
	};

	struct identity
	{
		template<typename T>
		constexpr T&& operator()(T&& t) const noexcept {
			return (T&&)t;
		}
		using is_transparent = void;
	};

	template <
		typename Struct,
		typename Member>
	using has_member_impl = std::void_t<
		decltype(std::declval<
			Struct>().*std::declval<Member>())>;

	template <
		typename Struct,
		typename Member>
	constexpr bool has_member = std::is_convertible_v<
		has_member_impl<Struct, Member>, void>;

	template <typename T>
	inline constexpr bool IsRelocatable = 
		std::is_trivially_copyable_v<T> 
		&& std::is_trivially_destructible_v<T>;

	template <typename T>
	inline constexpr bool IsValueInitializationBitwiseZero =
		std::is_scalar_v<T> && 
		!std::is_member_object_pointer_v<T>;
} // namespace base