#pragma once

#include <base/qt/ui/AbstractFlatButton.h>


__BASE_QT_UI_NAMESPACE_BEGIN

class FlatButton final : public AbstractFlatButton {
public:
	FlatButton(QWidget* parent = nullptr);
protected:
	bool event(QEvent* _event) override;
	void mousePressEvent(QMouseEvent* _event) override;
	void paintEvent(QPaintEvent*) override;
private:
	void drawBackground(
		QPainter& _painter,
		const QStyleOptionComplex& _option,
		const QRect& _iconRect);
	void drawBadge(
		QPainter& _painter,
		const QStyleOptionComplex& _option);

	void drawIcon(
		QPainter& painter,
		const QIcon& icon,
		const QStyleOption& option);

	void updateHoverControl(const QPoint& _pos);
	void drawText(
		QPainter& painter,
		const QStyleOptionComplex& option,
		const QString& text);
private:
	QStyle::SubControls _hoverControl;
};

__BASE_QT_UI_NAMESPACE_END