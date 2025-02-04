#pragma once

#include <base/images/Images.h>

namespace base::images {
	class ImageFilter {
	public:
		enum class Filter : uchar {
			BradleyThreshold = 0x00
		};

		ImageFilter();
		~ImageFilter();

		void filter(
			Image* image,
			Filter filter);
	private:
		void BradleyThreshold(Image* image);
	};
} // namespace base::images