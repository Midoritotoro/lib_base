#pragma once 

#include <base/core/arch/Platform.h>

#include <base/images/ImagesRgb.h>
#include <base/images/ImagesGeometry.h>

#include <base/core/Types.h>

#include <base/core/utility/OverflowCheck.h>
#include <base/core/utility/Assert.h>

// Отключает все внутренние исключения
// #define LIB_BASE_IMAGES_NO_FAILURE

#ifdef LIB_BASE_ENABLE_QT
	#include <QImage>
	#include <QPixmap>

	#include <private/qimage_p.h>
	#include <private/qpixmap_raster_p.h>
#endif


__BASE_IMAGES_NAMESPACE_BEGIN

class GLImage;
class Image;
class AbstractFormatHandler;

enum class Filter : uchar {
	None = 0x00,
	BradleyThreshold = 0x01
};

enum class ColorSpace : uchar {
	Invalid = 0x00,
	BGR = 0x01,
	Mono = 0x02,
	RGB = 0x04,
	RGBA = 0x08,
	/*YCbCr = 0x08,
	YUV = 0x10,
	CMY = 0x20,
	CMYK = 0x40,
	HSL = 0x80,
	HSV = 0xFF*/
};

struct ImageData {
	int32 width = 0;
	int32 height = 0;

	uchar* data = nullptr;
	ushort channels = 0;

	int32 depth;
	int32 bytesPerLine = 0;

	int32 sizeInBytes = 0;
	sizetype dataLength = 0;

	int32 bitsPerChannel = 0;
	ColorSpace colorSpace;

	std::optional<const char*> path;
	std::optional<ushort> jpegQuality;

	AbstractFormatHandler* handler = nullptr;
	Filter filter = Filter::None;
};

__BASE_IMAGES_NAMESPACE_END