#pragma once 

#include <QImage>
#include <QPixmap>

#include <QPixmapCache>

#include <QApplication>
#include <QScreen>


namespace base::qt::images {
	[[nodiscard]] bool IsRgbNull(QRgb rgb);

	[[nodiscard]] QPixmap PixmapFast(QImage&& image);
	[[nodiscard]] QImage Opaque(QImage&& image);

	[[nodiscard]] QImage Prepare(
		const QImage& image,
		double _scale);

	[[nodiscard]] QImage Prepare(
		QImage image,
		const QSize& _outer);
} // namespace base::qt::images