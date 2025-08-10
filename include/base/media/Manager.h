#pragma once


#include <base/media/ffmpeg/video/FrameGenerator.h>
#include <base/media/ffmpeg/audio/AudioReader.h>

#include <QThread>
#include <QTimer>
#include <QMutex>

#include <memory>
#include <QObject>


__BASE_MEDIA_NAMESPACE_BEGIN

class Manager : public QObject {
	Q_OBJECT
public:
	enum class State {
		Paused,
		Playing,
		Stopped
	};
Q_SIGNALS:
	void endOfMedia();
	void needToRepaint(const QImage& frame, int timeout);

	void positionChanged(qint64 position);
	void durationChanged(qint64 duration);

	void playbackStateChanged(State state);
public:
	Manager();
	~Manager();

	Q_INVOKABLE void cleanUp();

	  Time::time_t duration() const noexcept;
	  Time::time_t position() const noexcept;

	Q_INVOKABLE void pause();
	Q_INVOKABLE void play();

	Q_INVOKABLE void setTargetSize(const QSize& size);
	Q_INVOKABLE void setDisplayType(bool showNormal);

	void setVideo(std::unique_ptr<ffmpeg::video::FrameGenerator>&& generator, const QSize& size = QSize());
	void setAudio(std::unique_ptr<ffmpeg::audio::AudioReader>&& audio);

	  bool hasVideo() const noexcept;
	  bool hasAudio() const noexcept;

	Q_INVOKABLE void rewind(Time::time_t position);
	Q_INVOKABLE void setSpeed(float speed);
private:
	void process();

	QTimer _timer;

	QThread* _thread = nullptr;
	QMutex _mutex;

	QSize _size;

	std::unique_ptr<ffmpeg::video::FrameGenerator> _frameGenerator = nullptr;
	std::unique_ptr<ffmpeg::audio::AudioReader> _audioReader = nullptr;

	State _state;

	bool _showNormal = false;
};

__BASE_MEDIA_NAMESPACE_END
