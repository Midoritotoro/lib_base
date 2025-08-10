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

	  innerTime_t currentValue();
	  time_t convert(innerTime_t value);

	  innerProfile_t currentProfileValue();
	  profileTime_t convertProfile(innerProfile_t);

} // namespace details

  time_t now();
  profileTime_t profile();

bool adjustTime();

  std::string formattedUnixTime(int64_t unixTime);

  int minutes(int64_t unixTime);
  int hours(int64_t unixTime);
  int seconds(int64_t unixTime);

__BASE_TIME_NAMESPACE_END
