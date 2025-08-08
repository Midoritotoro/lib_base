#pragma once

#include <base/core/compatibility/CompilerDetection.h>
#include <base/core/arch/SystemDetection.h>

#if defined(base_os_windows) && defined(base_cpp_msvc) && !defined(base_cpp_gnu) && !defined(base_cpp_clang)
#include <__msvc_iter_core.hpp>
#endif

#if base_has_cxx20
	#include <ranges>
#endif

#include <base/core/utility/Execution.h>
#include <base/core/utility/TypeTraits.h>

//#include <src/core/utility/algorithm/AllOf.h>
//#include <src/core/utility/algorithm/NoneOf.h>

//#include <src/core/utility/algorithm/Copy.h>
//#include <src/core/utility/algorithm/Count.h>
//
//#include <src/core/utility/algorithm/Fill.h>
//#include <src/core/utility/algorithm/Find.h>
//
//#include <src/core/utility/algorithm/ForEach.h>
//#include <src/core/utility/algorithm/Mismatch.h>
//
//#include <src/core/utility/algorithm/Move.h>
//#include <src/core/utility/algorithm/PartialSort.h>

//#include <src/core/utility/algorithm/AnyOf.h>
//#include <src/core/utility/algorithm/Remove.h>
//
//#include <src/core/utility/algorithm/Replace.h>
//#include <src/core/utility/algorithm/Reverse.h>
//
//#include <src/core/utility/algorithm/ReverseCopy.h>
//#include <src/core/utility/algorithm/Rotate.h>
//
//#include <src/core/utility/algorithm/Search.h>
//#include <src/core/utility/algorithm/Sort.h>
//
//#include <src/core/utility/algorithm/SwapRanges.h>
//#include <src/core/utility/algorithm/Transform.h>

__BASE_NAMESPACE_BEGIN

using namespace ::std::ranges;

template<typename I, typename S, typename T,
	typename Op = plus, typename P = identity>
	requires ::std::input_iterator<I>&&
::std::sentinel_for<S, I>&&
	indirectly_binary_invocable<Op, T*, ::std::projected<I, P>>&&
	::std::assignable_from<T&, ::std::indirect_result_t<Op&,
	T*, ::std::projected<I, P>>>
base_always_inline base_nodiscard T accumulate(
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

base_always_inline base_nodiscard T accumulate(
	Rng&& rng,
	T init,
	Op op = Op{},
	P proj = P{})
{
	return (*this)(::std::ranges::begin(rng), ::std::ranges::end(rng),
		::std::move(init), ::std::move(op), ::std::move(proj));
}

template <typename T>
base_always_inline void accumulateMax(T& a, const T& b) {
	if (a < b)
		a = b;
}

template <typename T>
base_always_inline void accumulateMin(T& a, const T& b) {
	if (a > b)
		a = b;
}

template <typename T>
base_always_inline base_nodiscard T&& take(T& value) {
	return ::std::exchange(value, T{});
}

#if defined(base_os_windows)
	base_nodiscard bool IsWindowsGreaterThen(int version);
	base_nodiscard bool SetAutoRunKey(
		const ::std::wstring& path,
		const ::std::wstring& key);

	#define MINIMUM_WINDOWS_VERSION		NTDDI_WIN10
	#define IS_MINIMUM_WINDOWS_VERSION	IsWindowsGreaterThen(MINIMUM_WINDOWS_VERSION)
#endif // base_os_windows

__BASE_NAMESPACE_END
