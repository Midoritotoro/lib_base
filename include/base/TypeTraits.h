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
} // namespace base