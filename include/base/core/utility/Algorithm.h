#pragma once

#include <base/core/arch/Platform.h>

#if defined(OS_WIN) && defined(CPP_MSVC) && !defined(__GNUC__) && !defined(__clang__)
#include <__msvc_iter_core.hpp>
#endif

#include <ranges>
#include <base/core/Types.h>

#include <base/core/utility/TypeTraits.h>


__BASE_NAMESPACE_BEGIN

using namespace ::std::ranges;

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
#if defined(OS_WIN)
	[[nodiscard]] bool IsWindowsGreaterThen(int version);
	[[nodiscard]] bool SetAutoRunKey(
		const ::std::wstring& path,
		const ::std::wstring& key);

	#define MINIMUM_WINDOWS_VERSION		NTDDI_WIN10
	#define IS_MINIMUM_WINDOWS_VERSION	IsWindowsGreaterThen(MINIMUM_WINDOWS_VERSION)
#endif // OS_WIN

template<int ...>
struct seq { };

template<int N, int ...S>
struct gens : gens<N-1, N-1, S...> {};

template<int ...S>
struct gens<0, S...> {
    typedef seq<S...> type;
};

template <typename... Ts>
struct tuple_reverse;

template <>
struct tuple_reverse<std::tuple<>>
{
    using type = std::tuple<>;
};

template <typename T, typename... Ts>
struct tuple_reverse<std::tuple<T, Ts...>>
{
	using head = std::tuple<T>;
	using tail = typename tuple_reverse<
		std::tuple<Ts...>>::type;

	using type = decltype(
		std::tuple_cat(
		std::declval<tail>(),
			std::declval<head>()));
};

namespace TupleReverseImpl {


// Helper functions to recursivly copy the source tuple to the target tuple
template <class ...Args>
void doCopyTypes( std::vector<void*>::size_type index,
                  std::vector<const void*> & content_source,
                  std::vector<void*> & content_target ) {}

template <class T, class ...Args>
void doCopyTypes( std::vector<void*>::size_type index,
                  std::vector<const void*> & content_source,
                  std::vector<void*> & content_target,
                  const T * t,
                  Args... args )
{
  // target and source vector do have the types in the same order
  // so we can cast here to the correct type, then copy it
  T * ptrSource = reinterpret_cast<T*>(
	  const_cast<void*>(content_source.at(index)));
  T * ptrTarget = reinterpret_cast<T*>(
	  content_target.at(index));

  *ptrTarget = *ptrSource;

  doCopyTypes(
	  index+1, content_source, 
	  content_target, args... );
}

template <class Tuple, int ...S>
void copyTypes( std::vector<const void*> & content_source,
                std::vector<void*> & content_target,
                Tuple & tuple,
                seq<S...> )
{
  doCopyTypes(
	  0, content_source,
	  content_target, &std::get<S>(tuple)... );
}

template <class V>
void dofillContent( V & content ) {}

template <class V, class T, class ...Args>
void dofillContent( V & content, T * t, Args... args )
{
  content.push_back( t );

  dofillContent( content, args... );
}

template <class V,class Tuple, int ...S>
void fill( V & content, Tuple & tuple, seq<S...> )
{
  dofillContent(content, &std::get<S>(tuple)...);
}

} // namespace TupleReverseImpl

// Reverses a tuple and its content
template <class Tuple>
typename tuple_reverse<Tuple>::type reverseTuple( const Tuple & tuple )
{
    // declare return type
    using _ReversedTupleType_ = typename tuple_reverse<Tuple>::type;
	_ReversedTupleType_ return_tuple;

    // create source and target pointer vectors for the copy action
    std::vector<const void*> contentSource;
    std::vector<void*> contentTarget;

    TupleReverseImpl::fill(
		contentSource, tuple, 
		typename gens<std::tuple_size<Tuple>::value>::type());
    TupleReverseImpl::fill(
		contentTarget, return_tuple,
		typename gens<std::tuple_size<_ReversedTupleType_>::value>::type());

    // to be in the same order as contentTarget
    std::reverse(contentTarget.begin(), contentTarget.end());

    // now copy everything
    TupleReverseImpl::copyTypes(
		contentSource, contentTarget, tuple, 
		typename SequenceHelper::gens<std::tuple_size<Tuple>::value>::type());

    return return_tuple;
}

__BASE_NAMESPACE_END
