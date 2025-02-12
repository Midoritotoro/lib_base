#pragma once 

#include <type_traits>

namespace base {
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