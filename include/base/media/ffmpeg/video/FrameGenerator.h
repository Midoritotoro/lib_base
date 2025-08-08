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

	base_nodiscard Frame renderNext(
		QSize size,
		Qt::AspectRatioMode mode,
		bool fullScreen = false);
	base_nodiscard Frame renderCurrent(
		QSize size,
		Qt::AspectRatioMode mode,
		bool fullScreen = false);

	void setSpeed(float speed);
	void rewind(Time::time_t positionMs);

	base_nodiscard QSize resolution() const;

	base_nodiscard Time::time_t duration() const noexcept;
	base_nodiscard Time::time_t position() const noexcept;

	base_nodiscard Time::time_t frameDelay() const noexcept;
private:
	struct ReadFrame {
		FramePointer frame = nullptr;
		Time::time_t position = 0;
		Time::time_t duration = 0;
	};

	void readNextFrame();
	void resolveNextFrameTiming();

	base_nodiscard static int Read(void* opaque,
		uint8_t* buffer,
		int bufferSize);
	base_nodiscard static int64_t Seek(void* opaque,
		int64_t offset,
		int whence);

	base_nodiscard int read(
		uint8_t* buffer,
		int bufferSize);
	base_nodiscard int64_t seek(
		int64_t offset,
		int whence);

	base_nodiscard QSize recountMaximumFrameSize(const QSize& targetSize);

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
