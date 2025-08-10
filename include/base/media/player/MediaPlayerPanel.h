#pragma once

#include <QWidget>
#include <QLabel>

#include <base/media/player/VideoStateWidget.h>
#include <base/qt/ui/EnhancedSlider.h>

#include <base/media/player/SpeedButton.h>


__BASE_QT_UI_NAMESPACE_BEGIN

class VolumeController;
class FullScreenButton;

class MediaPlayerPanel : public QWidget {
	Q_OBJECT
private:
	VideoStateWidget* _videoStateWidget = nullptr;
	EnhancedSlider* _playbackSlider = nullptr, * _volumeSlider = nullptr;
	QLabel* _timeLabel = nullptr, * _remainingTimeLabel = nullptr;

	FullScreenButton* _fullScreenButton = nullptr;
	SpeedController* _speedButton = nullptr;

	QString _fullScreenToImagePath;

	VolumeController* _volumeToggle = nullptr;
	int _previousVolumeSliderValue = 0, _mediaPlayerPanelWidth = 0;
public:
	MediaPlayerPanel(QWidget* parent = nullptr);

	void updateTimeText(
		int mediaPosition,
		int mediaDuration);
	void updateStateWidget(VideoStateWidget::State state);

	  EnhancedSlider* playbackSlider() const noexcept;
	  SpeedController* speedController() const noexcept;

	void setVideoSliderMaximum(int value);
	void setVolume(int value);
Q_SIGNALS:
	void videoRepeatClicked();
	void videoPlayClicked();
	void videoPauseClicked();

	void needsFullScreen();
	void needsNormal();
	void needsChangeVolume(int volume);
protected:
	void paintEvent(QPaintEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;
private:
	void updateSize();
	void updateTimeSize();

	void updateControlsGeometry();

	int contentHeight() const noexcept;
};

__BASE_QT_UI_NAMESPACE_END