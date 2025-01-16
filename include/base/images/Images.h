#pragma once 

#include <base/Platform.h>
#include <base/images/StbImage.h>

#ifdef LIB_BASE_ENABLE_QT
#include <QImage>
#endif

namespace base::gl {
	class Image;
}

namespace base::images {
#ifdef LIB_BASE_ENABLE_QT
	using Image = QImage;
#else
	using Image = gl::Image;
#endif
}