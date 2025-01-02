#pragma once 


#include "base/Utility.h"

#include <ranges>
#include <__msvc_iter_core.hpp>


namespace core::utility {
	using namespace std::ranges;
	template <typename F, typename Type, typename Type2>
	concept indirectly_binary_invocable =
		std::indirectly_readable<Type> &&
		std::indirectly_readable<Type2> &&
		std::copy_constructible<F>;

	struct plus
	{
		template<typename T, typename U>
		constexpr auto operator()(T&& t, U&& u) const -> decltype((T&&)t + (U&&)u) {
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

	struct accumulate_fn {

		template<typename I, typename S, typename T,
			typename Op = plus, typename P = identity>

			requires std::input_iterator<I>									&&
				std::sentinel_for<S, I>										&&
				indirectly_binary_invocable<Op, T*, std::projected<I, P>>	&&
				std::assignable_from<T&, std::indirect_result_t<Op&,
					T*, std::projected<I, P>>>

			T operator()(
				I first,
				S last,
				T init,
				Op op = Op{},
				P proj = P{}) const 
		{
			for (; first != last; ++first)
				init = std::invoke(op, init, std::invoke(proj, *first));
			return init;
		}

		template<typename Rng, typename T, typename Op = plus, typename P = identity>
			requires input_range<Rng>				&&
				indirectly_binary_invocable<Op, T*,
				std::projected<iterator_t<Rng>, P>> &&
				std::assignable_from<T&, std::indirect_result_t<Op&, T*,
				std::projected<iterator_t<Rng>, P>>>

			T operator()(
				Rng&& rng,
				T init,
				Op op = Op{},
				P proj = P{}) const
		{
			return (*this)(std::ranges::begin(rng), std::ranges::end(rng),
				std::move(init), std::move(op), std::move(proj));
		}
	};

	inline constexpr accumulate_fn accumulate{};

	template <typename T>
	inline void accumulateMax(T& a, const T& b) {
		if (a < b) a = b;
	}

	template <typename T>
	inline void accumulateMin(T& a, const T& b) {
		if (a > b) a = b;
	}
} // namespace core::utility