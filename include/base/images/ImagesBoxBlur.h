#pragma once


namespace base::images {
	Image boxBlurImage(
		const QImage& _image,
		const QRect& _rect,
		int _radius);

	Image boxBlurImage(
		const QImage& _image,
		int _radius);
} // namespace base::images