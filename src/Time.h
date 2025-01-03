#pragma once

#include "base/Time.h"
#include <string>


namespace Time {
	using time = std::int64_t;
	using profileTime = std::int64_t;

	namespace details {

		using innerTime = std::int64_t;
		using innerProfile = std::int64_t;

		void init();

		[[nodiscard]] innerTime currentValue();
		[[nodiscard]] time convert(innerTime value);

		[[nodiscard]] innerProfile currentProfileValue();
		[[nodiscard]] profileTime convertProfile(innerProfile);

	} // namespace details

	[[nodiscard]] time now();
	[[nodiscard]] profileTime profile();

	bool adjustTime();

	[[nodiscard]] std::string formattedUnixTime(int64_t unixTime);

	[[nodiscard]] int minutes(int64_t unixTime);
	[[nodiscard]] int hours(int64_t unixTime);
	[[nodiscard]] int seconds(int64_t unixTime);
} // namespace Time