#pragma once 

#include <base/Platform.h>

//#define STB_IMAGE_IMPLEMENTATION
#include <base/images/StbImage.h>

//#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <base/images/StbImageWrite.h>

//#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <base/images/StbImageResize.h>

#include <base/images/ImagesRgb.h>
#include <base/images/ImagesGeometry.h>

#include <base/Types.h>

#ifdef LIB_BASE_ENABLE_QT
#include <QImage>
#include <QPixmap>
#endif

namespace base::images {
	class GLImage;
	using Image = GLImage;
} // namespace base::images