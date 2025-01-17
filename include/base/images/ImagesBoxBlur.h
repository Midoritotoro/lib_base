#pragma once

#include <base/images/Images.h>

namespace base::images {
	Image boxBlurImage(
		const Image& image,
		const Rect& rect,
		int radius);

	Image boxBlurImage(
		const Image& image,
		int radius);
} // namespace base::images