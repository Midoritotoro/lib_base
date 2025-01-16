#pragma once 

#include <base/Platform.h>

#include <base/images/StbImage.h>
#include <base/images/StbImageWrite.h>


#ifdef LIB_BASE_ENABLE_QT
#include <QImage>
#endif

namespace base::gl {
	class Image;
}

namespace base::images {
#ifdef LIB_BASE_ENABLE_QT
	using Image = QImage;
	using Rect = QRect;
	using Size = QSize;
#else
	struct Size {
		int width = 0;
		int height = 0;
	};

	struct Rect {
		Size size;

		int x = 0;
		int y = 0;
	};

	using Image = gl::Image;
#endif
} // namespace base::images