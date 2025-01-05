#pragma once 

#include <QImage>


namespace base::qt::images {
	[[nodiscard]] QImage Blur(
		QImage&& image,
		bool ignoreAlpha = false);
} // namespace base::qt::images