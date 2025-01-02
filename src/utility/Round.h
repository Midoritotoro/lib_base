#pragma once 


namespace core::utility {
	[[nodiscard]] inline double SafeRound(double value) {
		Expects(!std::isnan(value));

		if (const auto result = std::round(value); !std::isnan(result)) {
			return result;
		}
		const auto errors = std::fetestexcept(FE_ALL_EXCEPT);
		if (const auto result = std::round(value); !std::isnan(result)) {
			return result;
		}

		std::feclearexcept(FE_ALL_EXCEPT);
		if (const auto result = std::round(value); !std::isnan(result)) {
			return result;
		}

		return value;
	}
} // core::utility
