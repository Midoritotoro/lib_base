#pragma once

#include <base/qt/ui/AbstractFlatButton.h>


namespace base::qt::ui {
	class FlatButton final : public AbstractFlatButton {
		Q_OBJECT
	public:
		FlatButton(QWidget* parent = nullptr);
	Q_SIGNALS:
		void menuRequested();
	protected:
		bool event(QEvent* _event) override;
		void mousePressEvent(QMouseEvent* _event) override;
		void paintEvent(QPaintEvent*) override;

		virtual void showMenu(not_null<QMenu*> _menu);
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
} // namespace base::qt::ui