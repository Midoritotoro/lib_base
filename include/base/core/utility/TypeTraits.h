#pragma once 

#include <type_traits>
#include <base/core/utility/Concepts.h>

#include <optional>
#include <variant>

#include <base/core/compatibility/Warnings.h>

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


#ifdef __cpp_lib_remove_cvref
using std::remove_cvref;
using std::remove_cvref_t;
#else
template <typename T>
using remove_cvref = std::remove_cv<std::remove_reference_t<T>>;
template <typename T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;
#endif // __cpp_lib_remove_cvref



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
struct IsCharacterOrByteOrBool<std::byte> : 
	std::true_type 
{};

#endif // defined(__cpp_lib_byte)

// --------------------------------------------------


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

#if base_has_cxx20

	template <class _Iterator_>
	using IteratorReferenceType		= std::iter_reference_t<_Iterator_>;

	template <class _Iterator_>
	using IteratorValueType			= std::iter_value_t<_Iterator_>;

	template <class _Iterator_>
	using IteratorDifferenceType	= std::iter_difference_t<_Iterator_>;

#else

	template <class _Iterator_>
	using IteratorReferenceType		= typename std::iterator_traits<_Iterator_>::reference;

	template <class _Iterator_>
	using IteratorValueType			= typename std::iterator_traits<_Iterator_>::value_type;

	template <class _Iterator_>
	using IteratorDifferenceType	= typename std::iterator_traits<_Iterator_>::difference_type;

#endif

template <
    std::size_t A,
    std::size_t B>
struct MinimumSize: 
    std::integral_constant<std::size_t, (A < B) ? A : B> 
{};

template <
    std::size_t A,
    std::size_t B>
struct MaximumSize: 
    std::integral_constant<std::size_t, (A > B) ? A : B> 
{};

template <class _Type_>
struct MaximumObjects: 
    std::integral_constant<std::size_t,
        ~static_cast<std::size_t>(0) / sizeof(_Type_)> 
{};

#if defined(base_os_windows) && defined(base_cpp_msvc)
    template <
        typename _Type_,
        class   _Allocator_>
    constexpr bool CanDestroyRange  = !std::conjunction_v<
        std::is_trivially_destructible<_Type_>,
        std::_Uses_default_destroy<_Allocator_, _Type_*>>;
#else 
    template <
        typename _Type_,
        class   _Allocator_>
    constexpr bool CanDestroyRange = !std::is_trivially_destructible_v<_Type_>;
#endif

template <
    typename    _Type_,
    class       _Allocator_>
constexpr bool IsNoThrowMoveConstructible =
    #if defined(base_os_windows) && defined(base_cpp_msvc)
		    std::conjunction_v<
			    std::is_nothrow_move_constructible<_Type_>,
			    std::_Uses_default_construct<_Allocator_, _Type_*, _Type_>>;
    #else
		    std::is_nothrow_move_constructible_v<_Allocator_>;
    #endif

template <
    typename    _Type_,
    class       _Allocator_,
    class ...   _Args_>
constexpr bool IsNoThrowMoveConstructibleArgs =
    #if defined(base_os_windows) && defined(base_cpp_msvc)
		    std::conjunction_v<
			    std::is_nothrow_move_constructible<_Type_>,
			    std::_Uses_default_construct<_Allocator_, _Type_*, _Args_...>>;
    #else
		    std::is_nothrow_move_constructible_v<_Allocator_>;
    #endif

template <
    class _Source_, 
    class _Destination_>
// checks the convertibility of _Source_ to _Destination_
constexpr bool IsPointerAddressConvertible = std::is_void_v<_Source_>
    || std::is_void_v<_Destination_>
    // is_same_v is required for function pointers to work
    || std::is_same_v<
        std::remove_cv_t<_Source_>, 
        std::remove_cv_t<_Destination_>>
#ifdef __cpp_lib_is_pointer_interconvertible
    || std::is_pointer_interconvertible_base_of_v<_Destination_, _Source_>
#endif
    ;

namespace HasADLSwapDetail {
#if defined(base_cpp_clang) || defined(__EDG__)
	void swap() = delete; // Block unqualified name lookup
#else
	void swap();
#endif

	template <
		class,
		class = void>
	struct HasADLSwap : 
		std::false_type 
	{};

	template <class _Type_>
	struct HasADLSwap < _Type_, std::void_t<decltype(std::swap(
		std::declval<_Type_&>(), 
		std::declval<_Type_&>())) >>: 
			std::true_type 
	{};
} // namespace _Has_ADL_swap_detail

template <class _Type_>
constexpr bool IsTriviallySwappable_v = std::conjunction_v<
	std::is_trivially_destructible<_Type_>,
	std::is_trivially_move_constructible<_Type_>,
		std::is_trivially_move_assignable<_Type_>, 
	std::negation<HasADLSwapDetail::HasADLSwap<_Type_>>>;

#ifdef __cpp_lib_byte
template <>
inline constexpr bool IsTriviallySwappable_v<std::byte> = true;
#endif // defined(__cpp_lib_byte)

template <class _Type_>
// true_type if and only if it is valid to swap two _Ty lvalues by exchanging object representations.
struct IsTriviallySwappable : 
	std::bool_constant<IsTriviallySwappable_v<_Type_>> 
{};


template <
	class _Ty,
	class... _Types>
constexpr bool IsAnyOf_v = // true if and only if _Ty is in _Types
#if base_has_cxx17
    (std::is_same_v<_Ty, _Types> || ...);
#else
    std::disjunction_v<std::is_same<_Ty, _Types>...>;
#endif

//template <class _Iterator_>
//constexpr bool isIteratorForwardCxx17 = std::is_convertible_v<memory::IteratorCategory<_Iterator_>, std::forward_iterator_tag>;
//
//template <class _Iter>
//constexpr bool _Is_ranges_fwd_iter_v =
//#if _HAS_CXX20
//    forward_iterator<_Iter> ||
//#endif
//    _Is_cpp17_fwd_iter_v<_Iter>;

__BASE_NAMESPACE_END
