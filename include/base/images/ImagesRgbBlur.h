#pragma once 

#include <base/images/Images.h>


namespace base::images {
	[[nodiscard]] Image RgbBlur(
		QImage&& image,
		bool ignoreAlpha = true);
} // namespace base::images