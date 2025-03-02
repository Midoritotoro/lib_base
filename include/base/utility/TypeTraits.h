#pragma once 

#include <type_traits>
#include <ranges>

#include <base/system/Platform.h>


namespace base {
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
			template <typename T, typename Void, template <typename...> class Op, typename...Args>
			struct detector {
				using value_t = std::false_type;
				using type = T;
			};
			template <typename T, template <typename...> class Op, typename...Args>
			struct detector<T, std::void_t<Op<Args...>>, Op, Args...> {
				using value_t = std::true_type;
				using type = Op<Args...>;
			};
		} // namespace _detail

		template <template <typename...> class Op, typename...Args>
		using is_detected = typename _detail::detector<nonesuch, void, Op, Args...>::value_t;

		template <template <typename...> class Op, typename...Args>
		constexpr inline bool is_detected_v = is_detected<Op, Args...>::value;


		// is_virtual_base_of_v<B, D> is true if and only if B is a virtual base class of D.
		// Just like is_base_of:
		// * only works on complete types;
		// * B and D must be class types;
		// * ignores cv-qualifications;
		// * B may be inaccessibile.

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
				// Base is a base class of Derived.
				std::is_base_of<Base, Derived>,

				// Check that Derived* can be converted to Base*, ignoring
				// accessibility. If this is possible, then Base is
				// an unambiguous base of Derived (=> virtual bases are always
				// unambiguous).
				is_detected<is_virtual_base_conversion_test, Derived, Base>,

				// Check that Base* can _not_ be converted to Derived*,
				// again ignoring accessibility. This seals the deal:
				// if this conversion cannot happen, it means that Base is an
				// ambiguous base and/or it is a virtual base.
				// But we have already established that Base is an unambiguous
				// base, hence: Base is a virtual base.
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
			std::is_convertible<typename std::iterator_traits<Iterator>::iterator_category, std::input_iterator_tag>::value,
			bool>::type;

		template <typename Iterator>
		using IfIsForwardIterator = typename std::enable_if<
			std::is_convertible<typename std::iterator_traits<Iterator>::iterator_category, std::forward_iterator_tag>::value,
			bool>::type;

		template <typename Iterator>
		using IfIsNotForwardIterator = typename std::enable_if<
			!std::is_convertible<typename std::iterator_traits<Iterator>::iterator_category, std::forward_iterator_tag>::value,
			bool>::type;
} // namespace base