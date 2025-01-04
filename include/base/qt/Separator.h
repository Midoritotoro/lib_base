#pragma once

#include <base/qt/StyleWidgets.h>
#include <QWidget>


class Separator : public QWidget {
	Q_OBJECT
public:
	Separator(
		QWidget* parent,
		const style::Separator* style,
		const style::PopupMenu* menuStyle);
private:
	const int _lineWidth;

	const style::margins& _padding;

	const style::color& _fg;
	const style::color& _bg;

	const int _height;
};