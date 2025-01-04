#pragma once

#include <QImage>
#include <QSize>

#include <algorithm>
#include <cmath>

#include <QFontMetrics>
#include <QPixmapCache>

#include <base/qt/style/StyleScale.h>
#include <base/qt/style/StyleWidgets.h>


namespace base::qt::style {
	enum class CornersRoundMode {
		All,
		Top,
		Bottom
	};

	[[nodiscard]] bool RightToLeft();
	void SetRightToLeft(bool _rightToLeft);

	[[nodiscard]] Qt::LayoutDirection LayoutDirection();

	void RoundCorners(
		QPainter& painter,
		const QSize& widgetSize,
		int borderRadius,
		CornersRoundMode roundType);
	void RoundCorners(
		QPainter& painter,
		const QSize& widgetSize,
		int borderRadius);

	void RoundTopCorners(
		QPainter& painter,
		const QSize& widgetSize,
		int borderRadius);
	void RoundBottomCorners(
		QPainter& painter,
		const QSize& widgetSize,
		int borderRadius);

	void Start();
	void Stop();

	void RegisterStyles();
} // namespace base::qt::style