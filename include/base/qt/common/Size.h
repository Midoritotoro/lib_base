#pragma once 

#include <QString>

#include <QFontMetrics>
#include <QFont>

#include <QApplication>
#include <QScreen>

#include <base/core/Types.h>


__BASE_QT_COMMON_NAMESPACE_BEGIN

  inline QSize TextSize(
	const QString& text,
	const QFontMetrics& metrics)
{
	return metrics.size(0, text);
}

  inline QSize TextSize(
	const QString& text,
	const QFont& font)
{
	return text.isEmpty() 
		? QSize()
		: TextSize(text, QFontMetrics(font));
}

  inline QSize GetMinimumSizeWithAspectRatio(
	const QSize& imageSize,
	int targetWidth)
{
	Expects(imageSize.width() > 0);

	return QSize(targetWidth, targetWidth *
		imageSize.height() / imageSize.width());
}

  inline bool PartiallyEqual(
	const QSize& first,
	const QSize& second,
	int maxDifference)
{
	const auto widthDifference = qMax(
		first.width(), second.width())
		- qMin(first.width(), second.width());

	const auto heightDifference = qMax(
		first.height(), second.height())
		- qMin(first.height(), second.height());

	return (widthDifference + heightDifference) <= maxDifference;
}

  inline int FontHeight(const QFont& font) {
	return QFontMetrics(font).height();
}

  inline QSize ScreenResolution() {
	return QApplication::primaryScreen()->availableGeometry().size();
}

  inline int ScreenRefreshRate() {
	return QApplication::primaryScreen()->refreshRate();
}

__BASE_QT_COMMON_NAMESPACE_END
