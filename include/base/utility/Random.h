#pragma once 

#include <random>
#include <base/core/Types.h>


namespace base {
	template<class T> struct Distribution {};

	template<>  struct Distribution<int> { typedef ::std::uniform_int_distribution<int> type; };
	template<>  struct Distribution<uint> { typedef ::std::uniform_int_distribution<uint> type; };

	template<>  struct Distribution<float> { typedef ::std::uniform_int_distribution<float> type; };
	template<>	struct Distribution<double> { typedef ::std::uniform_real_distribution<double> type; };

	template<>	struct Distribution<int64> { typedef ::std::uniform_int_distribution<int64> type; };
	template<>	struct Distribution<short> { typedef ::std::uniform_int_distribution<short> type; };

	template<>	struct Distribution<long> { typedef ::std::uniform_int_distribution<long> type; };
	template<>	struct Distribution<ushort> { typedef ::std::uniform_int_distribution<ushort> type; };

	template<>	struct Distribution<ulong> { typedef ::std::uniform_int_distribution<ulong> type; };
	template<>	struct Distribution<ulonglong> { typedef ::std::uniform_int_distribution<ulonglong> type; };

//
//#define _RNG_REQUIRE_INTTYPE(_RandType, _CheckedType)                                                                  \
//    static_assert(_Is_any_of_v<_CheckedType, short, int, long, long long, unsigned short, unsigned int, unsigned long, \
//                      unsigned long long>,                                                                             \
//        "invalid template argument for " #_RandType                                                                    \
//        ": N4950 [rand.req.genl]/1.5 requires one of short, int, long, long long, unsigned short, unsigned int, "      \
//        "unsigned long, or unsigned long long");                                                                       \
//    _RNG_PROHIBIT_CHAR(_CheckedType)

	class Random {
	public: 

	};
} // namespace base