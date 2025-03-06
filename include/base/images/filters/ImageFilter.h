#pragma once

#include <base/images/Images.h>

__BASE_IMAGES_NAMESPACE_BEGIN

class ImageFilter {
public:
	ImageFilter(not_null<Image*> image);
		
#if defined(LIB_BASE_ENABLE_QT)
	ImageFilter(not_null<QImage*> image);
#endif

	void filter(Filter filter);
private:
	void BradleyThreshold();

	Image* _image = nullptr;
};

__BASE_IMAGES_NAMESPACE_END
