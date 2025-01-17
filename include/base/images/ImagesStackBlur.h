#pragma once 

#include <base/images/Images.h>

namespace base::images {
	[[nodiscard]] Image stackBlurImage(
		const Image& image,
		int radius,
		int threadCount = 1);
} // namespace base::images