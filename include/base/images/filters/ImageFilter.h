#pragma once

#include <base/images/Images.h>

namespace base::images {
	class ImageFilter {
	public:
		ImageFilter(not_null<Image*> image);

		void filter(Filter filter);
	private:
		void BradleyThreshold();

		Image* _image = nullptr;
	};
} // namespace base::images