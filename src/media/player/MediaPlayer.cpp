#include <base/media/player/MediaPlayer.h>

#include <QByteArray>

#include <QPainter>
#include <QMouseEvent>
#include <QMimeDatabase>

#include <QShortcut>
#include <QMetaObject>

#include <base/qt/style/StyleCore.h>

#include <base/media/player/VideoStateWidget.h>

#include <base/images/ImagesPrepare.h>
#include <base/Utility.h>

extern "C" {

}


namespace {
	inline constexpr auto kPanelBottomIndent = 5;

	[[nodiscard]] QByteArray ReadFile(const QString& filepath) {
		auto file = QFile(filepath);
		return file.open(QIODevice::ReadOnly)
			? file.readAll()
			: QByteArray();
	}
} // namespace

namespace base::qt::ui {

	MediaPlayer::MediaPlayer(QWidget* parent) :
		QWidget(parent)
		, _manager(std::make_unique<Manager>())
	{
		_mediaPlayerPanel = new MediaPlayerPanel(this);
		_widgetsHider = std::make_unique<WidgetsHider>(false, true, QWidgetList({ _mediaPlayerPanel, _mediaPlayerPanel->speedController() }));

		_widgetsHider->SetInactivityDuration(3000);
		_widgetsHider->SetAnimationDuration(3000);

		setNormal();

		setContextMenuPolicy(Qt::ActionsContextMenu);

		setAttribute(Qt::WA_TranslucentBackground);

		setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
		setMouseTracking(true);

		addAction(QString::fromUtf8("Перейти к сообщению"), [this]() {
			emit needScrollToMessage();
			});

		connect(_manager.get(), &Manager::endOfMedia, this, [this] {
			_mediaPlayerPanel->updateStateWidget(VideoStateWidget::State::Repeat);
			qDebug() << "Media player time taken: " << static_cast<double>(Time::now() - _currMs) / 1000 << " seconds";
			if (_manager->hasVideo() || _manager->hasAudio())
				cleanUp();
			});

		connect(_manager.get(), &Manager::durationChanged, _mediaPlayerPanel, &MediaPlayerPanel::setVideoSliderMaximum);
		connect(_manager.get(), &Manager::positionChanged, this, [this](qint64 position) {
			disconnect(_mediaPlayerPanel->playbackSlider(), &QSlider::valueChanged, _manager.get(), &Manager::rewind);
			_mediaPlayerPanel->playbackSlider()->setValue(position);

			_mediaPlayerPanel->updateTimeText(position, _manager->duration());
			connect(_mediaPlayerPanel->playbackSlider(), &QSlider::valueChanged, _manager.get(), &Manager::rewind);
			});

		connect(_manager.get(), &Manager::playbackStateChanged, this, [this](Manager::State state) {
			_playbackState = state;
			switch (state) {
			case Manager::State::Playing:
				_mediaPlayerPanel->updateStateWidget(VideoStateWidget::State::Pause);
				break;

			case Manager::State::Paused:
				_mediaPlayerPanel->updateStateWidget(VideoStateWidget::State::Play);
				break;
			}
			});

		connect(_mediaPlayerPanel, &MediaPlayerPanel::videoRepeatClicked, [this] {
			rewind(0);
			_currMs = Time::now();
			});

		connect(_mediaPlayerPanel, &MediaPlayerPanel::videoPlayClicked, this, &MediaPlayer::play);
		connect(_mediaPlayerPanel, &MediaPlayerPanel::videoPauseClicked, this, &MediaPlayer::pause);

		connect(_mediaPlayerPanel, &MediaPlayerPanel::needsNormal, this, &MediaPlayer::setNormal);
		connect(_mediaPlayerPanel, &MediaPlayerPanel::needsFullScreen, this, &MediaPlayer::setFullScreen);

		connect(_mediaPlayerPanel->playbackSlider(), &QSlider::valueChanged, _manager.get(), &Manager::rewind);
		connect(_mediaPlayerPanel, &MediaPlayerPanel::needsChangeVolume, this, &MediaPlayer::changeVolume);

		connect(_mediaPlayerPanel->speedController(), &SpeedController::speedChanged, _manager.get(), &Manager::setSpeed);

		_mediaPlayerPanel->setVolume(20);

		connect(_manager.get(), &Manager::needToRepaint, this, [this](const QImage& image, int fps) {
			_currFPS = fps;
			_current = image;

			update();
			});

		QShortcut* videoStateShortcut = new QShortcut(QKeySequence(Qt::Key_Space), this);

		connect(videoStateShortcut, &QShortcut::activated, [this] {
			playbackState() == Manager::State::Playing
				? pause()
				: play();
			});
	}

	void MediaPlayer::setMedia(const QString& path) {
		if (_manager->hasVideo() || _manager->hasAudio()) {
			pause();
			cleanUp();
		}

		_currentMediaType = Media::detectMediaType(path);
		_currentMediaPath = path;

		if (_currentMediaType == Media::Type::Unknown)
			return;

		updatePanelVisibility();
		const auto data = ReadFile(path);

		switch (_currentMediaType) {
		case Media::Type::Video:
			_manager->setVideo(std::move(std::make_unique<FFmpeg::FrameGenerator>(data, SWS_BICUBIC)));
			_mediaPlayerPanel->updateStateWidget(VideoStateWidget::State::Pause);
			play();

			_currMs = Time::now();
			break;
		case Media::Type::Photo:
			_current.loadFromData(data);
			_current = images::Prepare(_current, 0.7);

			update();
			break;

		case Media::Type::Audio:
			_manager->setAudio(std::move(std::make_unique<FFmpeg::AudioReader>(data)));
			_mediaPlayerPanel->updateStateWidget(VideoStateWidget::State::Pause);
			play();

			_currMs = Time::now();
			break;
		}
	}

	int MediaPlayer::getVideoControlsHeight() const noexcept {
		return !_mediaPlayerPanel->isHidden()
			? _mediaPlayerPanel->height()
			: 0;
	}

	QSize MediaPlayer::occupiedMediaSpace() const noexcept {
		return _currentFrameRect.size();
	}

	QPoint MediaPlayer::mediaPosition() const noexcept {
		return QPoint(_currentFrameRect.x(), _currentFrameRect.y());
	}

	Manager::State MediaPlayer::playbackState() const noexcept {
		return _playbackState;
	}

	void MediaPlayer::play() {
		if (playbackState() != Manager::State::Stopped)
			QMetaObject::invokeMethod(_manager.get(), "play", Qt::QueuedConnection);
	}

	void MediaPlayer::pause() {
		QMetaObject::invokeMethod(_manager.get(), "pause", Qt::QueuedConnection);
	}

	void MediaPlayer::rewind(Time::time_t positionMs) {
		if (_manager->hasVideo() == false && _currentMediaPath.isEmpty() == false)
			setMedia(_currentMediaPath);
		QMetaObject::invokeMethod(_manager.get(), "rewind", Qt::QueuedConnection, Q_ARG(Time::time_t, positionMs));
	}

	void MediaPlayer::cleanUp() {
		QMetaObject::invokeMethod(_manager.get(), "cleanUp", Qt::QueuedConnection);
	}

	void MediaPlayer::resizeEvent(QResizeEvent* event) {
		_mediaPlayerPanel->move((width() - _mediaPlayerPanel->width()) / 2,
			height() - _mediaPlayerPanel->height() - kPanelBottomIndent);
	}

	void MediaPlayer::paintEvent(QPaintEvent* event) {
		auto painter = QPainter(this);
		paintBackground(painter, event);

		painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

		const auto center = _current.size().width() < size().width()
			|| _current.size().height() < size().height()
			? QPoint((width() - _current.width()) / 2, (height() - _current.height()) / 2) : QPoint(0, 0);

		_currentFrameRect = QRect(center, _current.size());

		//if (_current.size().width() <= size().width() && _current.size().height() <= size().height())
		//	painter.drawImage(center, _current);
		//else
		//	painter.drawImage(QRect(QPoint(0, 0), size()), _current);

		const auto text = "Fps: " + QString::number(_currFPS);
		const auto frameTopLeft = _currentFrameRect.topLeft();

		const auto textSize = QFontMetrics(painter.font()).size(0, text);

		painter.setPen(Qt::red);
		painter.drawText(QPoint(
			frameTopLeft.x() + textSize.width(),
			frameTopLeft.y() + textSize.height()), text);
	}

	void MediaPlayer::mousePressEvent(QMouseEvent* event) {
		if (!(event->button() == Qt::LeftButton && _currentFrameRect.contains(event->pos())))
			return;

		switch (playbackState()) {

		case Manager::State::Playing:
			pause();
			break;

		case Manager::State::Paused:
			if (_manager->hasVideo() == false && _currentMediaPath.isEmpty() == false && _currentMediaType == Media::Type::Video)
				setMedia(_currentMediaPath);

			if ((_manager->duration() - _manager->position()) <= 100)
				rewind(0);

			play();
			break;
		}
	}


	void MediaPlayer::updatePanelVisibility() {
		switch (_currentMediaType) {
		case Media::Type::Video:
			_widgetsHider->addWidget(_mediaPlayerPanel);
			_widgetsHider->addWidget(_mediaPlayerPanel->speedController());

			_widgetsHider->resetTimer();
			_mediaPlayerPanel->show();

			break;

		case Media::Type::Photo:
			_widgetsHider->removeWidget(_mediaPlayerPanel);
			_widgetsHider->removeWidget(_mediaPlayerPanel->speedController());

			_widgetsHider->resetTimer();
			_mediaPlayerPanel->hide();

			break;

		case Media::Type::Audio:
			_widgetsHider->addWidget(_mediaPlayerPanel);
			_widgetsHider->addWidget(_mediaPlayerPanel->speedController());

			_widgetsHider->resetTimer();
			_mediaPlayerPanel->show();

			break;
		}
	}

	void MediaPlayer::setFullScreen() {
		_displayType = MediaDisplayType::FullScreen;
		_manager->setDisplayType(false);

		_manager->setTargetSize(size());
		emit mediaGeometryChanged();

		update();
	}

	void MediaPlayer::setNormal() {
		_displayType = MediaDisplayType::Normal;
		_manager->setDisplayType(true);

		_manager->setTargetSize(QSize());
		emit mediaGeometryChanged();

		update();
	}

	void MediaPlayer::changeVolume(int value) {
		
	}

	void MediaPlayer::paintBackground(
		QPainter& painter,
		QPaintEvent* event)
	{
		const auto opacity = painter.opacity();
		painter.setOpacity(0.5);

		if (const auto fill = rect().intersected(event->rect()); !fill.isNull())
			painter.fillRect(fill, Qt::black);

		painter.setOpacity(opacity);
	}
} // namespace base::qt::ui