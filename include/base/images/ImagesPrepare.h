#pragma once 


#include <base/images/Images.h>


namespace base::images {
	[[nodiscard]] bool IsRgbNull(Rgb rgb);

#ifdef LIB_BASE_ENABLE_QT
	[[nodiscard]] QPixmap PixmapFast(Image&& image);
#endif
	[[nodiscard]] Image Opaque(Image&& image);

	[[nodiscard]] Image Prepare(
		const Image& image,
		double _scale);

	[[nodiscard]] Image Prepare(
		Image image,
		const Size& _outer);
} // namespace base::qt::images