#pragma once

#include <base/system/Platform.h>
#include <base/Time.h>

#if defined(OS_WIN) && defined(_MSC_VER) && !defined(__GNUC__) && !defined(__clang__)
#include <__msvc_iter_core.hpp>
#endif

#include <random>
#include <ranges>

#include <cstdlib>
#include <gsl/gsl>

#include <iostream>


/**
 *После выхода из текущей области видимости выводит затраченное на выполнение этого блока кода время
 *\param name - "Имя" области видимости
*/
#define measureExecutionTime(name) \
		 const auto ms = base::Time::now(); \
		 const auto timer = gsl::finally([=] { \
			std::cout << name << " completed for: " \
			<< base::Time::now() - ms << " ms" << '\n'; });
//!
//! \brief
//! После выхода из текущей области видимости записывает затраченное на выполнение этого блока кода время в value
#define measureExecutionTimeToValue(value) \
		 const auto ms = base::Time::now(); \
		 const auto timer = gsl::finally([&]() mutable { \
			value = base::Time::now() - ms; });

//!
//! \brief
//! Проверяет, является ли ptr с названием member членом структуры типа type
//! \param ptr - Указатель на член структуры (типа member).
//! \param type - Тип структуры, которая содержит member.
//! \param member - Имя члена структуры, на который указывает ptr.
#define container_of(ptr, type, member) \
    ((type *)(((char *)(ptr)) - offsetof(type, member)))


#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#define free_null(a) \
	do { \
		free( a ); \
		a = nullptr; \
	} while(0)

#define delete_null(ptr) \
	do { \
		delete ptr; \
		ptr = nullptr; \
	} while (0)

#define delete_array_null(arr_ptr) \
	do { \
		delete[] arr_ptr; \
		arr_ptr = nullptr; \
	} while (0)

#define empty_str(str) (!str || !*str)

#define stringify(x)	#x


#ifndef LIB_BASE_ENABLE_QT
	#define DISABLE_COPY(Class) \
		Class(const Class&) = delete;\
		Class& operator=(const Class&) = delete;

	#define DISABLE_COPY_MOVE(Class) \
		DISABLE_COPY(Class) \
		Class(Class&&) = delete; \
		Class &operator=(Class &&) = delete;
#else
	#define DISABLE_COPY Q_DISABLE_COPY
	#define DISABLE_COPY_MOVE Q_DISABLE_COPY_MOVE
#endif 

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
	struct Distribution<double> {
		typedef ::std::uniform_real_distribution<double> type;
	};

	template<>
	struct Distribution<int64_t> {
		typedef ::std::uniform_int_distribution<int64_t> type;
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
		const std::wstring& path, 
		const std::wstring& key);

	#define MINIMUM_WINDOWS_VERSION NTDDI_WIN10
	#define IS_MINIMUM_WINDOWS_VERSION IsWindowsGreaterThen(MINIMUM_WINDOWS_VERSION)
#endif // OS_WIN
} // namespace base