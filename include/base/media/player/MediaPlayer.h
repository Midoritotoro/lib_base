#pragma once

#include <base/media/Manager.h>

#include <base/media/player/MediaPlayerPanel.h>
#include <base/qt/common/WidgetsHider.h>

#include <QWidget>

#include <base/media/MediaCommon.h>

namespace base::media {
	class MediaPlayer final : public QWidget {
		Q_OBJECT
	public:
		MediaPlayer(QWidget* parent = nullptr);

		void setMedia(const QString& path);

		[[nodiscard]] int getVideoControlsHeight() const noexcept;

		[[nodiscard]] QSize occupiedMediaSpace() const noexcept;
		[[nodiscard]] QPoint mediaPosition() const noexcept;

		[[nodiscard]] Manager::State playbackState() const noexcept;

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
}