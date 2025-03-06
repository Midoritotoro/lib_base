#pragma once 


#include <base/images/Images.h>


__BASE_IMAGES_NAMESPACE_BEGIN

NODISCARD bool IsRgbNull(Rgb rgb);

#ifdef LIB_BASE_ENABLE_QT

NODISCARD QPixmap PixmapFast(QImage&& image);
NODISCARD QImage Opaque(QImage&& image);

NODISCARD QImage Prepare(
	const QImage& image,
	double _scale);

NODISCARD QImage Prepare(
	QImage image,
	const QSize& _outer);

#endif

__BASE_IMAGES_NAMESPACE_END