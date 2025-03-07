#include <src/qt/text/TextDrawUtility.h>

#include <base/qt/style/StyleCore.h>
#include <base/qt/style/StyleFont.h>

#include <QPainterPath>


__BASE_QT_TEXT_NAMESPACE_BEGIN

GeometryDescriptor SimpleGeometry(
	int availableWidth,
	int elisionLines,
	int elisionRemoveFromEnd,
	bool elisionBreakEverywhere)
{
	constexpr auto wrap = [](
		Fn<LineGeometry(int line)> layout,
		bool breakEverywhere = false)  
		{
			return GeometryDescriptor { 
				std::move(layout),
				breakEverywhere 
			};
		};

	if (!elisionLines)
		return wrap([=](int line) {
			return LineGeometry { 
				.width = availableWidth
			};
		});
	else if (!elisionRemoveFromEnd)
		return wrap([=](int line) {
			return LineGeometry {
				.width = availableWidth,
				.elided = (line + 1 >= elisionLines),
			};
		}, elisionBreakEverywhere);
	else
		return wrap([=](int line) {
			const auto elided = (line + 1 >= elisionLines);
			const auto removeFromEnd = (elided ? elisionRemoveFromEnd : 0);

			return LineGeometry{
				.width = availableWidth - removeFromEnd,
				.elided = elided,
			};
		}, elisionBreakEverywhere);
};

void ValidateQuotePaintCache(
	QuotePaintCache& cache,
	const style::QuoteStyle& st)
{
	const auto expand = st.expand.isNull() ? nullptr : &st.expand;
	const auto collapse = st.collapse.isNull() ? nullptr : &st.collapse;

	if (!cache.corners.isNull()
		&& cache.bgCached == cache.bg
		&& cache.outlines == cache.outlines
		&& (!st.header || cache.headerCached == cache.header)
		&& ((!expand && !collapse)
		|| cache.iconCached == cache.icon))
		return;

	cache.bgCached = cache.bg;
	cache.outlinesCached = cache.outlines;

	if (st.header)
		cache.headerCached = cache.header;

	const auto radius = st.radius;
	const auto header = st.header;

	const auto outline = st.outline;

	const auto wcorner = std::max(radius, outline);
	const auto hcorner = std::max(header, radius);

	const auto middle = style::lineWidth;

	const auto wside = 2 * wcorner + middle;
	const auto hside = 2 * hcorner + middle;

	const auto full = QSize(wside, hside);
	const auto ratio = style::DevicePixelRatio();

	if (!cache.outlines[1].alpha())
		cache.outline = QImage();
	else if (const auto outline = st.outline) {
		const auto third = (cache.outlines[2].alpha() != 0);
		const auto size = QSize(outline, outline * (third ? 6 : 4));

		cache.outline = QImage(
			size * ratio,
			QImage::Format_ARGB32_Premultiplied);

		cache.outline.fill(cache.outlines[0]);
		cache.outline.setDevicePixelRatio(ratio);

		auto painter = QPainter(&cache.outline);

		painter.setCompositionMode(QPainter::CompositionMode_Source);
		painter.setRenderHints(QPainter::Antialiasing |
			QPainter::SmoothPixmapTransform |
			QPainter::TextAntialiasing);

		auto path = QPainterPath();

		path.moveTo(outline, outline);

		path.lineTo(outline, outline * (third ? 4 : 3));
		path.lineTo(0, outline * (third ? 5 : 4));

		path.lineTo(0, outline * 2);
		path.lineTo(outline, outline);

		painter.fillPath(path, cache.outlines[third ? 2 : 1]);

		if (third) {
			auto path = QPainterPath();
			path.moveTo(outline, outline * 3);

			path.lineTo(outline, outline * 5);
			path.lineTo(0, outline * 6);

			path.lineTo(0, outline * 4);
			path.lineTo(outline, outline * 3);

			painter.fillPath(path, cache.outlines[1]);
		}
	}

	auto image = QImage(full * ratio, QImage::Format_ARGB32_Premultiplied);

	image.fill(Qt::transparent);
	image.setDevicePixelRatio(ratio);

	auto painter = QPainter(&image);

	painter.setRenderHints(QPainter::Antialiasing |
		QPainter::SmoothPixmapTransform |
		QPainter::TextAntialiasing);
	painter.setPen(Qt::NoPen);

	if (header) {
		painter.setBrush(cache.header);
		painter.setClipRect(outline, 0, wside - outline, header);
		painter.drawRoundedRect(0, 0, wside, hcorner + radius, radius, radius);
	}
	if (outline) {
		const auto rect = QRect(0, 0, outline + radius * 2, hside);

		if (!cache.outline.isNull()) {
			const auto shift = QPoint(0, st.outlineShift);

			painter.translate(shift);
			painter.setBrush(cache.outline);

			painter.setClipRect(QRect(-shift, QSize(outline, hside)));
			painter.drawRoundedRect(rect.translated(-shift), radius, radius);

			painter.translate(-shift);
		}
		else {
			painter.setBrush(cache.outlines[0]);
			painter.setClipRect(0, 0, outline, hside);
			painter.drawRoundedRect(rect, radius, radius);
		}
	}

	painter.setBrush(cache.bg);
	painter.setClipRect(outline, header, wside - outline, hside - header);
	painter.drawRoundedRect(0, 0, wside, hside, radius, radius);
	qDebug() << "ValidateQuotePaintCache fill rect: " << QRect(wside, hside, radius, radius);

	painter.end();

	cache.corners = std::move(image);
	cache.expand = QImage();
	cache.collapse = QImage();
}

void FillQuotePaint(
	QPainter& painter,
	QRect rect,
	const QuotePaintCache& cache,
	const style::QuoteStyle& st,
	SkipBlockPaintParts parts)
{
	qDebug() << "FillQuotePaint called";

	const auto& image = cache.corners;
		
	const auto ratio = int(image.devicePixelRatio());

	const auto iwidth = image.width() / ratio;
	const auto iheight = image.height() / ratio;

	const auto imiddle = style::lineWidth;

	const auto whalf = (iwidth - imiddle) / 2;
	const auto hhalf = (iheight - imiddle) / 2;

	const auto x = rect.left();
	auto y = rect.top();

	const auto width = rect.width();
	auto height = rect.height();

	const auto till = y + height;
	qDebug() << "skippedTop: " << parts.skippedTop;
	if (!parts.skippedTop) {
		const auto top = std::min(height, hhalf);

		painter.drawImage(
			QRect(x, y, whalf, top),
			image,
			QRect(0, 0, whalf * ratio, top * ratio));
		painter.drawImage(
			QRect(x + width - whalf, y, whalf, top),
			image,
			QRect((iwidth - whalf) * ratio, 0, whalf * ratio, top * ratio));

		qDebug() << "FillQuotePaint: " << image.size();
		qDebug() << "FillQuotePaint cache.bg: " << cache.bg;

		if (const auto middle = width - 2 * whalf) {
			const auto header = st.header;
			const auto fillHeader = std::min(header, top);

			if (fillHeader)
				painter.fillRect(x + whalf, y, middle, fillHeader, cache.header);
			if (const auto fillBody = top - fillHeader)
				painter.fillRect(
					QRect(x + whalf, y + fillHeader, middle, fillBody),
					cache.bg);
		}
		height -= top;
		if (!height)
			return;

		y += top;
		rect.setTop(y);
	}
	const auto outline = st.outline;

	if (!parts.skipBottom) {
		const auto bottom = std::min(height, hhalf);
		const auto skip = !cache.outline.isNull() ? outline : 0;
		painter.drawImage(
			QRect(x + skip, y + height - bottom, whalf - skip, bottom),
			image,
			QRect(
				skip * ratio,
				(iheight - bottom) * ratio,
				(whalf - skip) * ratio,
				bottom * ratio));
		painter.drawImage(
			QRect(
				x + width - whalf,
				y + height - bottom,
				whalf,
				bottom),
			image,
			QRect(
				(iwidth - whalf) * ratio,
				(iheight - bottom) * ratio,
				whalf * ratio,
				bottom * ratio));
		if (const auto middle = width - 2 * whalf) {
			painter.fillRect(
				QRect(x + whalf, y + height - bottom, middle, bottom),
				cache.bg);
		}
		if (skip) {
			if (cache.bottomCorner.size() != QSize(skip, whalf)) {
				cache.bottomCorner = QImage(
					QSize(skip, hhalf) * ratio,
					QImage::Format_ARGB32_Premultiplied);

				cache.bottomCorner.setDevicePixelRatio(ratio);
				cache.bottomCorner.fill(Qt::transparent);

				cache.bottomRounding = QImage(
					QSize(skip, hhalf) * ratio,
					QImage::Format_ARGB32_Premultiplied);

				cache.bottomRounding.setDevicePixelRatio(ratio);
				cache.bottomRounding.fill(Qt::transparent);

				const auto radius = st.radius;
				auto q = QPainter(&cache.bottomRounding);

				q.setRenderHints(QPainter::Antialiasing |
					QPainter::SmoothPixmapTransform |
					QPainter::TextAntialiasing);

				q.setPen(Qt::NoPen);
				q.setBrush(Qt::white);

				q.drawRoundedRect(
					0,
					-2 * radius,
					skip + 2 * radius,
					hhalf + 2 * radius,
					radius,
					radius);
			}
			auto q = QPainter(&cache.bottomCorner);
			const auto skipped = (height - bottom)
				+ (parts.skippedTop ? int(parts.skippedTop) : hhalf)
				- st.outlineShift;

			q.translate(0, -skipped);
			q.fillRect(0, skipped, skip, bottom, cache.outline);

			q.setCompositionMode(QPainter::CompositionMode_DestinationIn);
			q.drawImage(0, skipped + bottom - hhalf, cache.bottomRounding);
			q.end();

			painter.drawImage(
				QRect(x, y + height - bottom, skip, bottom),
				cache.bottomCorner,
				QRect(0, 0, skip * ratio, bottom * ratio));
		}
		height -= bottom;
		rect.setHeight(height);
	}
	if (outline && height > 0) {
		if (!cache.outline.isNull()) {
			const auto skipped = st.outlineShift
				- (parts.skippedTop ? int(parts.skippedTop) : hhalf);
			const auto top = y + skipped;

			painter.translate(x, top);
			painter.fillRect(0, -skipped, outline, height, cache.outline);
			painter.translate(-x, -top);
		}
		else
			painter.fillRect(x, y, outline, height, cache.outlines[0]);
	}
	painter.fillRect(x + outline, y, width - outline, height, cache.bg);
}

__BASE_QT_TEXT_NAMESPACE_END