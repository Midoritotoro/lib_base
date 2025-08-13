#pragma once 

#include <type_traits>
#include <base/core/BaseNamespace.h>

#include <base/core/compatibility/Warnings.h>
#include <base/core/type_traits/IsDetected.h>

__BASE_TYPE_TRAITS_NAMESPACE_BEGIN

namespace _detail {
	base_warning_push

	base_disable_warning_gcc("-Wold-style-cast");
	base_disable_warning_clang("-Wold-style-cast");

	template <
		typename From,
		typename To>
	using is_virtual_base_conversion_test = decltype((To*)std::declval<From*>());

	base_warning_push

	template <
		typename Base,
		typename Derived,
		typename = void>
	struct is_virtual_base_of:
		std::false_type
	{};

	template <
		typename Base,
		typename Derived>
	struct is_virtual_base_of<
		Base, Derived,
		std::enable_if_t<
			std::conjunction_v<
				std::is_base_of<Base, Derived>,
				is_detected<is_virtual_base_conversion_test, Derived, Base>,
			std::negation<
				is_detected<is_virtual_base_conversion_test, Base, Derived>
				>
			>
		>	
	>: 
		std::true_type
	{};
}

template <
	typename Base,
	typename Derived>
using is_virtual_base_of = _detail::is_virtual_base_of<
	std::remove_cv_t<Base>,
	std::remove_cv_t<Derived>>;

template <
	typename Base,
	typename Derived>
constexpr inline bool is_virtual_base_of_v = is_virtual_base_of<Base, Derived>::value;

__BASE_TYPE_TRAITS_NAMESPACE_END