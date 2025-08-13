#pragma once 

#include <base/core/BaseNamespace.h>
#include <type_traits>


__BASE_TYPE_TRAITS_NAMESPACE_BEGIN

#if defined(base_cpp_clang) || defined(__EDG__)
  void swap() = delete; // Block unqualified name lookup
#else
  void swap();
#endif

template <
	class,
	class = void>
struct _Has_adl_swap:
	std::false_type
{};

template <class _Type_>
struct _Has_adl_swap<_Type_, std::void_t<decltype(std::swap(
	std::declval<_Type_&>(),
	std::declval<_Type_&>()))>
>:
	std::true_type
{};

template <class _Type_>
constexpr bool is_trivially_swappable_v = std::conjunction_v<
	std::is_trivially_destructible<_Type_>,
	std::is_trivially_move_constructible<_Type_>,
	std::is_trivially_move_assignable<_Type_>,
	std::negation<_Has_adl_swap<_Type_>
>>;

#ifdef __cpp_lib_byte
template <>
inline constexpr bool is_trivially_swappable_v<std::byte> = true;
#endif // defined(__cpp_lib_byte)

__BASE_TYPE_TRAITS_NAMESPACE_END
