#pragma once 

#include <base/Platform.h>

#include <base/images/StbImage.h>
#include <base/images/StbImageWrite.h>

#include <base/images/ImagesRgb.h>
#include <base/images/ImagesGeometry.h>

#include <base/Types.h>

#ifdef LIB_BASE_ENABLE_QT
#include <QImage>
#include <QPixmap>
#endif

namespace base::gl {
	class Image;
}

namespace base::images {
	using Image = gl::Image;
} // namespace base::images