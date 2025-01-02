#include "Time.h"

#include <atomic>
#include <ctime>

#include <limits>
#include <type_traits>

#include <cassert>
#include <iostream>

#include <Windows.h>

#ifdef max
	#undef max
#endif // max


namespace Time {
	namespace details {
		namespace {

			time LastAdjustmentTime;
			std::time_t LastAdjustmentUnixtime;

			using seconds_type = std::uint32_t;
			std::atomic<seconds_type> AdjustSeconds;

			innerTime StartValue;
			innerProfile StartProfileValue;

			double Frequency;
			double ProfileFrequency;

			struct StaticInit {
				StaticInit();
			};

			StaticInit::StaticInit() {
				StartValue = currentValue();
				StartProfileValue = currentProfileValue();

				init();
				LastAdjustmentUnixtime = ::time(nullptr);
			}

			StaticInit StaticInitObject;

			bool adjustTime() {
				const auto now = Time::now();
				const auto delta = (now - LastAdjustmentTime);

				const auto unixtime = ::time(nullptr);

				const auto real = (unixtime - LastAdjustmentUnixtime);
				const auto seconds = (time(real) * 1000 - delta) / 1000;

				LastAdjustmentUnixtime = unixtime;
				LastAdjustmentTime = now;

				if (seconds <= 0) {
					return false;
				}
				auto current = seconds_type(0);

				static constexpr auto maxTime = std::numeric_limits<seconds_type>::max();

				while (true) {
					if (time(current) + seconds > time(maxTime))
						return false;

					const auto next = current + seconds_type(seconds);
					if (AdjustSeconds.compare_exchange_weak(current, next))
						return true;
				}
				return false;
			}

			time computeAdjustment() {
				return time(AdjustSeconds.load()) * 1000;
			}

			profileTime computeProfileAdjustment() {
				return computeAdjustment() * 1000;
			}
		} // namespace 

		void init() {
			LARGE_INTEGER value;
			QueryPerformanceFrequency(&value);
			Frequency = 1000. / double(value.QuadPart);
			ProfileFrequency = 1000000. / double(value.QuadPart);
		}

		innerTime currentValue() {
			LARGE_INTEGER value;
			QueryPerformanceCounter(&value);
			return value.QuadPart;
		}

		time convert(innerTime value) {
			return time(value * Frequency);
		}

		innerProfile currentProfileValue() {
			LARGE_INTEGER value;
			QueryPerformanceCounter(&value);
			return value.QuadPart;
		}

		profileTime convertProfile(innerProfile value) {
			return profileTime(value * ProfileFrequency);
		}

	} // namespace details

	time now() {
		const auto elapsed = details::currentValue() - details::StartValue;
		return details::convert(elapsed) + details::computeAdjustment();
	}

	profileTime profile() {
		const auto elapsed = details::currentProfileValue()
			- details::StartProfileValue;
		return details::convertProfile(elapsed)
			+ details::computeProfileAdjustment();
	}

	bool adjustTime() {
		return details::adjustTime();
	}

	QString formattedUnixTime(int64_t unixTime) {
		char buffer[20];
		strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&unixTime));

		return buffer;
	}

	int minutes(int64_t unixTime) {
		return localtime(&unixTime)->tm_min;
	}

	int hours(int64_t unixTime) {
		return localtime(&unixTime)->tm_hour;
	}

	int seconds(int64_t unixTime) {
		return localtime(&unixTime)->tm_sec;
	}
} // namespace Time