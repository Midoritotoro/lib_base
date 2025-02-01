#pragma once

#include <base/images/Images.h>

namespace base::images {
	class ImageFilter {
	public:
		enum class Filter: uchar {

		};

		ImageFilter();
		~ImageFilter();

		void filter(
			Image* image,
			Filter filter);
	};
} // namespace base::images