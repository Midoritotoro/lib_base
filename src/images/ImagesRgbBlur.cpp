#include <base/images/ImagesRgbBlur.h>

#include <base/images/Images.h>

namespace base::images {
	namespace {
		uint64 BlurGetColors(const uchar* p) {
			return (uint64)p[0]
				+ ((uint64)p[1] << 16)
				+ ((uint64)p[2] << 32)
				+ ((uint64)p[3] << 48);
		}
	} // namespace 

	QImage RgbBlur(
		QImage&& image,
		bool ignoreAlpha)
	{
		if (image.isNull())
			return std::move(image);

		const auto ratio = image.devicePixelRatio();
		const auto format = image.format();

		if (format != QImage::Format_RGB32
			&& format != QImage::Format_ARGB32_Premultiplied) {
			image = std::move(image).convertToFormat(
				QImage::Format_ARGB32_Premultiplied);
			image.setDevicePixelRatio(ratio);
		}

		auto pix = image.bits();
		if (!pix)
			return std::move(image);

		const auto w = image.width();
		const auto h = image.height();

		const auto radius = 3;

		const auto r1 = radius + 1;
		const auto div = radius * 2 + 1;

		const auto stride = w * 4;

		if (radius >= 16 || div >= w || div >= h || stride > w * 4)
			return std::move(image);

		const auto withalpha = !ignoreAlpha && image.hasAlphaChannel();

		if (withalpha) {
			auto smaller = QImage(image.size(), image.format());
			{
				auto painter = QPainter(&smaller);

				painter.setCompositionMode(QPainter::CompositionMode_Source);
				painter.fillRect(0, 0, w, h, Qt::transparent);
				painter.drawImage(
					QRect(radius, radius, w - 2 * radius, h - 2 * radius),
					image,
					QRect(0, 0, w, h));
			}

			smaller.setDevicePixelRatio(ratio);
			auto was = std::exchange(image, std::exchange(smaller, {}));

			assert(!image.isNull());

			pix = image.bits();
			if (!pix)
				return was;
		}
		const auto buffer = std::make_unique<uint64[]>(w * h);
		const auto rgb = buffer.get();

		int x, y, i;

		int yw = 0;

		const int we = w - r1;

		for (y = 0; y < h; y++) {
			uint64 cur = BlurGetColors(&pix[yw]);
			uint64 rgballsum = -radius * cur;

			uint64 rgbsum = cur * ((r1 * (r1 + 1)) >> 1);

			for (i = 1; i <= radius; i++) {
				uint64 cur = BlurGetColors(&pix[yw + i * 4]);
				rgbsum += cur * (r1 - i);
				rgballsum += cur;
			}

			x = 0;

#define update(start, middle, end) \
		rgb[y * w + x] = (rgbsum >> 4) & 0x00FF00FF00FF00FFLL; \
		rgballsum += BlurGetColors(&pix[yw + (start) * 4]) - 2 * BlurGetColors(&pix[yw + (middle) * 4]) + BlurGetColors(&pix[yw + (end) * 4]); \
		rgbsum += rgballsum; \
		x++;

			while (x < r1) {
				update(0, x, x + r1);
			}
			while (x < we) {
				update(x - r1, x, x + r1);
			}
			while (x < w) {
				update(x - r1, x, w - 1);
			}

#undef update

			yw += stride;
		}

		const int he = h - r1;
		for (x = 0; x < w; x++) {
			uint64 rgballsum = -radius * rgb[x];
			uint64 rgbsum = rgb[x] * ((r1 * (r1 + 1)) >> 1);
			for (i = 1; i <= radius; i++) {
				rgbsum += rgb[i * w + x] * (r1 - i);
				rgballsum += rgb[i * w + x];
			}

			y = 0;
			int yi = x * 4;

#define update(start, middle, end) \
		uint64 res = rgbsum >> 4; \
		pix[yi] = res & 0xFF; \
		pix[yi + 1] = (res >> 16) & 0xFF; \
		pix[yi + 2] = (res >> 32) & 0xFF; \
		pix[yi + 3] = (res >> 48) & 0xFF; \
		rgballsum += rgb[x + (start) * w] - 2 * rgb[x + (middle) * w] + rgb[x + (end) * w]; \
		rgbsum += rgballsum; \
		y++; \
		yi += stride;

			while (y < r1) {
				update(0, y, y + r1);
			}
			while (y < he) {
				update(y - r1, y, y + r1);
			}
			while (y < h) {
				update(y - r1, y, h - 1);
			}

#undef update
		}

		return std::move(image);
	}
} // namespace base::images