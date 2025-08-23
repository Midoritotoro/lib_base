#pragma once 

#include <base/core/BaseNamespace.h>
#include <type_traits>


__BASE_TYPE_TRAITS_NAMESPACE_BEGIN

template <class _Type_>
constexpr inline bool is_nonbool_integral_v = std::is_integral_v<_Type_> && !std::is_same_v<std::remove_cv_t<_Type_>, bool>;

template <class _Type_>
struct _Is_character:
	std::false_type
{};

template <>
struct _Is_character<char>:
	std::true_type
{};

template <>
struct _Is_character<signed char>:
	std::true_type
{};

template <>
struct _Is_character<unsigned char>:
	std::true_type
{};

#ifdef __cpp_char8_t

template <>
struct _Is_character<char8_t>:
	std::true_type
{};

#endif

template <class _Type_>
struct _Is_character_or_bool: 
	_Is_character<_Type_>::type
{};

template <>
struct _Is_character_or_bool<bool>:
	std::true_type
{};

template <class _Type_>
struct _Is_character_or_byte_or_bool:
	_Is_character_or_bool<_Type_>::type
{};

#ifdef __cpp_lib_byte

template <>
struct _Is_character_or_byte_or_bool<std::byte>:
	std::true_type
{};

#endif // defined(__cpp_lib_byte)

template <class _Type_>
constexpr inline bool is_character_v = _Is_character<_Type_>::value;

template <class _Type_>
constexpr inline bool is_character_or_bool_v = _Is_character_or_bool<_Type_>::value;

template <class _Type_>
constexpr inline bool is_character_or_byte_or_bool_v = _Is_character_or_byte_or_bool<_Type_>::value;

__BASE_TYPE_TRAITS_NAMESPACE_END
