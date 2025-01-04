#pragma once 

#include <base/Platform.h>


namespace base {
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
} // namespace base