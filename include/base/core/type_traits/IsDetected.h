#pragma once 

#include <base/core/BaseNamespace.h>
#include <type_traits>


__BASE_TYPE_TRAITS_NAMESPACE_BEGIN

struct _Nonesuch {
	~_Nonesuch() = delete;
	_Nonesuch(const _Nonesuch&) = delete;
	void operator=(const _Nonesuch&) = delete;
};


template <
	typename						_Type_,
	typename						_Void_,
	template <typename...> class	_Op_,
	typename...						_Args_>
struct _Detector 
{
	using value_t = std::false_type;
	using type = _Type_;
};

template <
	typename						_Type_,
	template <typename...> class	_Op_,
	typename...						_Args_>
struct _Detector<
	_Type_,
	std::void_t<_Op_<_Args_...>>,
	_Op_,
	_Args_...> 
{
	using value_t = std::true_type;
	using type = _Op_<_Args_...>;
};


template <
	template <typename...> class	_Op_,
	typename...						_Args_>
using is_detected = typename _Detector<
	_Nonesuch, void,
	_Op_, _Args_...>::value_t;

template <
	template <typename...> class	_Op_,
	typename...						_Args_>
constexpr inline bool is_detected_v = is_detected<_Op_, _Args_...>::value;


__BASE_TYPE_TRAITS_NAMESPACE_END
