#pragma once 


#include <base/images/Images.h>


__BASE_IMAGES_NAMESPACE_BEGIN

  bool IsRgbNull(Rgb rgb);

#ifdef LIB_BASE_ENABLE_QT

  QPixmap PixmapFast(QImage&& image);
  QImage Opaque(QImage&& image);

  QImage Prepare(
	const QImage& image,
	double _scale);

  QImage Prepare(
	QImage image,
	const QSize& _outer);

#endif

__BASE_IMAGES_NAMESPACE_END