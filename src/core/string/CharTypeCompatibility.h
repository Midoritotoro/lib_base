#pragma once 

#include <base/core/utility/TypeTraits.h>

__BASE_STRING_NAMESPACE_BEGIN

template <typename _Char>
struct IsCompatibleCharTypeHelper
	: std::integral_constant<bool,
	std::is_same<_Char, _Char>::value ||
	std::is_same<_Char, ushort>::value ||
	std::is_same<_Char, char16_t>::value ||
	(std::is_same<_Char, wchar_t>::value && sizeof(wchar_t) == sizeof(_Char))> 
{};

template <typename _Char>
struct IsCompatibleCharType
	: IsCompatibleCharTypeHelper<remove_cvref_t<_Char>> 
{};

template <typename Pointer>
struct IsCompatiblePointerHelper 
	: std::false_type
{};

template <typename _Char>
struct IsCompatiblePointerHelper<_Char*>
	: IsCompatibleCharType<_Char> 
{};

template <typename Pointer>
struct IsCompatiblePointer
	: IsCompatiblePointerHelper<remove_cvref_t<Pointer>>
{};

template <
	typename T, 
	typename Enable = void>
struct IsContainerCompatibleWithStringView : std::false_type {};

template <typename T>
struct IsContainerCompatibleWithStringView<T, std::enable_if_t<std::conjunction_v<
	// lacking concepts and ranges, we accept any T whose std::data yields a suitable pointer ...
	IsCompatiblePointer<decltype(
		std::data(
			std::declval<const T&>()))>,
	// ... and that has a suitable size ...
	std::is_convertible<decltype(
		std::size(
			std::declval<const T&>()))
		, sizetype>,
	// ... and it's a range as it defines an iterator-like API
	IsCompatibleCharType<typename std::iterator_traits<
		decltype(
			std::begin(
				std::declval<const T&>()))>::value_type>,
	std::is_convertible<
		decltype(
			std::begin(
				std::declval<const T&>()) 
			!= 
			std::end(
				std::declval<const T&>())),
	bool>,

	// These need to be treated specially due to the empty vs null distinction
	std::negation<std::is_same<std::decay_t<T>, BasicString>>,

	// Don't make an accidental copy constructor
	std::negation<std::is_same<std::decay_t<T>, StringView>>
>>> 
	: std::true_type
{};

__BASE_STRING_NAMESPACE_END
