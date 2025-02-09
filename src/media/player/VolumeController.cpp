#include <base/media/player/VolumeController.h>
#include <base/qt/style/StyleScale.h>

#include <QPainter>
#include <QMouseEvent>


namespace base::qt::ui {
	VolumeController::VolumeController(QWidget * parent):
		QPushButton(parent)
		, _isSpeakerOn(false)
		, _isVolumeValueSmall(true)
	{
		setAttribute(Qt::WA_NoSystemBackground);
		setCursor(Qt::PointingHandCursor);

		_speakerOff = QImage(":/assets/images/speaker_white_off.png");
		_speakerOn = QImage(":/assets/images/speaker_white_on.png");
		_speakerSmallOn = QImage(":/assets/images/speaker_white_small_on.png");

		qDebug() << "_speakerOn: " << _speakerOn.size();
	}


	void VolumeController::paintSpeakerOff(QPainter& painter) {
		_speakerOff = std::move(_speakerOff).scaled(
			_speakerOff.width() * style::DevicePixelRatio(),
			_speakerOff.height() * style::DevicePixelRatio(),
			Qt::IgnoreAspectRatio,
			Qt::SmoothTransformation);

		_speakerOff.setDevicePixelRatio(style::DevicePixelRatio());
		painter.drawImage(0, 0, _speakerOff);
	}

	void VolumeController::paintSpeakerOn(QPainter& painter) {
		auto& image = _isVolumeValueSmall
			? _speakerSmallOn
			: _speakerOn;

		image = std::move(image).scaled(
			image.width() * style::DevicePixelRatio(),
			image.height() * style::DevicePixelRatio(),
			Qt::IgnoreAspectRatio,
			Qt::SmoothTransformation);

		image.setDevicePixelRatio(style::DevicePixelRatio());
		painter.drawImage(0, 0, image);
	}

	void VolumeController::paintEvent(QPaintEvent* event) {
		QPainter painter(this);
		painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

		painter.setPen(Qt::NoPen);
		painter.setBrush(Qt::NoBrush);

		_isSpeakerOn
			? paintSpeakerOn(painter)
			: paintSpeakerOff(painter);
	}

	bool VolumeController::isSpeakerOn() const noexcept {
		return _isSpeakerOn;
	}

	void VolumeController::setSpeakerEnabled(bool enabled) {
		_isSpeakerOn = enabled;
		update();
	}

	void VolumeController::setVolume(int volume) {
		volume > 50
			? _isVolumeValueSmall = false
			: _isVolumeValueSmall = true;
	}
} // namespace base::qt::ui