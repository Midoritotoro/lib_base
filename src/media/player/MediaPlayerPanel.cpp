#include <base/media/player/MediaPlayerPanel.h>

#include <base/media/player/SpeedButton.h>
#include <base/media/player/VolumeController.h>

#include <QPaintEvent>
#include <QMargins>
#include <QPainter>

#include <QCoreApplication>

#include <QDir>
#include <QFile>
#include <QPainterPath>

#include <QApplication>
#include <QScreen>

#include <base/core/system/Time.h>
#include <base/core/utility/Algorithm.h>

#include <base/qt/style/StyleCore.h>
#include <base/qt/style/StyleWidgets.h>

#include <base/media/player/FullScreenButton.h>
#include <base/qt/common/Size.h>


__BASE_QT_UI_NAMESPACE_BEGIN

MediaPlayerPanel::MediaPlayerPanel(QWidget* parent) :
	QWidget(parent)
{
	setContentsMargins(0, 0, 0, 0);

	const auto screenWidth = QApplication::primaryScreen()->availableGeometry().width();
	_mediaPlayerPanelWidth = screenWidth / 4;

	_speedButton = new SpeedController(parent);

	_videoStateWidget = new VideoStateWidget(this);
	_volumeToggle = new VolumeController(this);

	_volumeSlider = new EnhancedSlider(this);
	_playbackSlider = new EnhancedSlider(this);

	_timeLabel = new QLabel(this);
	_remainingTimeLabel = new QLabel(this);

	_fullScreenButton = new FullScreenButton(this);

	_timeLabel->setAttribute(Qt::WA_NoSystemBackground);
	_remainingTimeLabel->setAttribute(Qt::WA_NoSystemBackground);

	_playbackSlider->setStyleSheet(style::SliderStyle());
	_volumeSlider->setStyleSheet(style::SliderStyle());

	_fullScreenButton->setFixedSize(25, 25);
	_speedButton->setFixedSize(25, 25);

	_videoStateWidget->setFixedSize(30, 30);
	_volumeToggle->setFixedSize(25, 25);

	_volumeSlider->setFixedHeight(style::sliderHeight);
	_playbackSlider->setFixedHeight(style::sliderHeight);

	updateSize();
	updateControlsGeometry();

	connect(_volumeToggle, &QPushButton::clicked, this, [this]() {
		_volumeToggle->isSpeakerOn()
			? setVolume(0)
			: setVolume(_previousVolumeSliderValue);
		});

	connect(_volumeSlider, &QSlider::valueChanged, this, &MediaPlayerPanel::setVolume);

	connect(_fullScreenButton, &QAbstractButton::clicked, this, [this]() {
		_fullScreenButton->state() == FullScreenButton::State::FullScreenTo
			? emit needsNormal()
			: emit needsFullScreen();
		});

	connect(_videoStateWidget, &QPushButton::clicked, this, [this]() {
		switch (_videoStateWidget->state()) {
		case VideoStateWidget::State::Play:
			emit videoPlayClicked();
			break;

		case VideoStateWidget::State::Pause:
			emit videoPauseClicked();
			break;

		case VideoStateWidget::State::Repeat:
			emit videoRepeatClicked();
			break;
		}
		});
}

void MediaPlayerPanel::updateTimeText(
	int mediaPosition,
	int mediaDuration)
{
	const auto positionSeconds = (mediaPosition / 1000) % 60;
	const auto positionMinutes = (mediaPosition / 1000) / 60;

	const auto durationSeconds = (mediaDuration / 1000) % 60;
	const auto durationMinutes = (mediaDuration / 1000) / 60;

	const auto remainingMinutes = durationMinutes - positionMinutes;
	const auto remainingSeconds = durationSeconds - positionSeconds;

	_timeLabel->setText(QString("%1:%2")
		.arg(positionMinutes, 2, 10, QChar('0'))
		.arg(positionSeconds, 2, 10, QChar('0')));

	_remainingTimeLabel->setText(QString("-%1:%2")
		.arg(qMax(remainingMinutes, -remainingMinutes), 2, 10, QChar('0'))
		.arg(qMax(remainingSeconds, -remainingSeconds), 2, 10, QChar('0')));

	updateTimeSize();
}

void MediaPlayerPanel::updateStateWidget(VideoStateWidget::State state) {
	_videoStateWidget->setState(state);
}

EnhancedSlider* MediaPlayerPanel::playbackSlider() const noexcept {
	return _playbackSlider;
}

SpeedController* MediaPlayerPanel::speedController() const noexcept {
	return _speedButton;
}

void MediaPlayerPanel::setVideoSliderMaximum(int value) {
	_playbackSlider->setMaximum(value);
}

void MediaPlayerPanel::setVolume(int value) {
	_volumeToggle->setVolume(value);

	if (value > 0 && _volumeToggle->isSpeakerOn() == false)
		_volumeToggle->setSpeakerEnabled(true);
	else if (value == 0 && _volumeToggle->isSpeakerOn() == true)
		_volumeToggle->setSpeakerEnabled(false);

	if (value > 0)
		_previousVolumeSliderValue = value;

	_volumeSlider->setValue(value);
	emit needsChangeVolume(value);
}

void MediaPlayerPanel::updateSize() {
	const auto width = style::mediaPlayerPanelMargins.left() + _mediaPlayerPanelWidth + style::mediaPlayerPanelMargins.right();
	const auto height = style::mediaPlayerPanelMargins.top() + contentHeight() + style::mediaPlayerPanelMargins.bottom();

	resize(width, height);
}

void MediaPlayerPanel::updateTimeSize() {
	const auto timeLabelSize = common::TextSize(_timeLabel->text(), _timeLabel->font());
	const auto remainingTimeLabelSize = common::TextSize(_remainingTimeLabel->text(), _remainingTimeLabel->font());

	if (timeLabelSize.isNull() || remainingTimeLabelSize.isNull())
		return;

	_timeLabel->resize(timeLabelSize);
	_remainingTimeLabel->resize(remainingTimeLabelSize);
}

void MediaPlayerPanel::updateControlsGeometry() {
	_playbackSlider->setGeometry(
		_timeLabel->width() / 2.,
		height() - style::mediaPlayerPanelMargins.bottom() - _playbackSlider->height(),
		width() - _remainingTimeLabel->width() / 2. - _timeLabel->width() / 2.,
		_playbackSlider->height()
	);

	_volumeSlider->setGeometry(
		style::mediaPlayerPanelMargins.left() * 1.5 + _volumeToggle->width(),
		_volumeToggle->height() / 2. - style::mediaPlayerPanelMargins.top(),
		(width() - style::mediaPlayerPanelMargins.left() - style::mediaPlayerPanelMargins.right()) / 5.,
		_volumeSlider->height()
	);

	_volumeToggle->move(style::mediaPlayerPanelMargins.left(), style::mediaPlayerPanelMargins.top());
	_videoStateWidget->move((width() - _videoStateWidget->width()) / 2., style::mediaPlayerPanelMargins.top());

	_timeLabel->move(style::mediaPlayerPanelMargins.left(), height() + style::mediaPlayerPanelMargins.bottom() - _timeLabel->height());
	_remainingTimeLabel->move(width() + style::mediaPlayerPanelMargins.right() - _remainingTimeLabel->width() / 2.,
		height() + style::mediaPlayerPanelMargins.bottom() - _remainingTimeLabel->height());

	_speedButton->move(
		parentWidget()->width() - (width() * 1.5) - style::mediaPlayerPanelMargins.right() * 2 - style::mediaPlayerPanelMargins.left(),
		parentWidget()->height() - height() + style::mediaPlayerPanelMargins.top() * 0.5);

	_fullScreenButton->move(width() - style::mediaPlayerPanelMargins.right() - _fullScreenButton->width(), style::mediaPlayerPanelMargins.top());
}

void MediaPlayerPanel::paintEvent(QPaintEvent* event) {
	QPainter painter(this);

	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
	painter.setOpacity(0.75);

	painter.setBrush(Qt::black);
	painter.setPen(Qt::NoPen);

	style::RoundCorners(painter, size(), style::mediaPlayerPanelBorderRadius);
	painter.drawRect(rect());
}

void MediaPlayerPanel::resizeEvent(QResizeEvent* event) {
	updateSize();
	updateControlsGeometry();
}

int MediaPlayerPanel::contentHeight() const noexcept {
	return _videoStateWidget->height() + _playbackSlider->height()
		+ style::mediaPlayerPanelMargins.top() + style::mediaPlayerPanelMargins.bottom();
}

__BASE_QT_UI_NAMESPACE_END
