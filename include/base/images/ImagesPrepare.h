#pragma once 


#include <base/images/Images.h>


__BASE_IMAGES_NAMESPACE_BEGIN

base_nodiscard bool IsRgbNull(Rgb rgb);

#ifdef LIB_BASE_ENABLE_QT

base_nodiscard QPixmap PixmapFast(QImage&& image);
base_nodiscard QImage Opaque(QImage&& image);

base_nodiscard QImage Prepare(
	const QImage& image,
	double _scale);

base_nodiscard QImage Prepare(
	QImage image,
	const QSize& _outer);

#endif

__BASE_IMAGES_NAMESPACE_END