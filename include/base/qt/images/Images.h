#pragma once 

#include <base/Platform.h>

#ifdef LIB_BASE_ENABLE_QT
#include <QImage>
#endif

namespace base::gl {
	class Image;
}

namespace base::images {
#ifdef LIB_BASE_ENABLE_QT
	using Image = gl::Image;
#else
	using Image = QImage;
#endif
}