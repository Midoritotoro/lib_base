#pragma once 

#include <base/Platform.h>

#include <base/images/ImagesRgb.h>
#include <base/images/ImagesGeometry.h>

#include <base/Types.h>

#include <base/OverflowCheck.h>
#include <base/Assert.h>

#define STB_IMAGE_IMPLEMENTATION
#include <base/images/StbImage.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <base/images/StbImageWrite.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <base/images/StbImageResize.h>


#ifdef LIB_BASE_ENABLE_QT
#include <QImage>
#include <QPixmap>

#include <private/qimage_p.h>
#include <private/qpixmap_raster_p.h>
#endif

namespace base::images {
	class GLImage;
	class Image;
} // namespace base::images