#pragma once

#include <base/Platform.h>

#if defined(_MSC_VER) && !defined(__llvm__) && !defined(__INTEL_COMPILER) \
	&& !defined(__GNUC__) && !defined(__clang__)
#include <__msvc_iter_core.hpp>
#endif

#include <ranges>
#include <iostream>

#include <cstdlib>

#if __has_include(<gsl/gsl>)
/**
 *После выхода из текущей области видимости определяет затраченное на выполнение этого блока кода время
 *\param name - "Имя" области видимости
*/
#define measureExecutionTime(name) \
		 const auto ms = Time::now(); \
		 const auto timer = gsl::finally([] { \
			std::cout << name << " completed for: " \
			<< Time::now() - ms << " ms" << '\n'; });
#endif

#define container_of(ptr, type, member) \
    ((type *)(((char *)(ptr)) - offsetof(type, member)))


namespace base {
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

    [[nodiscard]] void* TFind(
        const void* vkey,
        void* const* vrootp,
        int (*compar)(const void*,
            const void*));

    [[nodiscard]] void* TSearch(
        const void* vkey,
        void** vrootp,
        int (*compar)(const void*,
            const void*));

    void TRecurse(
        const node_t* root,
        cmp_fn_t action, int level);

    /**
     *Обходит узлы дерева
     *\param vroot - Дерево
     *\param action - Узел дерева для обхода
     */
    void TWalk(
        const void* vroot,
        cmp_fn_t action);

    void TDestroyRecurse(
        node_t* root,
        void (*free_action)(void*));

    void TDestroy(
        void* root,
        void (*freenode)(void*));

	template<typename I, typename S, typename T,
		typename Op = plus, typename P = identity>
		requires std::input_iterator<I>&&
	std::sentinel_for<S, I>&&
		indirectly_binary_invocable<Op, T*, std::projected<I, P>>&&
		std::assignable_from<T&, std::indirect_result_t<Op&,
		T*, std::projected<I, P>>>

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
		std::projected<iterator_t<Rng>, P>>&&
		std::assignable_from<T&, std::indirect_result_t<Op&, T*,
		std::projected<iterator_t<Rng>, P>>>

		always_inline [[nodiscard]] T accumulate(
			Rng&& rng,
			T init,
			Op op = Op{},
			P proj = P{})
	{
		return (*this)(std::ranges::begin(rng), std::ranges::end(rng),
			std::move(init), std::move(op), std::move(proj));
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
		return std::exchange(value, T{});
	}

#ifdef _WIN32
	[[nodiscard]] bool IsWindowsGreaterThen(int version);
	[[nodiscard]] bool SetAutoRunKey(LPWSTR path, LPWSTR key);

	#define MINIMUM_WINDOWS_VERSION NTDDI_WIN10
	#define IS_MINIMUM_WINDOWS_VERSION IsWindowsGreaterThen(MINIMUM_WINDOWS_VERSION)

	[[nodiscard]] bool addToAutoRun(LPWSTR key);
#endif // _WIN32
} // namespace base