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

	base_nodiscard innerTime_t currentValue();
	base_nodiscard time_t convert(innerTime_t value);

	base_nodiscard innerProfile_t currentProfileValue();
	base_nodiscard profileTime_t convertProfile(innerProfile_t);

} // namespace details

base_nodiscard time_t now();
base_nodiscard profileTime_t profile();

bool adjustTime();

base_nodiscard std::string formattedUnixTime(int64_t unixTime);

base_nodiscard int minutes(int64_t unixTime);
base_nodiscard int hours(int64_t unixTime);
base_nodiscard int seconds(int64_t unixTime);

__BASE_TIME_NAMESPACE_END
