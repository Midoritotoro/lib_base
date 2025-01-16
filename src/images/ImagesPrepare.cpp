#include <base/qt/images/ImagesPrepare.h>

#include <base/qt/common/Types.h>
#include <base/qt/style/StyleScale.h>

#include <base/Utility.h>
#include <base/qt/common/Size.h>

#include <QPainter>
#include <gsl/gsl>


namespace base::images {
	namespace {
		struct Shifted {
			Shifted() = default;
			Shifted(uint32 value) : value(value) 
			{}
			Shifted(uint64 value) : value(value)
			{}

			uint64 value = 0;
		};

		inline Shifted operator+(Shifted a, Shifted b) {
			return Shifted(a.value + b.value);
		}

		inline Shifted operator*(Shifted shifted, uint64 multiplier) {
			return Shifted(shifted.value * multiplier);
		}

		inline Shifted operator*(uint64 multiplier, Shifted shifted) {
			return Shifted(shifted.value * multiplier);
		}

		Shifted reshifted(Shifted components) {
			return (components.value >> 8) & 0x00FF00FF00FF00FFULL;
		}

		inline uint32 unshifted(Shifted components) {
			return static_cast<uint32>((components.value & 0x000000000000FF00ULL) >> 8)
				| static_cast<uint32>((components.value & 0x00000000FF000000ULL) >> 16)
				| static_cast<uint32>((components.value & 0x0000FF0000000000ULL) >> 24)
				| static_cast<uint32>((components.value & 0xFF00000000000000ULL) >> 32);
		}

		inline uint32 getAlpha(Shifted components) {
			return (components.value & 0x00FF000000000000ULL) >> 48;
		}

		Shifted shifted(QColor color) {
			const auto alpha = static_cast<uint64>((color.alpha() & 0xFF) + 1);
			const auto components = static_cast<uint64>(color.blue() & 0xFF)
				| (static_cast<uint64>(color.green() & 0xFF) << 16)
				| (static_cast<uint64>(color.red() & 0xFF) << 32)
				| (static_cast<uint64>(255) << 48);
			return reshifted(components * alpha);
		}
	}

	bool IsRgbNull(QRgb rgb) {
		return qRed(rgb) == 0 
			&& qGreen(rgb) == 0 
			&& qBlue(rgb) == 0;
	}

	QPixmap PixmapFast(QImage&& image) {
		Expects(image.format() == QImage::Format_ARGB32_Premultiplied
			|| image.format() == QImage::Format_RGB32);

		return QPixmap::fromImage(std::move(image), Qt::NoFormatConversion);
	}

	QImage Opaque(QImage&& image) {
		if (image.hasAlphaChannel()) {
			if (image.format() != QImage::Format_ARGB32_Premultiplied)
				image = std::move(image).convertToFormat(
					QImage::Format_ARGB32_Premultiplied);

			auto ints = reinterpret_cast<uint32*>(image.bits());
			const auto bg = shifted(QColor(Qt::white));

			const auto width = image.width();
			const auto height = image.height();

			const auto addPerLine = (image.bytesPerLine() / sizeof(uint32)) - width;

			for (auto y = 0; y != height; ++y) {
				for (auto x = 0; x != width; ++x) {
					const auto components = shifted(*ints);
					*ints++ = unshifted(components * 256
						+ bg * (256 - getAlpha(components)));
				}
				ints += addPerLine;
			}
		}
		return std::move(image);
	}

	QImage Prepare(
		QImage image,
		const QSize& _outer)
	{
		const auto imageWidth = image.width();
		const auto imageHeight = image.height();

		if (_outer.width() <= 0 || (_outer.width() == imageWidth
			&& (_outer.height() <= 0 || _outer.height() == imageHeight)))
			;
		else if (imageHeight <= 0)
			image = image.scaledToWidth(
				_outer.width(),
				Qt::SmoothTransformation);
		else
			image = image.scaled(
				_outer.width(),
				_outer.height(),
				Qt::IgnoreAspectRatio,
				Qt::SmoothTransformation);

		auto outer = _outer;

		if (!outer.isEmpty()) {
			const auto ratio = style::DevicePixelRatio();
			outer *= ratio;

			if (outer != QSize(_outer.width(), _outer.height())) {
				image.setDevicePixelRatio(ratio);

				auto result = QImage(outer, QImage::Format_ARGB32_Premultiplied);
				result.setDevicePixelRatio(ratio);

				auto painter = QPainter(&result);

				if (_outer.width() < outer.width() || _outer.height() < outer.height())
					painter.fillRect(
						QRect(QPoint(), result.size() / ratio),
						Qt::black);
				painter.drawImage(
					(result.width() - imageWidth) / (2 * ratio),
					(result.height() - imageWidth) / (2 * ratio),
					image);

				image = std::move(result);
			}
		}

		image.setDevicePixelRatio(style::DevicePixelRatio());
		return image;
	}

	QImage Prepare(
		const QImage& sourceImage,
		double _scale)
	 {
		const auto resolveSize = [=](const QSize& size) -> QSize {
			const auto screenSize = base::qt::common::ScreenResolution();
			double scale = qMin(static_cast<double>(screenSize.width()) / size.width(),
				static_cast<double>(screenSize.height()) / size.height());

			if (size.width() * scale <= (screenSize.width() * _scale))
				return size;

			scale = qMin(scale, (screenSize.width() * _scale) / size.width());
			return QSize(size.width() * scale, size.height() * scale);
		};

		return Prepare(sourceImage, resolveSize(sourceImage.size()));
	}
} // namespace base::qt::images