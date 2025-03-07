#include <base/media/Manager.h>

#include <QDebug>
#include <QAbstractEventDispatcher>

#include <functional>
#include <base/core/async/Concurrent.h>


__BASE_MEDIA_NAMESPACE_BEGIN

Manager::Manager() :
	_state(State::Stopped)
{
	_thread = new QThread;

	connect(_thread, &QThread::started, this, [=] { process(); });

	_timer.setSingleShot(true);
	_timer.moveToThread(_thread);

	connect(&_timer, &QTimer::timeout, this, [=] { process(); });

	moveToThread(_thread);

	emit playbackStateChanged(_state);
}

Manager::~Manager() {
	_thread->quit();
	cleanUp();
}

void Manager::cleanUp() {
	QMutexLocker locker(&_mutex);

	_state = State::Stopped;
	emit playbackStateChanged(_state);

	if (hasVideo()) {
		_frameGenerator.reset();
		_frameGenerator = nullptr;
	}

	if (hasAudio()) {
		_audioReader.reset();
		_audioReader = nullptr;
	}

	if (_thread->isRunning())
		_thread->quit();

	_state = State::Playing;
}

Time::time_t Manager::duration() const noexcept {
	return hasVideo()
		? _frameGenerator->duration()
		: 0;
}

Time::time_t Manager::position() const noexcept {
	return hasVideo()
		? _frameGenerator->position()
		: 0;
}

void Manager::pause() {
	_state = State::Paused;
	emit playbackStateChanged(State::Paused);
}

void Manager::play() {
	_state = State::Playing;
	emit playbackStateChanged(State::Playing);

	_timer.start(1);
}

void Manager::process() {
	if (_state != State::Playing)
		return;

	if (hasVideo()) {
		const auto ms = Time::now();
		const auto frame = _frameGenerator->renderNext(_size, Qt::IgnoreAspectRatio, !_showNormal);

		if (frame.last)
			return emit endOfMedia();

		emit positionChanged(_frameGenerator->position());

		auto nowMs = Time::now() - ms;

		concurrent::invokeAsync([=] { emit needToRepaint(frame.image, 1000 / std::max(time_t(1), nowMs));  });

		const auto timeout = std::max(time_t(1), _frameGenerator->frameDelay() - nowMs);
		_timer.start(timeout);
	}
}

void Manager::setTargetSize(const QSize& size) {
	QMutexLocker locker(&_mutex);
	_size = size;
}

void Manager::setDisplayType(bool showNormal) {
	QMutexLocker locker(&_mutex);
	_showNormal = showNormal;
}

void Manager::setVideo(std::unique_ptr<ffmpeg::video::FrameGenerator>&& generator, const QSize& size) {
	QMutexLocker locker(&_mutex);

	_size = size;
	_frameGenerator = std::move(generator);

	emit durationChanged(_frameGenerator->duration());

	if (!_thread->isRunning())
		_thread->start();
	_thread->setPriority(QThread::TimeCriticalPriority);
}

void Manager::setAudio(std::unique_ptr<ffmpeg::audio::AudioReader>&& audio) {
	QMutexLocker locker(&_mutex);

	_audioReader = std::move(audio);
	emit durationChanged(_audioReader->duration());

	if (!_thread->isRunning())
		_thread->start();

	_thread->setPriority(QThread::TimeCriticalPriority);
}

bool Manager::hasVideo() const noexcept {
	return _frameGenerator != nullptr;
}


bool Manager::hasAudio() const noexcept {
	return _audioReader != nullptr;
}

void Manager::rewind(Time::time_t position) {
	QMutexLocker locker(&_mutex);

	emit positionChanged(position);

	if (hasVideo()) {
		_frameGenerator->rewind(position);

		const auto frame = _frameGenerator->renderNext(_size, Qt::IgnoreAspectRatio, _showNormal);
		emit needToRepaint(frame.image, 0);

		if (_frameGenerator->position() >= _frameGenerator->duration() - _frameGenerator->frameDelay())
			emit endOfMedia();
		else if (_state == State::Playing)
			play();
	}

	if (hasAudio()) {
		_audioReader->open(position);

		if (_audioReader->position() >= _audioReader->duration())
			emit endOfMedia();
		else if (_state == State::Playing)
			play();
	}
}

void Manager::setSpeed(float speed) {
	QMutexLocker locker(&_mutex);
	if (hasVideo())
		_frameGenerator->setSpeed(speed);
}

__BASE_MEDIA_NAMESPACE_END
