#pragma once

#include <base/media/ffmpeg/Utility.h>
#include <base/core/system/Time.h>

#include <base/media/ffmpeg/video/Scale.h>


__BASE_MEDIA_FFMPEG_VIDEO_NAMESPACE_BEGIN

class ThumbnailGenerator;

class FrameGenerator {
public:
	struct Frame {
		Time::time_t duration = 0;
		QImage image;
		bool last = false;
	};

	FrameGenerator(
		const QByteArray& bytes,
		int swscaleFlags = 0,
		bool findStreamInfo = true,
		bool createCodec = true);
	FrameGenerator(
		const QString& path,
		int swscaleFlags = 0,
		bool findStreamInfo = true,
		bool createCodec = true);

	NODISCARD Frame renderNext(
		QSize size,
		Qt::AspectRatioMode mode,
		bool fullScreen = false);
	NODISCARD Frame renderCurrent(
		QSize size,
		Qt::AspectRatioMode mode,
		bool fullScreen = false);

	void setSpeed(float speed);
	void rewind(Time::time_t positionMs);

	NODISCARD QSize resolution() const;

	NODISCARD Time::time_t duration() const noexcept;
	NODISCARD Time::time_t position() const noexcept;

	NODISCARD Time::time_t frameDelay() const noexcept;
private:
	struct ReadFrame {
		FramePointer frame = nullptr;
		Time::time_t position = 0;
		Time::time_t duration = 0;
	};

	void readNextFrame();
	void resolveNextFrameTiming();

	NODISCARD static int Read(void* opaque,
		uint8_t* buffer,
		int bufferSize);
	NODISCARD static int64_t Seek(void* opaque,
		int64_t offset,
		int whence);

	NODISCARD int read(
		uint8_t* buffer,
		int bufferSize);
	NODISCARD int64_t seek(
		int64_t offset,
		int whence);

	NODISCARD QSize recountMaximumFrameSize(const QSize& targetSize);

	FormatPointer _format = nullptr;
	CodecPointer _codec = nullptr;

	SwscalePointer _scale = nullptr;

	ReadFrame _current;
	ReadFrame _next;

	Time::time_t _framePosition = 0;

	int _deviceOffset = 0;
	int _bestVideoStreamId = 0;

	int _nextFrameDelay = 0;
	int _currentFrameDelay = 0;
	int _frameIndex = 0;

	float _speed = 1.0f;

	QByteArray _bytes = nullptr;

	AVDictionary* _avOptions = nullptr;

	int _swscaleFlags = 0;

	friend class ThumbnailGenerator;
};


__BASE_MEDIA_FFMPEG_VIDEO_NAMESPACE_END
