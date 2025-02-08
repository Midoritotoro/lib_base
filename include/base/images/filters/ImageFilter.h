#pragma once

#include <base/images/Images.h>

namespace base::images {
	class ImageFilter {
	public:
		ImageFilter(not_null<Image*> image);
		
#if defined(LIB_BASE_ENABLE_QT)
		ImageFilter(not_null<QImage*> image);
#endif

		void filter(Filter filter);
	private:
		void BradleyThreshold();

		QImage* _image = nullptr;
	};
} // namespace base::images