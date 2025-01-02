#pragma once 

#include "Config.h"
#include "../../../core/Types.h"


#define container_of(ptr, type, member) \
    ((type *)(((char *)(ptr)) - offsetof(type, member)))

namespace FFmpeg {
	struct rational_t {
		unsigned num, den;
	};

	[[nodiscard]] int64_t GCD(
		int64_t a,
		int64_t b);

	[[nodiscard]] int LCM(int a, int b);

	bool UReduce(
		unsigned* pi_dst_nom, unsigned* pi_dst_den,
		uint64_t i_nom, uint64_t i_den, uint64_t i_max);
} // namespace FFmpeg