#pragma once

#include <base/Platform.h>

#include <string>
#include <ctime>


namespace Time {
	using time_t = std::int64_t;
	using profileTime_t = std::int64_t;

	namespace details {

		using innerTime_t = std::int64_t;
		using innerProfile_t = std::int64_t;

		LIB_BASE void init();

		LIB_BASE [[nodiscard]] innerTime_t currentValue();
		LIB_BASE [[nodiscard]] time_t convert(innerTime_t value);

		LIB_BASE [[nodiscard]] innerProfile_t currentProfileValue();
		LIB_BASE [[nodiscard]] profileTime_t convertProfile(innerProfile_t);

	} // namespace details

	LIB_BASE [[nodiscard]] time_t now();
	LIB_BASE [[nodiscard]] profileTime_t profile();

	LIB_BASE bool adjustTime();

	LIB_BASE [[nodiscard]] std::string formattedUnixTime(int64_t unixTime);

	LIB_BASE [[nodiscard]] int minutes(int64_t unixTime);
	LIB_BASE [[nodiscard]] int hours(int64_t unixTime);
	LIB_BASE [[nodiscard]] int seconds(int64_t unixTime);
} // namespace Time