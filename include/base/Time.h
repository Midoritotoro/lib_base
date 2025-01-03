#pragma once

#include "Platform.h"
#include <string>


namespace Time {
	using time = std::int64_t;
	using profileTime = std::int64_t;

	namespace details {

		using innerTime = std::int64_t;
		using innerProfile = std::int64_t;

		LIB_BASE void init();

		LIB_BASE [[nodiscard]] innerTime currentValue();
		LIB_BASE [[nodiscard]] time convert(innerTime value);

		LIB_BASE [[nodiscard]] innerProfile currentProfileValue();
		LIB_BASE [[nodiscard]] profileTime convertProfile(innerProfile);

	} // namespace details

	LIB_BASE [[nodiscard]] time now();
	LIB_BASE [[nodiscard]] profileTime profile();

	LIB_BASE bool adjustTime();

	LIB_BASE [[nodiscard]] std::string formattedUnixTime(int64_t unixTime);

	LIB_BASE [[nodiscard]] int minutes(int64_t unixTime);
	LIB_BASE [[nodiscard]] int hours(int64_t unixTime);
	LIB_BASE [[nodiscard]] int seconds(int64_t unixTime);
} // namespace Time