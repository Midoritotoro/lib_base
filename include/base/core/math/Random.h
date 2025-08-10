#pragma once 

#include <random>
#include <base/core/Types.h>

#include <base/core/arch/KeywordSupport.h>
#include <type_traits>


__BASE_NAMESPACE_BEGIN

class Random {
	template<
		typename Integer = int,
		typename = std::enable_if_t<
			std::_Is_any_of_v<Integer,
				short, int, long, longlong,
				ushort, uint, ulong,
				ulonglong>>>

	struct RandomDistribution {
		static_assert(!std::_Is_character<Integer>);
		using type = std::uniform_int_distribution<Integer>;
	};
public: 
	template <typename T>
	static inline   T randomNumber(
		T from,
		T to)
	{
		auto randomDevice = ::std::random_device();
		auto generator = ::std::mt19937(randomDevice());

		return RandomDistribution<T>::type(from, to)(generator);
	}
};

__BASE_NAMESPACE_END