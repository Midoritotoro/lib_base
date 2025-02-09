#pragma once

#ifndef __STDC_CONSTANT_MACROS
	#define __STDC_CONSTANT_MACROS
#endif

#include <base/Types.h>

extern "C" {
    #include <libavfilter/avfilter.h>
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
	#include <libswscale/swscale.h>
	#include <libswresample/swresample.h>
    #include <libavutil/avutil.h>
	#include <libavformat/avio.h>
} // extern "C"

#include <string>
#include <memory>

#include <QFile>
#include <QSize>
#include <qDebug>

#include <QImage>

// #include "video/Scale.h"


#define DA_FFMPEG_NEW_CHANNEL_LAYOUT (LIBAVUTIL_VERSION_INT >= \
	AV_VERSION_INT(57, 28, 100))

#define DA_FFMPEG_CONST_WRITE_CALLBACK (LIBAVFORMAT_VERSION_INT >= \
	AV_VERSION_INT(61, 01, 100))

#define FFMPEG_HAVE_DURATION (LIBAVUTIL_VERSION_INT >= \
	AV_VERSION_INT(58, 02, 100))


namespace base::media::ffmpeg::video {
	constexpr auto kAvioBlockSize = 4096;

class AvErrorWrap {
public:
	AvErrorWrap(int code = 0) : _code(code) {
	}

	[[nodiscard]] bool failed() const {
		return (_code < 0);
	}
	[[nodiscard]] explicit operator bool() const {
		return failed();
	}

	[[nodiscard]] int code() const {
		return _code;
	}

	[[nodiscard]] std::string text() const {
		char string[AV_ERROR_MAX_STRING_SIZE] = { 0 };
		return std::string(av_make_error_string(
			string,
			sizeof(string),
			_code));
	}

private:
	int _code = 0;
};

class Packet {
public:
	Packet() = default;
	Packet(Packet&& other) : _data(std::exchange(other._data, {})) {}

	Packet& operator=(Packet&& other) noexcept {
		if (this != &other) {
			release();
			_data = std::exchange(other._data, {});
		}
		return *this;
	}

	~Packet() {
		release();
	}

	[[nodiscard]] AVPacket& fields() {
		if (!_data)
			_data = av_packet_alloc();
		return *_data;
	}

	[[nodiscard]] const AVPacket& fields() const {
		if (!_data)
			_data = av_packet_alloc();
		return *_data;
	}

	[[nodiscard]] bool empty() const {
		return !_data || !fields().data;
	}

	void release() {
		av_packet_free(&_data);
	}

private:
	mutable AVPacket* _data = nullptr;
};

struct IODeleter {
	void operator()(AVIOContext* value);
};

struct FormatDeleter {
	void operator()(AVFormatContext* value);
};

struct CodecDeleter {
	void operator()(AVCodecContext* value);
};

struct FrameDeleter {
	void operator()(AVFrame* value);
};

struct SwscaleDeleter {
	QSize srcSize;
	int srcFormat = int(AV_PIX_FMT_NONE);
	QSize dstSize;
	int dstFormat = int(AV_PIX_FMT_NONE);

	void operator()(SwsContext* value);

};

struct SwresampleDeleter {
	AVSampleFormat srcFormat = AV_SAMPLE_FMT_NONE;
	int srcRate = 0;
	int srcChannels = 0;
	AVSampleFormat dstFormat = AV_SAMPLE_FMT_NONE;
	int dstRate = 0;
	int dstChannels = 0;

	void operator()(SwrContext* value);
};

struct CodecDescriptor {
	AVStream* stream;
	bool hwAllowed = false;
};


using IOPointer = std::unique_ptr<AVIOContext, IODeleter>;
using FormatPointer = std::unique_ptr<AVFormatContext, FormatDeleter>;

using CodecPointer = std::unique_ptr<AVCodecContext, CodecDeleter>;
using FramePointer = std::unique_ptr<AVFrame, FrameDeleter>;

using SwscalePointer = std::unique_ptr<SwsContext, SwscaleDeleter>;
using SwresamplePointer = std::unique_ptr<SwrContext, SwresampleDeleter>;


[[nodiscard]] IOPointer MakeIOPointer(
	void* opaque,
	int(*read)(void* opaque, uint8_t* buffer, int bufferSize),
#if DA_FFMPEG_CONST_WRITE_CALLBACK
	int(*write)(void* opaque, const uint8_t* buffer, int bufferSize),
#else
	int(*write)(void* opaque, uint8_t* buffer, int bufferSize),
#endif
	int64_t(*seek)(void* opaque, int64_t offset, int whence));

[[nodiscard]] IOPointer MakeIOPointer(
	void* opaque,
	unsigned char* buffer,
	int(*read)(void* opaque, uint8_t* buffer, int bufferSize),
#if DA_FFMPEG_CONST_WRITE_CALLBACK
	int(*write)(void* opaque, const uint8_t* buffer, int bufferSize),
#else
	int(*write)(void* opaque, uint8_t* buffer, int bufferSize),
#endif
	int64_t(*seek)(void* opaque, int64_t offset, int whence));


[[nodiscard]] FormatPointer MakeFormatPointer(
	void* opaque,
	int(*read)(void* opaque, uint8_t* buffer, int bufferSize),
#if DA_FFMPEG_CONST_WRITE_CALLBACK
	int(*write)(void* opaque, const uint8_t* buffer, int bufferSize),
#else
	int(*write)(void* opaque, uint8_t* buffer, int bufferSize),
#endif
	int64_t(*seek)(void* opaque, int64_t offset, int whence));


[[nodiscard]] FormatPointer MakeFormatPointer(const QString& path);
[[nodiscard]] FormatPointer MakeFormatPointer(IOPointer* ioPointer);

[[nodiscard]] CodecPointer MakeCodecPointer(CodecDescriptor descriptor);

[[nodiscard]] FramePointer MakeFramePointer();
[[nodiscard]] FramePointer DuplicateFramePointer(AVFrame* frame);

[[nodiscard]] bool FrameHasData(AVFrame* frame);
void ClearFrameMemory(AVFrame* frame);


[[nodiscard]] SwscalePointer MakeSwscalePointer(
	QSize srcSize,
	int srcFormat,
	QSize dstSize,
	int dstFormat,
	SwscalePointer* existing = nullptr,
	int flags = 0);

[[nodiscard]] SwscalePointer MakeSwscalePointer(
	AVFrame* frame,
	QSize resize,
	SwscalePointer* existing = nullptr,
	int flags = 0);


[[nodiscard]] SwresamplePointer MakeSwresamplePointer(
#if DA_FFMPEG_NEW_CHANNEL_LAYOUT
	AVChannelLayout* srcLayout,
#else // DA_FFMPEG_NEW_CHANNEL_LAYOUT
	uint64_t srcLayout,
#endif // DA_FFMPEG_NEW_CHANNEL_LAYOUT
	AVSampleFormat srcFormat,
	int srcRate,
#if DA_FFMPEG_NEW_CHANNEL_LAYOUT
	AVChannelLayout* dstLayout,
#else // DA_FFMPEG_NEW_CHANNEL_LAYOUT
	uint64_t dstLayout,
#endif // DA_FFMPEG_NEW_CHANNEL_LAYOUT
	AVSampleFormat dstFormat,
	int dstRate,
	SwresamplePointer* existing = nullptr);

[[nodiscard]] CodecPointer MakeCodecPointer(CodecDescriptor descriptor);

[[nodiscard]] FramePointer MakeFramePointer();
[[nodiscard]] FramePointer DuplicateFramePointer(AVFrame* frame);

[[nodiscard]] bool FrameHasData(AVFrame* frame);
void ClearFrameMemory(AVFrame* frame);

[[nodiscard]] const AVCodec* FindDecoder(AVCodecContext* context);

struct EnqueuedFrame {
	int64_t position = 0;
	int64_t samples = 0;
	FramePointer frame = nullptr;

	EnqueuedFrame& operator=(const EnqueuedFrame& other) {
		position = other.position;
		samples = other.samples;
		frame = FramePointer(other.frame.get());
		return *this;
	}
	EnqueuedFrame() {};
	EnqueuedFrame(const EnqueuedFrame& _frame) {
		*this = _frame;
	}
};

} // namespace base::media::ffmpeg::video
