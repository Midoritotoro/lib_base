#include <base/media/ffmpeg/Utility.h>

extern "C" {
	#include <libavutil/imgutils.h>
	#include <libavutil/opt.h>
} // extern "C"

#include <base/media/ffmpeg/video/HardwareAcceleration.h>

#if defined OS_WIN
	#include <Windows.h>
#endif // OS_WIN

#include <qDebug>

#include <base/core/system/Time.h>
#include <base/core/utility/Algorithm.h>

__BASE_MEDIA_FFMPEG_NAMESPACE_BEGIN

void IODeleter::operator()(AVIOContext* value) {
	if (value) {
		av_freep(&value->buffer);
		avio_context_free(&value);
	}
}

void FormatDeleter::operator()(AVFormatContext* value) {
	if (value) {
		const auto deleter = IOPointer(value->pb);
		avformat_close_input(&value);
	}
}

void CodecDeleter::operator()(AVCodecContext* value) {
	if (value)
		avcodec_free_context(&value);
}

void FrameDeleter::operator()(AVFrame* value) {
	if (value)
		av_frame_free(&value);
}

void SwscaleDeleter::operator()(SwsContext* value) {
	if (value)
		sws_freeContext(value);
}

void SwresampleDeleter::operator()(SwrContext* value) {
	if (value)
		swr_free(&value);
}

IOPointer MakeIOPointer(
	void* opaque,
	int(*read)(void* opaque, uint8_t* buffer, int bufferSize),
#if DA_FFMPEG_CONST_WRITE_CALLBACK
	int(*write)(void* opaque, const uint8_t* buffer, int bufferSize),
#else
	int(*write)(void* opaque, uint8_t* buffer, int bufferSize),
#endif
	int64_t(*seek)(void* opaque, int64_t offset, int whence)
) {
	auto buffer = reinterpret_cast<unsigned char*>(av_malloc(kAvioBlockSize));
	if (!buffer)
		return {};

	auto pointer = IOPointer(avio_alloc_context(
		buffer, kAvioBlockSize, write ? 1 : 0,
		opaque, read, write, seek));

	if (!pointer)
		return {};
	
	return pointer;
}

IOPointer MakeIOPointer(
	void* opaque,
	unsigned char* buffer,
	int(*read)(void* opaque, uint8_t* buffer, int bufferSize),
#if DA_FFMPEG_CONST_WRITE_CALLBACK
	int(*write)(void* opaque, const uint8_t* buffer, int bufferSize),
#else
	int(*write)(void* opaque, uint8_t* buffer, int bufferSize),
#endif
	int64_t(*seek)(void* opaque, int64_t offset, int whence))
{
	auto pointer = IOPointer(avio_alloc_context(
		buffer, kAvioBlockSize, write ? 1 : 0,
		opaque, read, write, seek));

	if (!pointer)
		return {};

	return pointer;
}

FormatPointer MakeFormatPointer(
	void* opaque,
	int(*read)(void* opaque, uint8_t* buffer, int bufferSize),
#if DA_FFMPEG_CONST_WRITE_CALLBACK
	int(*write)(void* opaque, const uint8_t* buffer, int bufferSize),
#else
	int(*write)(void* opaque, uint8_t* buffer, int bufferSize),
#endif
	int64_t(*seek)(void* opaque, int64_t offset, int whence)
) {
	//const auto ms = Time::now();
	//const auto timer = gsl::finally([=] { qDebug() << "MakeFormatPointer: " << Time::now() - ms << " ms"; });

	auto ioPointer = MakeIOPointer(opaque, read, write, seek);
	if (!ioPointer)
		return {};
	
	ioPointer->seekable = (seek != nullptr);

	auto result = avformat_alloc_context();
	if (!result)
		return {};

	result->pb = ioPointer.get();
	result->flags |= AVFMT_FLAG_CUSTOM_IO;

	auto options = (AVDictionary*)nullptr;
	const auto guard = gsl::finally([&] { av_dict_free(&options); });

	av_dict_set(&options, "usetoc", "1", 0);
	av_dict_set(&options, "duration", "100ms", 0);

	const auto error = AvErrorWrap(avformat_open_input(
		&result, nullptr, nullptr, &options));

	if (error) 
		return {};
	
	if (seek)
		result->flags |= AVFMT_FLAG_FAST_SEEK;

	ioPointer.release();

	return FormatPointer(result);
}

FormatPointer MakeFormatPointer(const QString& path) {
	auto format = (AVFormatContext*)nullptr;

	format = avformat_alloc_context();
	if (!format)
		return {};

	auto error = AvErrorWrap(avformat_open_input(&format, 
		path.toUtf8(), nullptr, nullptr));

	if (error)
		return {};

	error = avformat_find_stream_info(format, nullptr);
	if (error)
		return {};

	qDebug() << format->bit_rate << format->debug << format->url;

	return FormatPointer(format);
}

FormatPointer MakeFormatPointer(
	IOPointer* ioPointer
) {
	auto format = (AVFormatContext*)nullptr;

	format = avformat_alloc_context();
	if (!format)
		return {};

	format->pb = ioPointer->get();
	format->flags |= AVFMT_FLAG_CUSTOM_IO;

	auto error = avformat_open_input(&format, 0, 0, 0);
	if (error)
		return {};

	error = avformat_find_stream_info(format, nullptr);
	if (error)
		return {};

	ioPointer->release();
	return FormatPointer(format);
}

const AVCodec* FindDecoder(AVCodecContext* context) {
	return (context->codec_id == AV_CODEC_ID_VP9)
		? avcodec_find_decoder_by_name("libvpx-vp9")
		: avcodec_find_decoder(context->codec_id);
}

CodecPointer MakeCodecPointer(CodecDescriptor descriptor) {
// 	measureExecutionTime("MakeCodecPointer")

	auto error = AvErrorWrap();

	auto result = CodecPointer(avcodec_alloc_context3(nullptr));
	const auto context = result.get();
	if (!context)
		return {};

	const auto stream = descriptor.stream;
	error = avcodec_parameters_to_context(context, stream->codecpar);
	if (error)
		return {};

	context->pkt_timebase = stream->time_base;

	const auto codec = FindDecoder(context);
	if (!codec)
		return {};

	context->codec = codec;

	av_opt_set(context, "refcounted_frames", "1", 0);
	av_opt_set(context, "threads", "auto", 0);

	if (codec->capabilities & AV_CODEC_CAP_FRAME_THREADS)
		context->thread_type = FF_THREAD_FRAME;
	else if (codec->capabilities & AV_CODEC_CAP_SLICE_THREADS)
		context->thread_type = FF_THREAD_SLICE;

	if (descriptor.hwAllowed) {
		context->get_format = GetHwFormat;
		context->opaque = context;
	}
	if ((error = avcodec_open2(context, codec, nullptr)))
		return {};

	return result;
}

FramePointer MakeFramePointer() {
	return FramePointer(av_frame_alloc());
}

FramePointer DuplicateFramePointer(AVFrame* frame) {
	return frame
		? FramePointer(av_frame_clone(frame))
		: FramePointer();
}

bool FrameHasData(AVFrame* frame) {
	return (frame && frame->data[0] != nullptr);
}

void ClearFrameMemory(AVFrame* frame) {
	if (FrameHasData(frame))
		av_frame_unref(frame);
}

SwscalePointer MakeSwscalePointer(
	QSize srcSize,
	int srcFormat,
	QSize dstSize,
	int dstFormat,
	SwscalePointer* existing,
	int flags) {
	if (existing && (*existing) != nullptr) {
		const auto& deleter = existing->get_deleter();
		if (deleter.srcSize == srcSize
			&& deleter.srcFormat == srcFormat
			&& deleter.dstSize == dstSize
			&& deleter.dstFormat == dstFormat) {
			return std::move(*existing);
		}
	}
	if (srcFormat <= AV_PIX_FMT_NONE || srcFormat >= AV_PIX_FMT_NB)
		return SwscalePointer();

	const auto result = sws_getCachedContext(
		existing ? existing->release() : nullptr,
		srcSize.width(),
		srcSize.height(),
		AVPixelFormat(srcFormat),
		dstSize.width(),
		dstSize.height(),
		AVPixelFormat(dstFormat),
		flags,
		nullptr,
		nullptr,
		nullptr);

	return SwscalePointer(
		result,
		{ srcSize, srcFormat, dstSize, dstFormat });
}

SwscalePointer MakeSwscalePointer(
	AVFrame* frame,
	QSize resize,
	SwscalePointer* existing,
	int flags) {
	return MakeSwscalePointer(
		QSize(frame->width, frame->height),
		frame->format,
		resize,
		AV_PIX_FMT_BGRA,
		existing);
}


SwresamplePointer MakeSwresamplePointer(
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
	SwresamplePointer* existing
) {
	if (existing && (*existing) != nullptr) {
		const auto& deleter = existing->get_deleter();
		if (true
#if DA_FFMPEG_NEW_CHANNEL_LAYOUT
			&& srcLayout->nb_channels == deleter.srcChannels
			&& dstLayout->nb_channels == deleter.dstChannels
#else // DA_FFMPEG_NEW_CHANNEL_LAYOUT
			&& (av_get_channel_layout_nb_channels(srcLayout)
				== deleter.srcChannels)
			&& (av_get_channel_layout_nb_channels(dstLayout)
				== deleter.dstChannels)
#endif // DA_FFMPEG_NEW_CHANNEL_LAYOUT
			&& srcFormat == deleter.srcFormat
			&& dstFormat == deleter.dstFormat
			&& srcRate == deleter.srcRate
			&& dstRate == deleter.dstRate) {
			return std::move(*existing);
		}
	}

	// Initialize audio resampler
#if DA_FFMPEG_NEW_CHANNEL_LAYOUT
	auto result = (SwrContext*)nullptr;
	auto error = AvErrorWrap(swr_alloc_set_opts2(
		&result,
		dstLayout,
		dstFormat,
		dstRate,
		srcLayout,
		srcFormat,
		srcRate,
		0,
		nullptr));
	if (error || !result) {
		return SwresamplePointer();
	}
#else // DA_FFMPEG_NEW_CHANNEL_LAYOUT
	auto result = swr_alloc_set_opts(
		existing ? existing.get() : nullptr,
		dstLayout,
		dstFormat,
		dstRate,
		srcLayout,
		srcFormat,
		srcRate,
		0,
		nullptr);
#endif // DA_FFMPEG_NEW_CHANNEL_LAYOUT

	error = AvErrorWrap(swr_init(result));
	if (error) {
		swr_free(&result);
		return SwresamplePointer();
	}

	return SwresamplePointer(
		result,
		{
			srcFormat,
			srcRate,
#if DA_FFMPEG_NEW_CHANNEL_LAYOUT
			srcLayout->nb_channels,
#else // DA_FFMPEG_NEW_CHANNEL_LAYOUT
			av_get_channel_layout_nb_channels(srcLayout),
#endif // DA_FFMPEG_NEW_CHANNEL_LAYOUT
			dstFormat,
			dstRate,
#if DA_FFMPEG_NEW_CHANNEL_LAYOUT
			dstLayout->nb_channels,
#else // DA_FFMPEG_NEW_CHANNEL_LAYOUT
			av_get_channel_layout_nb_channels(dstLayout),
#endif // DA_FFMPEG_NEW_CHANNEL_LAYOUT
		});
}

__BASE_MEDIA_FFMPEG_NAMESPACE_END
