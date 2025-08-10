#pragma once

#include <base/core/Types.h>

#include <QScrollArea>
#include <QVBoxLayout>


__BASE_QT_UI_NAMESPACE_BEGIN

class InnerWidget : public QWidget {
	Q_OBJECT
public:
	InnerWidget(QWidget* parent = nullptr);

	void setOpacity(double opacity);
	  double opacity() const noexcept;

	void setBackgroundColor(const QColor& color);
	  QColor backgroundColor() const noexcept;
protected:
	void paintEvent(QPaintEvent* event) override;
private:
	double _opacity = 1.;
	QColor _backgroundColor = Qt::black;
};

class ScrollArea : public QScrollArea {
public:
	ScrollArea(QWidget* parent = nullptr);

	int scrollHeight() const;
	int scrollTopMax() const;
	int scrollTop() const;

	bool focusNextPrevChild(bool next) override;
	void setOpacity(double opacity);

	  int computeScrollTo(int toTop, int toBottom);

	void scrollToY(int toTop, int toBottom = -1);
	void scrolled();

	void scrollToWidget(not_null<QWidget*> widget);

	void disableScroll(bool dis);

	void addItem(QWidget* item, Qt::Alignment align = Qt::AlignLeft | Qt::AlignTop);

	void setWidget(InnerWidget* widget);
	  InnerWidget* widget() const noexcept;
protected:
	bool viewportEvent(QEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;

	void paintEvent(QPaintEvent* event) override;
private:
	  int itemsHeight() const;
	bool _disabled = false;

	int _verticalValue;
	int _itemsHeight = 0;
	double _opacity = 1.;

	QVBoxLayout* _scrollLayout = nullptr;
};

__BASE_QT_UI_NAMESPACE_END