#pragma once 

#include <type_traits>

#include <base/core/arch/Platform.h>
#include <base/core/utility/Concepts.h>

__BASE_NAMESPACE_BEGIN

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
inline constexpr bool has_member = std::is_convertible_v<
has_member_impl<Struct, Member>, void>;

template <typename T>
inline constexpr bool IsRelocatable =
std::is_trivially_copyable_v<T>
&& std::is_trivially_destructible_v<T>;

template <typename T>
inline constexpr bool IsValueInitializationBitwiseZero =
std::is_scalar_v<T> &&
!std::is_member_object_pointer_v<T>;

#ifdef __cpp_lib_is_constant_evaluated
using std::is_constant_evaluated;
#define SUPPORTS_IS_CONSTANT_EVALUATED
#else
constexpr bool is_constant_evaluated() noexcept
{
#if 0
return false;
#elif __has_builtin(__builtin_is_constant_evaluated)
	# define SUPPORTS_IS_CONSTANT_EVALUATED
	return __builtin_is_constant_evaluated();
#else
	return false;
#endif
}
#endif // __cpp_lib_is_constant_evaluated

#ifdef __cpp_lib_remove_cvref
using std::remove_cvref;
using std::remove_cvref_t;
#else
template <typename T>
using remove_cvref = std::remove_cv<std::remove_reference_t<T>>;
template <typename T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;
#endif // __cpp_lib_remove_cvref

struct nonesuch {
	~nonesuch() = delete;
	nonesuch(const nonesuch&) = delete;
	void operator=(const nonesuch&) = delete;
};

namespace _detail {
	template <
		typename T,
		typename Void, 
		template <typename...> class Op,
		typename...Args>
	struct detector {
		using value_t = std::false_type;
		using type = T;
	};

	template <
		typename T, 
		template <typename...> class Op,
		typename...Args>
	struct detector<T, std::void_t<Op<Args...>>, Op, Args...> {
		using value_t = std::true_type;
		using type = Op<Args...>;
	};
} // namespace _detail

template <template <typename...> class Op, typename...Args>
using is_detected = typename _detail::detector<nonesuch, void, Op, Args...>::value_t;

template <template <typename...> class Op, typename...Args>
constexpr inline bool is_detected_v = is_detected<Op, Args...>::value;

namespace _detail {
	WARNING_PUSH
		WARNING_DISABLE_GCC("-Wold-style-cast")
		WARNING_DISABLE_CLANG("-Wold-style-cast")
		template <typename From, typename To>
	using is_virtual_base_conversion_test = decltype(
		(To*)std::declval<From*>()
		);
	WARNING_POP

		template <typename Base, typename Derived, typename = void>
	struct is_virtual_base_of : std::false_type {};

	template <typename Base, typename Derived>
	struct is_virtual_base_of <
		Base, Derived,
		std::enable_if_t <
		std::conjunction_v<
		std::is_base_of<Base, Derived>,

		is_detected<is_virtual_base_conversion_test, Derived, Base>,

		std::negation<
		is_detected<is_virtual_base_conversion_test, Base, Derived>
		>
		>
		>
	> : std::true_type{};
}

template <typename Base, typename Derived>
using is_virtual_base_of = _detail::is_virtual_base_of<
	std::remove_cv_t<Base>, 
	std::remove_cv_t<Derived>>;

template <
	typename Base,
	typename Derived>
constexpr inline bool is_virtual_base_of_v = is_virtual_base_of<Base, Derived>::value;

template <typename Iterator>
using IfIsInputIterator = typename std::enable_if<
	std::is_convertible<typename 
	std::iterator_traits<Iterator>::iterator_category, 
	std::input_iterator_tag>::value,
	bool>::type;

template <typename Iterator>
using IfIsForwardIterator = typename std::enable_if<
	std::is_convertible<typename 
	std::iterator_traits<Iterator>::iterator_category,
	std::forward_iterator_tag>::value,
	bool>::type;

template <typename Iterator>
using IfIsNotForwardIterator = typename std::enable_if<
	!std::is_convertible<typename 
	std::iterator_traits<Iterator>::iterator_category,
	std::forward_iterator_tag>::value,
	bool>::type;

template <
	typename,
	typename = void>
constexpr bool IsTypeComplete = false;

template <typename T>
constexpr bool IsTypeComplete
	<T, std::void_t<decltype(sizeof(T))>> = true;

template <class _Type_>
constexpr bool is_nonbool_integral_v = 
	std::is_integral_v<_Type_> &&
	!std::is_same_v<std::remove_cv_t<_Type_>, bool>;

template <class _Iterator_>
constexpr bool IteratorIsContiguous = std::contiguous_iterator<_Iterator_>;

template <
	class _FirstIterator_,
	class _SecondIterator_>
constexpr bool IteratorsAreContiguous = 
	IteratorIsContiguous<_FirstIterator_> 
	&& IteratorsIsContiguous<_SecondIterator_>;


template <class _Iterator_>
constexpr bool IsIteratorVolatile = 
	std::is_volatile_v<
		std::remove_reference_t<
			std::iter_reference_t<_Iterator_>>>;


template <class _Type_>
struct IsCharacter : 
	std::false_type 
{};

template <>
struct IsCharacter<char> : 
	std::true_type 
{};

template <>
struct IsCharacter<signed char> :
	std::true_type
{}; 

template <>
struct IsCharacter<unsigned char> : 
	std::true_type
{}; 

#ifdef __cpp_char8_t

template <>
struct IsCharacter<char8_t> :
	std::true_type 
{};

#endif

template <class _Type_>
struct IsCharacterOrBool 
	: IsCharacter<_Type_>::type 
{};

template <>
struct IsCharacterOrBool<bool> : 
	std::true_type 
{};

template <class _Type_>
struct IsCharacterOrByteOrBool :
	IsCharacterOrBool<_Type_>::type
{};

#ifdef __cpp_lib_byte

template <>
struct IsCharacterOrByteOrBool<byte> : 
	std::true_type 
{};

#endif // defined(__cpp_lib_byte)

// --------------------------------------------------

namespace detail {
	// find out whether T is a conteiner
	// this is required to check the value type of containers for the existence of the comparison operator
	template <
		typename,
		typename = void>
	struct is_container
		: std::false_type
	{};

	template <typename T>
	struct is_container<
		T, std::void_t<
		typename T::value_type,
		std::is_convertible<decltype(
			std::declval<T>().begin() != std::declval<T>().end()), bool>
		>> : std::true_type {};


	template <
		typename,
		typename = void>
	struct has_operator_equal
		: std::false_type
	{};

	template <typename T>
	struct has_operator_equal<T, std::void_t<decltype(bool(
		std::declval<const T&>() == std::declval<const T&>()))>>
		: std::true_type
	{};

	// Two forward declarations
	template <
		typename T,
		bool = is_container<T>::value>
	struct expand_operator_equal_container;

	template <typename T>
	struct expand_operator_equal_tuple;

	// the entry point for the public method
	template <typename T>
	using expand_operator_equal = expand_operator_equal_container<T>;

	// if T isn't a container check if it's a tuple like object
	template <
		typename T,
		bool>
	struct expand_operator_equal_container
		: expand_operator_equal_tuple<T>
	{};
	// if T::value_type exists, check first T::value_type, then T itself
	template <typename T>
	struct expand_operator_equal_container<T, true> :
		std::conjunction<
		std::disjunction<
		std::is_same<T, typename T::value_type>, // avoid endless recursion
		expand_operator_equal<typename T::value_type>
		>, expand_operator_equal_tuple<T>>
	{};

	// recursively check the template arguments of a tuple like object
	template <typename ...T>
	using expand_operator_equal_recursive = std::conjunction<
		expand_operator_equal<T>...>;

	template <typename T>
	struct expand_operator_equal_tuple
		: has_operator_equal<T>
	{};

	template <typename T>
	struct expand_operator_equal_tuple
		<std::optional<T>> :
		expand_operator_equal_recursive<T>
	{};

	template <
		typename T1,
		typename T2>
	struct expand_operator_equal_tuple<
		std::pair<T1, T2>> :
		expand_operator_equal_recursive<T1, T2>
	{};

	template <typename ...T>
	struct expand_operator_equal_tuple<
		std::tuple<T...>> :
		expand_operator_equal_recursive<T...>
	{};

	template<typename ...T>
	struct expand_operator_equal_tuple<
		std::variant<T...>> :
		expand_operator_equal_recursive<T...>
	{};

	// the same for operator<(), see above for explanations
	template <
		typename,
		typename = void>
	struct has_operator_less_than
		: std::false_type
	{};

	template <typename T>
	struct has_operator_less_than<T, std::void_t<decltype(
		bool(std::declval<const T&>() < std::declval<const T&>()))>>
		: std::true_type
	{};

	template <
		typename T,
		bool = is_container<T>::value>
	struct expand_operator_less_than_container;

	template <typename T>
	struct expand_operator_less_than_tuple;

	template <typename T>
	using expand_operator_less_than = expand_operator_less_than_container<T>;

	template <
		typename T,
		bool>
	struct expand_operator_less_than_container
		: expand_operator_less_than_tuple<T>
	{};

	template <typename T>
	struct expand_operator_less_than_container<T, true> :
		std::conjunction<
		std::disjunction<
		std::is_same<T, typename T::value_type>,
		expand_operator_less_than<typename T::value_type>
			>, expand_operator_less_than_tuple<T>
		> {};

	template <typename ...T>
	using expand_operator_less_than_recursive =
		std::conjunction<expand_operator_less_than<T>...>;

	template <typename T>
	struct expand_operator_less_than_tuple
		: has_operator_less_than<T>
	{};

	template <typename T>
	struct expand_operator_less_than_tuple<
		std::optional<T>> :
		expand_operator_less_than_recursive<T>
	{};

	template <
		typename T1,
		typename T2>
	struct expand_operator_less_than_tuple<
		std::pair<T1, T2>> :
		expand_operator_less_than_recursive<T1, T2>
	{};

	template <typename ...T>
	struct expand_operator_less_than_tuple<
		std::tuple<T...>> :
		expand_operator_less_than_recursive<T...>
	{};

	template <typename ...T>
	struct expand_operator_less_than_tuple<
		std::variant<T...>> :
		expand_operator_less_than_recursive<T...>
	{};
}

template <
	typename T,
	typename = void>
struct is_dereferenceable
	: std::false_type 
{};

template<typename T>
struct is_dereferenceable<T, std::void_t<decltype(
		std::declval<T>().operator->())>>
    : std::true_type
{};

template <typename T>
inline constexpr bool is_dereferenceable_v = is_dereferenceable<T>::value;

template <typename T>
struct has_operator_equal 
	: detail::expand_operator_equal<T>
{};

template <typename T>
inline constexpr bool has_operator_equal_v = has_operator_equal<T>::value;

template <
	typename Container,
	typename T>
using has_operator_equal_container = 
	std::disjunction<
		std::is_base_of<
			Container, T>, 
	has_operator_equal<T>>;

template <typename T>
struct has_operator_less_than 
	: detail::expand_operator_less_than<T> 
{};

template <typename T>
inline constexpr bool has_operator_less_than_v = has_operator_less_than<T>::value;

template <
	typename Container,
	typename T>
using has_operator_less_than_container = 
	std::disjunction<
		std::is_base_of<
			Container, T>, 
	has_operator_less_than<T>>;

template <typename ...T>
using compare_eq_result = std::enable_if_t<
	std::conjunction_v<
		has_operator_equal<T>...>,
	bool>;

template <
	typename Container,
	typename ...T>
using compare_eq_result_container = std::enable_if_t<
	std::conjunction_v<
		has_operator_equal_container<
			Container, T>...>, 
	bool>;

template <typename ...T>
using compare_lt_result = std::enable_if_t<
	std::conjunction_v<
		has_operator_less_than<T>...>,
	bool>;

template <
	typename Container,
	typename ...T>
using compare_lt_result_container = std::enable_if_t<
	std::conjunction_v<
		has_operator_less_than_container<
			Container, T>...>, 
	bool>;

template <
	class _Element_, 
	bool _IsEnum_ = std::is_enum_v<_Element_>>
struct UnwrapEnum { 
	// if _Element_ is an enum, gets its underlying type;
	// otherwise leaves _Element_ unchanged

	using type = std::underlying_type_t<_Element_>;
};

template <class _Element_>
struct UnwrapEnum<_Element_, false> { 
	// passthrough non-enum type
	using type = _Element_;
};

template <class _Element_>
using unwrap_enum_t = typename UnwrapEnum<_Element_>::type;

__BASE_NAMESPACE_END
