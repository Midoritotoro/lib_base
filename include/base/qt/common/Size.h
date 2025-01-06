#pragma once 

#include <QString>

#include <QFontMetrics>
#include <QFont>

#include <QApplication>
#include <QScreen>

#include <gsl/gsl>
#include "Windows.h"


namespace base::qt::common {
	[[nodiscard]] inline QSize TextSize(
		const QString& text,
		const QFontMetrics& metrics)
	{
		return metrics.size(0, text);
	}

	[[nodiscard]] inline QSize TextSize(
		const QString& text,
		const QFont& font)
	{
		return text.isEmpty() 
			? QSize()
			: TextSize(text, QFontMetrics(font));
	}

	[[nodiscard]] inline QSize GetMinimumSizeWithAspectRatio(
		const QSize& imageSize,
		int targetWidth)
	{
		Expects(imageSize.width() > 0);

		return QSize(targetWidth, targetWidth *
			imageSize.height() / imageSize.width());
	}

	[[nodiscard]] inline bool PartiallyEqual(
		const QSize& first,
		const QSize& second,
		int maxDifference)
	{
		const auto widthDifference = qMax(first.width(), second.width())
			- qMin(first.width(), second.width());

		const auto heightDifference = qMax(first.height(), second.height())
			- qMin(first.height(), second.height());

		return (widthDifference + heightDifference) <= maxDifference;
	}

	[[nodiscard]] inline int FontHeight(const QFont& font) {
		return QFontMetrics(font).height();
	}

	[[nodiscard]] inline QSize ScreenResolution() {
#ifdef _WIN32
		return QSize(
			GetSystemMetrics(SM_CXSCREEN),
			GetSystemMetrics(SM_CYSCREEN)
		);
#endif
		return QSize(0, 0);
	}

	/**
	* ������� ���������� �������� ������
	* \retval -1, ���� �� ������� �������� ������� ����������
	*/
	[[nodiscard]] inline int ScreenRefreshRate() {
#ifdef _WIN32
		auto dm = DEVMODE();

		ZeroMemory(&dm, sizeof(dm));
		dm.dmSize = sizeof(dm);
		dm.dmDriverExtra = 0;

		if (EnumDisplaySettingsEx(nullptr, ENUM_CURRENT_SETTINGS, &dm, 0))
			return dm.dmDisplayFrequency;
		return -1;
#endif
		return -1;
	}
} // namespace core::utility