#include <base/media/ffmpeg/video/FrameGenerator.h>

#include <base/concurrent/Concurrent.h>
#include <base/utility/Algorithm.h>

#include <functional>
#include <QApplication>
#include <QScreen>

#include <base/images/ImagesBuffer.h>
#include <qDebug>

#include <base/qt/common/Size.h>
#include <QFile>


namespace base::media::ffmpeg::video {
	namespace {
		constexpr auto kMaxArea = 1920 * 1080 * 4;

		[[nodiscard]] QByteArray ReadFile(const QString& filepath) {
			auto file = QFile(filepath);
			return file.open(QIODevice::ReadOnly)
				? file.readAll()
				: QByteArray();
		}

		[[nodiscard]] QByteArray ReadFile(const QString& filepath, int bytes) {
			auto file = QFile(filepath);
			return file.open(QIODevice::ReadOnly)
				? file.read(bytes)
				: QByteArray();
		}
	} // namespace

FrameGenerator::FrameGenerator(
	const QByteArray& bytes,
	int swscaleFlags,
	bool findStreamInfo,
	bool createCodec
):
	_bytes(bytes)
	, _swscaleFlags(swscaleFlags)
{
//	measureExecutionTime("FrameGenerator::FrameGenerator")

	if (_bytes.isEmpty())
		return;

	_format = MakeFormatPointer(
		static_cast<void*>(this),
		&FrameGenerator::Read,
		nullptr,
		&FrameGenerator::Seek);

	if (findStreamInfo) {
		const auto error = AvErrorWrap(avformat_find_stream_info(_format.get(), nullptr));
		if (error)
			std::cout << error.text() << '\n';
	}

	_bestVideoStreamId = av_find_best_stream(
		_format.get(), AVMEDIA_TYPE_VIDEO,
		-1, -1, nullptr, 0);

	if (_bestVideoStreamId < 0)
		return;

	if (createCodec)
		_codec = MakeCodecPointer({ .stream = _format->streams[_bestVideoStreamId] });
}

FrameGenerator::FrameGenerator(
	const QString& path,
	int swscaleFlags,
	bool findStreamInfo,
	bool createCodec
):
	FrameGenerator(ReadFile(path), swscaleFlags,
		findStreamInfo, createCodec)
{}

FrameGenerator::Frame FrameGenerator::renderCurrent(
	QSize size,
	Qt::AspectRatioMode mode,
	bool fullScreen)
{
	//measureExecutionTime("FrameGenerator::renderCurrent")
	const auto frame = _current.frame.get();

	const auto width = frame->width;
	const auto height = frame->height;

	if (!width || !height)
		return {};

	auto scaledFrameSize = QSize(width, height);
	auto requiredSize = !size.isEmpty() ? size : scaledFrameSize;

	if (fullScreen)
		scaledFrameSize = requiredSize = recountMaximumFrameSize(requiredSize);

	auto storage = images::CreateFrameStorage(requiredSize);

	const auto dx = (requiredSize.width() - scaledFrameSize.width()) / 2;
	const auto dy = (requiredSize.height() - scaledFrameSize.height()) / 2;

	const auto srcFormat = (frame->format == AV_PIX_FMT_NONE)
		? _codec->pix_fmt
		: frame->format;
	const auto srcSize = QSize(frame->width, frame->height);

	const auto dstFormat = AV_PIX_FMT_BGRA;
	const auto dstSize = scaledFrameSize;

	const auto bgra = (srcFormat == AV_PIX_FMT_BGRA);
	const auto withAlpha = bgra || (srcFormat == AV_PIX_FMT_YUVA420P);

	const auto dstPerLine = storage.bytesPerLine();
	auto dst = storage.bits() + dx * sizeof(qint32) + dy * dstPerLine;

	if (srcSize == dstSize && bgra) {
		const auto srcPerLine = frame->linesize[0];
		const auto perLine = std::min(srcPerLine, int(dstPerLine));

		auto src = frame->data[0];

		for (auto y = 0, height = srcSize.height(); y != height; ++y) {
			memcpy(dst, src, perLine);

			src += srcPerLine;
			dst += dstPerLine;
		}
	}
	else {
		_scale = MakeSwscalePointer(
			srcSize,
			srcFormat,
			dstSize,
			dstFormat,
			&_scale,
			_swscaleFlags);

		uint8_t* dstData[AV_NUM_DATA_POINTERS] = { dst };
		int dstLinesize[AV_NUM_DATA_POINTERS] = { int(dstPerLine) };

		
		sws_scale(_scale.get(),
			frame->data,
			frame->linesize,
			0,
			frame->height,
			dstData,
			dstLinesize);
	}

	
	if (dx && requiredSize.height() > 0) {
		auto dst = storage.bits();
		const auto line = scaledFrameSize.width() * sizeof(qint32);

		memset(dst, 0, dx * sizeof(qint32));
		dst += dx * sizeof(qint32);

		for (auto y = 0; y != requiredSize.height() - 1; ++y) {
			memset(dst + line, 0, (dstPerLine - line));
			dst += dstPerLine;
		}

		dst += line;
		memset(dst, 0, (requiredSize.width() - scaledFrameSize.width() - dx) * sizeof(qint32));
	}
	else if (dy && requiredSize.width() > 0) {
		const auto dst = storage.bits();
		memset(dst, 0, dstPerLine * dy);
		memset(
			dst + dstPerLine * (dy + scaledFrameSize.height()),
			0,
			dstPerLine * (requiredSize.height() - scaledFrameSize.height() - dy));
	}
	if (withAlpha)
		images::PremultiplyInplace(storage);

	const auto duration = _next.frame
		? (_next.position - _current.position)
		: _current.duration;

	ClearFrameMemory(frame);

	return {
		.duration = duration,
		.image = storage,
		.last = !_next.frame 
	};
}

FrameGenerator::Frame FrameGenerator::renderNext(
	QSize size,
	Qt::AspectRatioMode mode,
	bool fullScreen) 
{
	if (!_codec)
		return {};
	else if (!_current.frame)
		readNextFrame();

	std::swap(_current, _next);
	if (!_current.frame)
		return {};

	readNextFrame();
	return renderCurrent(size, mode, fullScreen);
}

void FrameGenerator::setSpeed(float speed) {
	if (speed < 0.5 || speed > 2.5)
		return;

	_speed = speed;
}

void FrameGenerator::rewind(Time::time_t positionMs) {
//	measureExecutionTime("FrameGenerator::rewind")

	if (_codec == nullptr)
		return;

	const auto stream = _format->streams[_bestVideoStreamId];
	const auto timeBase = stream->time_base;

	const auto timeStamp = (positionMs * timeBase.den)
		/ (1000LL * timeBase.num);

	if (av_seek_frame(_format.get(), _bestVideoStreamId, timeStamp, AVSEEK_FLAG_BACKWARD) < 0)
		av_seek_frame(_format.get(), _bestVideoStreamId, timeStamp, 0);

	avcodec_flush_buffers(_codec.get());

	_current = ReadFrame();
	_next = ReadFrame();

	_currentFrameDelay = _nextFrameDelay = 0;
	_framePosition = positionMs;

	readNextFrame();
}

QSize FrameGenerator::resolution() const {
	if (_format == nullptr)
		return QSize();

	return QSize(
		_format->streams[_bestVideoStreamId]->codecpar->width,
		_format->streams[_bestVideoStreamId]->codecpar->height);
}

Time::time_t FrameGenerator::duration() const noexcept {
	if (_format == nullptr)
		return 0;

	const auto rebase = [](int64_t duration, const AVRational& base) {
		return (duration * 1000LL * base.num) / base.den;
	};

	const auto stream = _format->streams[_bestVideoStreamId];

	if (stream->duration != AV_NOPTS_VALUE)
		return rebase(stream->duration, stream->time_base);
	else if (_format->duration != AV_NOPTS_VALUE)
		return rebase(_format->duration, AVRational{ 1, AV_TIME_BASE });

	return 0;
}

Time::time_t FrameGenerator::position() const noexcept {
	return _framePosition;
}

Time::time_t FrameGenerator::frameDelay() const noexcept {
	return _currentFrameDelay;
}

int FrameGenerator::Read(void* opaque, 
	uint8_t* buffer,
	int bufferSize) 
{
	return static_cast<FrameGenerator*>(opaque)->read(buffer, bufferSize);
}

int64_t FrameGenerator::Seek(void* opaque,
	int64_t offset, 
	int whence) 
{
	return static_cast<FrameGenerator*>(opaque)->seek(offset, whence);
}

int FrameGenerator::read(
	uint8_t* buffer,
	int bufferSize) 
{
	const auto availableBytes = _bytes.size() - _deviceOffset;
	if (availableBytes <= 0)
		return AVERROR_EOF;

	const auto fill = std::min(int(availableBytes), bufferSize);
	memcpy(buffer, _bytes.data() + _deviceOffset, fill);

	_deviceOffset += fill;
	return fill;
}

int64_t FrameGenerator::seek(
	int64_t offset,
	int whence) 
{
	const auto size = _bytes.size();

	if (whence == AVSEEK_SIZE)
		return size;

	const auto now = [&]{
		switch (whence) {
			case SEEK_SET: return offset;
			case SEEK_CUR: return _deviceOffset + offset;
			case SEEK_END: return int64_t(size) + offset;
		}
		return int64_t(-1);
	}();

	if (now < 0 || now > size)
		return -1;

	_deviceOffset = now;
	return now;
}

QSize FrameGenerator::recountMaximumFrameSize(const QSize& targetSize) {
	const auto width = targetSize.width();
	const auto height = targetSize.height();

	const auto screenSize = base::qt::common::ScreenResolution();
	double scale = qMin(static_cast<double>(screenSize.width()) / width,
		static_cast<double>(screenSize.height()) / height);

	if (width * scale > (screenSize.width() * 0.7)) {
		const auto maximumWidthScale = (screenSize.width() * 0.7) / width;
		scale = qMin(scale, maximumWidthScale);
	}

	return QSize(width * scale, height * scale);
}


void FrameGenerator::readNextFrame() {
//	measureExecutionTime("FrameGenerator::readNextFrame")
	//	const auto milliseconds = Time::now();
	//	const auto time = gsl::finally([&milliseconds] { qDebug() << "readNextFrame: " << Time::now() - milliseconds << " ms";  });

	auto frame = _next.frame 
		? std::exchange(_next.frame, {}) 
		: MakeFramePointer();


	while (true) {
		auto ms = Time::now();
		auto result = avcodec_receive_frame(_codec.get(), frame.get());

	//	qDebug() << "avcodec_receive_frame(_codec.get(), frame.get()): " << Time::now() - ms << " ms";

		if (result >= 0) {
			if (frame->width * frame->height > kMaxArea)
				return;

			_next.frame = std::move(frame);
			resolveNextFrameTiming();
			return;
		}

		if (result == AVERROR_EOF)
			return;
		else if (result != AVERROR(EAGAIN))
			return;
		
		auto packet = Packet();
		auto finished = false;

		ms = Time::now();

		do {
			const auto result = av_read_frame(
				_format.get(),
				&packet.fields());

			if (result == AVERROR_EOF) {
				finished = true;
				break;
			}
			else if (result < 0)
				return;
		} while (packet.fields().stream_index != _bestVideoStreamId);

	//	qDebug() << "read frame while: " << Time::now() - ms << " ms";
		ms = Time::now();

		if (finished)
			result = avcodec_send_packet(_codec.get(), nullptr);
		else {
			const auto native = &packet.fields();
			const auto guard = gsl::finally([
				&,
					size = native->size,
					data = native->data
			] {
					native->size = size;
					native->data = data;
					packet = Packet();
				});
			result = avcodec_send_packet(_codec.get(), native);
		}
		//qDebug() << " avcodec_send_packet(_codec.get(), native): " << Time::now() - ms << " ms";
		if (result < 0) 
			return;
		}
}

void FrameGenerator::resolveNextFrameTiming() {
	//measureExecutionTime("FrameGenerator::resolveNextFrameTiming")

	const auto base = _format->streams[_bestVideoStreamId]->time_base;

#if DA_FFMPEG_HAVE_DURATION
	const auto duration = _next.frame->duration;
#else
	const auto duration = _next.frame->pkt_duration;
#endif
	const auto framePts = _next.frame->pts;
	auto framePosition = (framePts * 1000LL * base.num) / base.den;

	_currentFrameDelay = _nextFrameDelay / _speed;

	if (_framePosition + _currentFrameDelay < framePosition)
		_currentFrameDelay = qint32(framePosition - _framePosition) / _speed;
	else if (framePosition < _framePosition + _currentFrameDelay)
		framePosition = _framePosition + _currentFrameDelay;

	if (duration == AV_NOPTS_VALUE)
		_nextFrameDelay = 0;
	else
		_nextFrameDelay = (duration * 1000LL * base.num) / base.den;

	_framePosition = framePosition;

	_next.position = _framePosition;
	_next.duration = _nextFrameDelay;

	++_frameIndex;
}

} //namespace base::media::ffmpeg::video