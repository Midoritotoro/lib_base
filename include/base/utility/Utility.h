#pragma once

#include <base/system/Platform.h>
#include <base/Time.h>

#if defined(OS_WIN) && defined(CPP_MSVC) && !defined(__GNUC__) && !defined(__clang__)
#include <__msvc_iter_core.hpp>
#endif

#include <random>
#include <ranges>

#include <cstdlib>
#include <gsl/gsl>

#include <base/Assert.h>
#include <base/core/Types.h>

#include <iostream>




namespace base {
	using namespace ::std::ranges;

	

	struct rational_t {
		unsigned num, den;
	};

    struct node_t {
        char* key;
        struct node_t* llink, * rlink;
    };

    enum VISIT {
        preorder,
        postorder,
        endorder,
        leaf
    };

    typedef void (*cmp_fn_t)(const void*, VISIT, int);

	template<class T>
	struct Distribution {};

	template<>
	struct Distribution<int> {
		typedef ::std::uniform_int_distribution<int> type;
	};

	template<>
	struct Distribution<float> {
		typedef ::std::uniform_int_distribution<float> type;
	};

	template<>
	struct Distribution<double> {
		typedef ::std::uniform_real_distribution<double> type;
	};

	template<>
	struct Distribution<int64> {
		typedef ::std::uniform_int_distribution<int64> type;
	};

	[[nodiscard]] int64_t GCD(
		int64_t a,
		int64_t b);

	[[nodiscard]] int64_t LCM(
		int64_t a, 
		int64_t b);

	bool UnsignedReduce(
		unsigned* pi_dst_nom, unsigned* pi_dst_den,
		uint64_t i_nom, uint64_t i_den, uint64_t i_max);

	[[nodiscard]] double SafeRound(double value);

	template<typename I, typename S, typename T,
		typename Op = plus, typename P = identity>
		requires ::std::input_iterator<I>&&
	::std::sentinel_for<S, I>&&
		indirectly_binary_invocable<Op, T*, ::std::projected<I, P>>&&
		::std::assignable_from<T&, ::std::indirect_result_t<Op&,
		T*, ::std::projected<I, P>>>

		always_inline [[nodiscard]] T accumulate(
			I first,
			S last,
			T init,
			Op op = Op{},
			P proj = P{})
	{
		for (; first != last; ++first)
			init = std::invoke(op, init, std::invoke(proj, *first));
		return init;
	}

	template<typename Rng, typename T, typename Op = plus, typename P = identity>
		requires input_range<Rng>&&
	indirectly_binary_invocable<Op, T*,
		::std::projected<iterator_t<Rng>, P>>&&
		::std::assignable_from<T&, ::std::indirect_result_t<Op&, T*,
		::std::projected<iterator_t<Rng>, P>>>

		always_inline [[nodiscard]] T accumulate(
			Rng&& rng,
			T init,
			Op op = Op{},
			P proj = P{})
	{
		return (*this)(::std::ranges::begin(rng), ::std::ranges::end(rng),
			::std::move(init), ::std::move(op), ::std::move(proj));
	}

	template <typename T>
	always_inline void accumulateMax(T& a, const T& b) {
		if (a < b)
			a = b;
	}

	template <typename T>
	always_inline void accumulateMin(T& a, const T& b) {
		if (a > b)
			a = b;
	}

	template <typename T>
	always_inline [[nodiscard]] T&& take(T& value) {
		return ::std::exchange(value, T{});
	}

	template <typename T>
	always_inline [[nodiscard]] T randomNumber(
		T from,
		T to)
	{
		auto randomDevice = ::std::random_device();
		auto generator = ::std::mt19937(randomDevice());

		return Distribution<T>::type(from, to)(generator);
	}
#if defined(OS_WIN)
	[[nodiscard]] bool IsWindowsGreaterThen(int version);
	[[nodiscard]] bool SetAutoRunKey(
		const ::std::wstring& path,
		const ::std::wstring& key);

	#define MINIMUM_WINDOWS_VERSION		NTDDI_WIN10
	#define IS_MINIMUM_WINDOWS_VERSION	IsWindowsGreaterThen(MINIMUM_WINDOWS_VERSION)
#endif // OS_WIN
} // namespace base