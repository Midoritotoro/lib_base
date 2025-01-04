#include "Separator.h"

#include <QPainter>


Separator::Separator(
	QWidget* parent,
	const style::Separator* style,
	const style::PopupMenu* menuStyle
):
	QWidget(parent)
	, _lineWidth(style->width)
	, _padding(style->padding)
	, _fg(style->colorFg)
	, _bg(menuStyle->colorBg)
	, _height(_padding.top() + _lineWidth + _padding.bottom())
{
	auto painter = QPainter(this);

	painter.fillRect(0, 0, width(), _height, _bg);
	painter.fillRect(
		_padding.left(),
		_padding.top(),
		width() - _padding.left() - _padding.right(),
		_lineWidth,
		_fg);
}