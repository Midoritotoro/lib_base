#include <base/qt/style/StyleCore.h>

#include <QPainterPath>
#include <QPainter>

#include <base/qt/style/StyleWidgets.h>


namespace base::qt::style {
	namespace {
		bool rightToLeft = false;
	} // namespace

	bool RightToLeft() {
		return rightToLeft;
	}

	void SetRightToLeft(bool _rightToLeft) {
		rightToLeft = rightToLeft;
	}

	Qt::LayoutDirection LayoutDirection() {
		return RightToLeft() ? Qt::RightToLeft : Qt::LeftToRight;
	}

	void RoundCorners(
		QPainter& painter,
		const QSize& widgetSize,
		int borderRadius,
		CornersRoundMode roundType) 
	{
		auto path = QPainterPath();
		path.setFillRule(Qt::WindingFill);

		switch (roundType) {
			case CornersRoundMode::Top:
				path.moveTo(borderRadius, 0);

				path.lineTo(widgetSize.width() - borderRadius, 0);
				path.quadTo(widgetSize.width(), 0, widgetSize.width(), borderRadius);

				path.lineTo(widgetSize.width(), widgetSize.height());
				path.lineTo(0, widgetSize.height());

				path.lineTo(0, borderRadius);
				path.quadTo(0, 0, borderRadius, 0);

				break;

			case CornersRoundMode::Bottom:
				path.moveTo(0, 0);

				path.lineTo(widgetSize.width(), 0);

				path.lineTo(widgetSize.width(), widgetSize.height() - borderRadius);
				path.quadTo(widgetSize.width(), widgetSize.height(),
					widgetSize.width() - borderRadius, widgetSize.height());

				path.lineTo(borderRadius, widgetSize.height());
				path.quadTo(0, widgetSize.height(), 0,
					widgetSize.height() - borderRadius);

				path.lineTo(0, 0);

				break;

			case CornersRoundMode::All:
				path.moveTo(borderRadius, 0);

				path.lineTo(widgetSize.width() - borderRadius, 0);
				path.quadTo(widgetSize.width(), 0, widgetSize.width(), borderRadius);

				path.lineTo(widgetSize.width(), widgetSize.height() - borderRadius);
				path.quadTo(widgetSize.width(), widgetSize.height(),
					widgetSize.width() - borderRadius, widgetSize.height());

				path.lineTo(borderRadius, widgetSize.height());
				path.quadTo(0, widgetSize.height(), 0,
					widgetSize.height() - borderRadius);

				path.lineTo(0, borderRadius);
				path.quadTo(0, 0, borderRadius, 0);

				break;
		}

		painter.setClipPath(path);
	}

	void RoundCorners(
		QPainter& painter,
		const QSize& widgetSize,
		int borderRadius)
	{
		return RoundCorners(painter, widgetSize, borderRadius, CornersRoundMode::All);
	}

	void RoundTopCorners(
		QPainter& painter,
		const QSize& widgetSize,
		int borderRadius) 
	{
		return RoundCorners(painter, widgetSize, borderRadius, CornersRoundMode::Top);
	}

	void RoundBottomCorners(
		QPainter& painter,
		const QSize& widgetSize,
		int borderRadius) 
	{
		return RoundCorners(painter, widgetSize, borderRadius, CornersRoundMode::Bottom);
	}

	void Start() {
		internal::RegisterFontFamily("Open Sans");
	}

	void Stop() {
		internal::DestroyFonts();
	}

	void RegisterStyles() {
		initStyles();
	}
} // namespace base::qt::style