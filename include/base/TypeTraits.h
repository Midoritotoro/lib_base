#pragma once 

#include <type_traits>

namespace base {
	template <
		typename Struct,
		typename Member>
	using struct_has_member_impl = std::void_t<
		decltype(std::declval<
			Struct>().*std::declval<Member>())>;

	template <
		typename Struct,
		typename Member>
	constexpr bool struct_has_member = std::is_convertible_v<
		struct_has_member_impl<Struct, Member>, void>;
} // namespace base