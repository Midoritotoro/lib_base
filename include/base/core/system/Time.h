#pragma once

#include <base/core/arch/Platform.h>

#include <string>
#include <ctime>


__BASE_TIME_NAMESPACE_BEGIN

using time_t = std::int64_t;
using profileTime_t = std::int64_t;

namespace details {

	using innerTime_t = std::int64_t;
	using innerProfile_t = std::int64_t;

	void init();

	[[nodiscard]] innerTime_t currentValue();
	[[nodiscard]] time_t convert(innerTime_t value);

	[[nodiscard]] innerProfile_t currentProfileValue();
	[[nodiscard]] profileTime_t convertProfile(innerProfile_t);

} // namespace details

[[nodiscard]] time_t now();
[[nodiscard]] profileTime_t profile();

bool adjustTime();

[[nodiscard]] std::string formattedUnixTime(int64_t unixTime);

[[nodiscard]] int minutes(int64_t unixTime);
[[nodiscard]] int hours(int64_t unixTime);
[[nodiscard]] int seconds(int64_t unixTime);

__BASE_TIME_NAMESPACE_END
