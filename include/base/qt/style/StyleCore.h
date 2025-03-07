#pragma once

#include <QImage>
#include <QSize>

#include <algorithm>
#include <cmath>

#include <optional>

#include <QFontMetrics>
#include <QPixmapCache>

#include <base/qt/style/StyleScale.h>
#include <base/qt/style/StyleWidgets.h>

#include <base/qt/style/StyleFont.h>


__BASE_QT_STYLE_NAMESPACE_BEGIN

enum class CornersRoundMode {
	All,
	Top,
	Bottom
};

NODISCARD bool RightToLeft();
void SetRightToLeft(bool _rightToLeft);

NODISCARD Qt::LayoutDirection LayoutDirection();

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

__BASE_QT_STYLE_NAMESPACE_END
