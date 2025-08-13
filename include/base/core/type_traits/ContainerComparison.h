#pragma once 

#include <base/core/BaseNamespace.h>

#include <type_traits>

#include <optional>
#include <variant>


__BASE_TYPE_TRAITS_NAMESPACE_BEGIN

// find out whether T is a conteiner
// this is required to check the value type of containers for the existence of the comparison operator
template <
	typename,
	typename = void>
struct _Is_container: 
	std::false_type
{};

template <typename _Type_>
struct _Is_container<
	_Type_, std::void_t<
		typename _Type_::value_type,
		std::is_convertible<decltype(
			std::declval<_Type_>().begin() != std::declval<_Type_>().end()),
	bool>
	>
>: 
	std::true_type 
{};

template <typename _Type_>
constexpr inline bool is_container_v = _Is_container<_Type_>::value;


template <
	typename,
	typename = void>
struct _Has_operator_equal: 
	std::false_type
{};

template <typename _Type_>
struct _Has_operator_equal<
	_Type_, 
	std::void_t<decltype(bool(
		std::declval<const _Type_&>() == std::declval<const _Type_&>()))
	>
>: 
	std::true_type
{};

// Two forward declarations
template <
	typename	_Type_,
	bool	=	is_container_v<_Type_>>
struct _Expand_operator_equal_container;

template <typename _Type_>
struct _Expand_operator_equal_tuple;

// the entry point for the public method
template <typename _Type_>
using _Expand_operator_equal = _Expand_operator_equal_container<_Type_>;

// if T isn't a container check if it's a tuple like object
template <
	typename _Type_,
	bool>
struct _Expand_operator_equal_container: 
	_Expand_operator_equal_tuple<_Type_>
{};

// if T::value_type exists, check first T::value_type, then T itself
template <typename _Type_>
struct _Expand_operator_equal_container<_Type_, true>:
	std::conjunction<
		std::disjunction<
			std::is_same<_Type_, typename _Type_::value_type>,
			_Expand_operator_equal<typename _Type_::value_type>
	>,	_Expand_operator_equal_tuple<_Type_>>
{};

// recursively check the template arguments of a tuple like object
template <typename ... _Type_>
using _Expand_operator_equal_recursive = std::conjunction<_Expand_operator_equal<_Type_>...>;

template <typename _Type_>
struct _Expand_operator_equal_tuple:
	_Has_operator_equal<_Type_>
{};

template <typename _Type_>
struct _Expand_operator_equal_tuple<std::optional<_Type_>>:
	_Expand_operator_equal_recursive<_Type_>
{};

template <
	typename _FirstType_,
	typename _SecondType_>
struct _Expand_operator_equal_tuple<std::pair<_FirstType_, _SecondType_>>:
	_Expand_operator_equal_recursive<_FirstType_, _SecondType_>
{};

template <typename ... _Type_>
struct _Expand_operator_equal_tuple<std::tuple<_Type_...>>:
	_Expand_operator_equal_recursive<_Type_...>
{};

template<typename ... _Type_>
struct _Expand_operator_equal_tuple<std::variant<_Type_...>>:
	_Expand_operator_equal_recursive<_Type_...>
{};


// the same for operator<(), see above for explanations
template <
	typename,
	typename = void>
struct _Has_operator_less_than:
	std::false_type
{};

template <typename _Type_>
struct _Has_operator_less_than<_Type_, std::void_t<decltype(
	bool(std::declval<const _Type_&>() < std::declval<const _Type_&>()))>
>:
	std::true_type
{};

template <
	typename	_Type_,
	bool =		is_container_v<_Type_>>
struct _Expand_operator_less_than_container;

template <typename _Type_>
struct _Expand_operator_less_than_tuple;

template <typename _Type_>
using _Expand_operator_less_than = _Expand_operator_less_than_container<_Type_>;

template <
	typename _Type_,
	bool>
struct _Expand_operator_less_than_container: 
	_Expand_operator_less_than_tuple<_Type_>
{};

template <typename _Type_>
struct _Expand_operator_less_than_container<_Type_, true>:
	std::conjunction<
		std::disjunction<
			std::is_same<_Type_, typename _Type_::value_type>,
			_Expand_operator_less_than<typename _Type_::value_type>
	>, expand_operator_less_than_tuple<_Type_>
> 
{};

template <typename ... _Type_>
using _Expand_operator_less_than_recursive = std::conjunction<_Expand_operator_less_than<_Type_>...>;

template <typename _Type_>
struct _Expand_operator_less_than_tuple:
	_Has_operator_less_than<_Type_>
{};

template <typename _Type_>
struct _Expand_operator_less_than_tuple<std::optional<_Type_>>:
	_Expand_operator_less_than_recursive<_Type_>
{};

template <
	typename _FirstType_,
	typename _SecondType_>
struct _Expand_operator_less_than_tuple<std::pair<_FirstType_, _SecondType_>>:
	_Expand_operator_less_than_recursive<_FirstType_, _SecondType_>
{};

template <typename ... _Type_>
struct _Expand_operator_less_than_tuple<std::tuple<_Type_...>>:
	_Expand_operator_less_than_recursive<_Type_...>
{};

template <typename ... _Type_>
struct _Expand_operator_less_than_tuple<std::variant<_Type_...>>:
	_Expand_operator_less_than_recursive<_Type_...>
{};


template <
	typename	_Type_,
	typename =	void>
struct _Is_dereferenceable: 
	std::false_type 
{};

template<typename _Type_>
struct _Is_dereferenceable<_Type_, std::void_t<decltype(std::declval<_Type_>().operator->())>>: 
	std::true_type
{};

template <typename _Type_>
inline constexpr bool is_dereferenceable_v = _Is_dereferenceable<_Type_>::value;



template <typename _Type_>
struct _Has_operator_equal: 
	_Expand_operator_equal<_Type_>
{};

template <typename _Type_>
inline constexpr bool has_operator_equal_v = _Has_operator_equal<_Type_>::value;

template <
	typename _Container_,
	typename _Type_>
using _Has_operator_equal_container = 
	std::disjunction<
		std::is_base_of<
			_Container_, _Type_>,
	_Has_operator_equal<_Type_>>;

template <typename _Type_>
struct _Has_operator_less_than: 
	_Expand_operator_less_than<_Type_> 
{};

template <typename _Type_>
inline constexpr bool has_operator_less_than_v = _Has_operator_less_than<_Type_>::value;


template <
	typename _Container_,
	typename _Type_>
using has_operator_less_than_container = 
	std::disjunction<
		std::is_base_of<
			_Container_, _Type_>,
	_Has_operator_less_than<_Type_>>;

template <typename ... _Type_>
using compare_eq_result = std::enable_if_t<
	std::conjunction_v<
		_Has_operator_equal<_Type_>...>,
	bool>;

template <
	typename		_Container_,
	typename ...	_Type_>
using compare_eq_result_container = std::enable_if_t<
	std::conjunction_v<
		_Has_operator_equal_container<
			_Container_, _Type_>...>, 
	bool>;

template <typename ... _Type_>
using compare_lt_result = std::enable_if_t<
	std::conjunction_v<
		_Has_operator_less_than<_Type_>...>,
	bool>;

template <
	typename		_Container_,
	typename ...	_Type_>
using compare_lt_result_container = std::enable_if_t<
	std::conjunction_v<
		_Has_operator_less_than_container<
			_Container_, _Type_>...>,
	bool>;

__BASE_TYPE_TRAITS_NAMESPACE_END
