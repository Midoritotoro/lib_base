#pragma once

#include <base/media/Manager.h>

#include <base/media/player/MediaPlayerPanel.h>
#include <base/qt/common/WidgetsHider.h>

#include <QWidget>

#include <base/media/MediaCommon.h>
#include <base/qt/ui/BaseWidget.h>

#include <base/core/thread/Thread.h>


__BASE_QT_STYLE_NAMESPACE_BEGIN

struct MediaPlayerStyle: public StyleBase {

};

__BASE_QT_STYLE_NAMESPACE_END

__BASE_MEDIA_NAMESPACE_BEGIN

class MediaPlayer final : 
	public qt::ui::CoreWidget<qt::style::MediaPlayerStyle> 
{
	Q_OBJECT
public:
	MediaPlayer(QWidget* parent = nullptr);

	void setMedia(const QString& path);

	NODISCARD int getVideoControlsHeight() const noexcept;

	NODISCARD QSize occupiedMediaSpace() const noexcept;
	NODISCARD QPoint mediaPosition() const noexcept;

	NODISCARD Manager::State playbackState() const noexcept;

	void setFullScreen();
	void setNormal();

	void play();
	void pause();

	void cleanUp();
	void rewind(Time::time_t positionMs);

	void changeVolume(int value);
Q_SIGNALS:
	void mediaGeometryChanged();
	void needScrollToMessage();
protected:
	void resizeEvent(QResizeEvent* event) override;
	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event);
private:
	enum class MediaDisplayType {
		FullScreen,
		Normal
	};

	void paintBackground(
		QPainter& painter,
		QPaintEvent* event);
	void updatePanelVisibility();

	std::unique_ptr<Manager> _manager = nullptr;
	base::qt::ui::MediaPlayerPanel* _mediaPlayerPanel = nullptr;

	std::unique_ptr<base::qt::ui::WidgetsHider> _widgetsHider = nullptr;

	QImage _current;
	QRect _currentFrameRect;

	Manager::State _playbackState;

	Time::time_t _currMs;

	MediaDisplayType _displayType = MediaDisplayType::Normal;
	media::Type _currentMediaType = media::Type::Unknown;

	QString _currentMediaPath;
	uint32 _currFPS = 0;
};

__BASE_MEDIA_NAMESPACE_END