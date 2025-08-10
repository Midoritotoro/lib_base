#pragma once

#include <QWidget>
#include <QPushButton>

#include <functional>
#include <base/qt/ui/EnhancedSlider.h>


__BASE_QT_UI_NAMESPACE_BEGIN

class SpeedController;
class SpeedButtonOverlay : public QWidget {
	Q_OBJECT
private:
	EnhancedSlider* _speedSlider = nullptr;
	QRect _textRect;

	float _speed = 1.0f;
	friend class SpeedController;
public:
	SpeedButtonOverlay(QWidget* parent = nullptr);

	  float speed() const noexcept;
protected:
	void paintEvent(QPaintEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;
};


class SpeedController : public QPushButton {
	Q_OBJECT
private:
	SpeedButtonOverlay* _overlay = nullptr;
	QImage _speedButtonImage;
public:
	SpeedController(QWidget* parent = nullptr);
Q_SIGNALS:
	void speedChanged(float speed);
protected:
	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;

	bool eventFilter(QObject* sender, QEvent* event) override;
};

__BASE_QT_UI_NAMESPACE_END