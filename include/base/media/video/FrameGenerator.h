#pragma once

#include "../Utility.h"
#include "../../../core/Time.h"


namespace FFmpeg {
class ThumbnailGenerator;

class FrameGenerator {
public:
	struct Frame {
		Time::time duration = 0;
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

	[[nodiscard]] Frame renderNext(
		QSize size,
		Qt::AspectRatioMode mode,
		bool fullScreen = false);
	[[nodiscard]] Frame renderCurrent(
		QSize size,
		Qt::AspectRatioMode mode,
		bool fullScreen = false);

	void setSpeed(float speed);
	void rewind(Time::time positionMs);

	[[nodiscard]] QSize resolution() const;

	[[nodiscard]] Time::time duration() const noexcept;
	[[nodiscard]] Time::time position() const noexcept;

	[[nodiscard]] Time::time frameDelay() const noexcept;
private:
	struct ReadFrame {
		FramePointer frame = nullptr;
		Time::time position = 0;
		Time::time duration = 0;
	};

	void readNextFrame();
	void resolveNextFrameTiming();

	[[nodiscard]] static int Read(void* opaque,
		uint8_t* buffer,
		int bufferSize);
	[[nodiscard]] static int64_t Seek(void* opaque,
		int64_t offset,
		int whence);

	[[nodiscard]] int read(
		uint8_t* buffer,
		int bufferSize);
	[[nodiscard]] int64_t seek(
		int64_t offset,
		int whence);

	[[nodiscard]] QSize recountMaximumFrameSize(const QSize& targetSize);

	FormatPointer _format = nullptr;
	CodecPointer _codec = nullptr;

	SwscalePointer _scale = nullptr;

	ReadFrame _current;
	ReadFrame _next;

	Time::time _framePosition = 0;

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

} // namespace FFmpeg
