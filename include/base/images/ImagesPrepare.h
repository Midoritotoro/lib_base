#pragma once 


#include <base/images/Images.h>


namespace base::images {
	[[nodiscard]] bool IsRgbNull(Rgb rgb);

#ifdef LIB_BASE_ENABLE_QT
	[[nodiscard]] QPixmap PixmapFast(QImage&& image);
	[[nodiscard]] QImage Opaque(QImage&& image);

	[[nodiscard]] QImage Prepare(
		const QImage& image,
		double _scale);

	[[nodiscard]] QImage Prepare(
		QImage image,
		const QSize& _outer);
#endif
} // namespace base::images