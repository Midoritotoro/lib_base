#include <base/system/Time.h>

#include <atomic>
#include <limits>

WARNING_DISABLE_MSVC(4996)

namespace base::Time {
	namespace details {
		namespace {

			time_t LastAdjustmentTime;
			std::time_t LastAdjustmentUnixtime;

			using seconds_type = std::uint32_t;
			std::atomic<seconds_type> AdjustSeconds;

			innerTime_t StartValue;
			innerProfile_t StartProfileValue;

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
				const auto seconds = (time_t(real) * 1000 - delta) / 1000;

				LastAdjustmentUnixtime = unixtime;
				LastAdjustmentTime = now;

				if (seconds <= 0)
					return false;
				
				auto current = seconds_type(0);

				static constexpr auto maxTime = std::numeric_limits<seconds_type>::max();

				while (true) {
					if (time_t(current) + seconds > time_t(maxTime))
						return false;

					const auto next = current + seconds_type(seconds);
					if (AdjustSeconds.compare_exchange_weak(current, next))
						return true;
				}

				return false;
			}

			time_t computeAdjustment() {
				return time_t (AdjustSeconds.load()) * 1000;
			}

			profileTime_t computeProfileAdjustment() {
				return computeAdjustment() * 1000;
			}
		} // namespace 

		void init() {
			LARGE_INTEGER value;
			QueryPerformanceFrequency(&value);
			Frequency = 1000. / double(value.QuadPart);
			ProfileFrequency = 1000000. / double(value.QuadPart);
		}

		innerTime_t currentValue() {
			LARGE_INTEGER value;
			QueryPerformanceCounter(&value);
			return value.QuadPart;
		}

		time_t convert(innerTime_t value) {
			return time_t(value * Frequency);
		}

		innerProfile_t currentProfileValue() {
			LARGE_INTEGER value;
			QueryPerformanceCounter(&value);
			return value.QuadPart;
		}

		profileTime_t convertProfile(innerProfile_t value) {
			return profileTime_t(value * ProfileFrequency);
		}

	} // namespace details

	time_t now() {
		const auto elapsed = details::currentValue() - details::StartValue;
		return details::convert(elapsed) + details::computeAdjustment();
	}

	profileTime_t profile() {
		const auto elapsed = details::currentProfileValue()
			- details::StartProfileValue;
		return details::convertProfile(elapsed)
			+ details::computeProfileAdjustment();
	}

	bool adjustTime() {
		return details::adjustTime();
	}

	std::string formattedUnixTime(int64_t unixTime) {
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
} // namespace base::Time