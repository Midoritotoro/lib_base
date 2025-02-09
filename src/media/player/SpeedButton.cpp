#include <base/media/player/SpeedButton.h>

#include <base/qt/style/StyleScale.h>
#include <base/qt/style/StyleWidgets.h>

#include <base/images/ImagesUtility.h>
#include <base/Utility.h>

#include <base/qt/style/StyleCore.h>

#include <QApplication>
#include <QPainter>
#include <QScreen>

#include <QPainterPath>
#include <QMouseEvent>

namespace base::qt::ui {
	SpeedButtonOverlay::SpeedButtonOverlay(QWidget* parent) :
		QWidget(parent)
	{
		_speedSlider = new EnhancedSlider(this);

		_speedSlider->setMaximum(25);
		_speedSlider->setMinimum(5);

		_speedSlider->setStyleSheet(style::SliderStyle());
		_speedSlider->setFixedHeight(style::sliderHeight);
	}

	float SpeedButtonOverlay::speed() const noexcept {
		return _speed;
	}

	void SpeedButtonOverlay::paintEvent(QPaintEvent* event) {
		QPainter painter(this);
		painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

		painter.setPen(Qt::NoPen);
		painter.setBrush(QColor(41, 45, 51));

		style::RoundCorners(painter, size(), 10);
		painter.drawRect(rect());

		painter.setPen(Qt::white);
		painter.drawText(_textRect, Qt::AlignCenter, QString::number(_speed, 'f', 1) + "x");
	}

	void SpeedButtonOverlay::resizeEvent(QResizeEvent* event) {
		_textRect = QRect(QPoint(), core::utility::TextSize(QString::number(_speed, 'f', 1) + "x", font()));
		_textRect.moveTo(QPoint(style::mediaPlayerPanelMargins.left() * 0.5,
			(height() - _textRect.height()) / 2.));

		_speedSlider->setGeometry(
			_textRect.right() + style::mediaPlayerPanelMargins.left(),
			(height() - _speedSlider->height()) / 2.,
			width() - _textRect.right() - style::mediaPlayerPanelMargins.left() *
			1.5 - style::mediaPlayerPanelMargins.right(),
			_speedSlider->height());
	}

	SpeedController::SpeedController(QWidget* parent) :
		QPushButton(parent)
	{
		_overlay = new SpeedButtonOverlay(parent);

		connect(_overlay->_speedSlider, &QAbstractSlider::valueChanged, [this](int value) {
			_overlay->_speed = value / 10.;
			emit speedChanged(_overlay->_speed);
			});

		_overlay->_speedSlider->setValue(_overlay->_speed * 10.);
		_overlay->hide();

		const auto screenWidth = QApplication::primaryScreen()->availableGeometry().width();

		_overlay->resize(screenWidth / 12., 30);
		_speedButtonImage = QImage(":/assets/images/speed_button.png");

		setAttribute(Qt::WA_NoSystemBackground);
		setCursor(Qt::PointingHandCursor);
	}

	void SpeedController::paintEvent(QPaintEvent* event) {
		QPainter painter(this);
		painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

		painter.setPen(Qt::NoPen);
		painter.setBrush(Qt::NoBrush);

		if (_speedButtonImage.size() != size()) {
			_speedButtonImage = images::Prepare(_speedButtonImage, size());

			_speedButtonImage = std::move(_speedButtonImage).scaled(
				_speedButtonImage.width() * style::DevicePixelRatio(),
				_speedButtonImage.height() * style::DevicePixelRatio(),
				Qt::IgnoreAspectRatio,
				Qt::SmoothTransformation);

			_speedButtonImage.setDevicePixelRatio(style::DevicePixelRatio());
		}

		painter.drawImage(0, 0, _speedButtonImage);
	}

	void SpeedController::mousePressEvent(QMouseEvent* event) {
		_overlay->isHidden()
			? _overlay->show()
			: _overlay->hide();

		_overlay->move(
			width() - _overlay->width() + x(),
			height() - _overlay->height() + y() -
			style::mediaPlayerPanelMargins.top() * 2. -
			style::mediaPlayerPanelMargins.bottom() * 2.
		);

		_overlay->raise();
	}

	bool SpeedController::eventFilter(QObject* sender, QEvent* event) {
		switch (event->type()) {
		case QEvent::Enter:
			_overlay->show();

			_overlay->move(
				width() - _overlay->width() + x(),
				height() - _overlay->height() + y() -
				style::mediaPlayerPanelMargins.top() * 2. -
				style::mediaPlayerPanelMargins.bottom() * 2.
			);
			return true;

		case QEvent::Leave:
			_overlay->hide();
			return true;
		}

		return QPushButton::eventFilter(sender, event);
	}
} // namespace base::qt::ui