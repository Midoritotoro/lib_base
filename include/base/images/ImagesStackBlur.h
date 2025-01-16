#pragma once 

#include <base/images/Images.h>

namespace base::images {
	Image stackBlurImage(
		const Image& _image,
		int _radius,
		int _threadCount = 1);
} // namespace base::images