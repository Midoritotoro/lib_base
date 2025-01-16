#pragma once 

#include <base/qt/images/ImageGaussianBlur.h>
#include <base/qt/images/ImageBoxBlur.h>

#include <base/qt/images/ImageStackBlur.h>


namespace base::qt::images {
	[[nodiscard]] QImage DefaultBlur(
		QImage&& image,
		bool ignoreAlpha = false);

	QImage boxBlurImage(
		const QImage& _image,
		const QRect& _rect,
		int _radius);

	QImage boxBlurImage(
		const QImage& _image,
		int _radius);

	QImage stackBlurImage(
		const QImage& _image,
		int _radius,
		int _threadCount = 1);

} // namespace base::qt::images