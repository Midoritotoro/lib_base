#pragma once 

#include <base/Platform.h>

#include <base/images/ImagesRgb.h>
#include <base/images/ImagesGeometry.h>

#include <base/Types.h>

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