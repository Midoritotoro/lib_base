#include <base/media/ffmpeg/audio/AbstractAudioReader.h>

namespace FFmpeg {

#if !DA_FFMPEG_NEW_CHANNEL_LAYOUT
	uint64_t AbstractFFMpegLoader::ComputeChannelLayout(
		uint64_t channel_layout,
		int channels) {
		if (channel_layout) {
			if (av_get_channel_layout_nb_channels(channel_layout) == channels) {
				return channel_layout;
			}
		}
		return av_get_default_channel_layout(channels);
	}
#endif // !DA_FFMPEG_NEW_CHANNEL_LAYOUT

	int64_t AbstractAudioReader::Mul(int64_t value, AVRational rational) {
		return value * rational.num / rational.den;
	}

	bool AbstractAudioReader::open(Time::time positionMs, float speed) {
		ioBuffer = (uchar*)av_malloc(kAvioBlockSize);

		if (_data.isEmpty())
			return false;
		
		fmtContext->pb = ioContext.get();

		if (AvErrorWrap error = avformat_open_input(&fmtContext, 0, 0, 0)) {
			ioBuffer = nullptr;

			return false;
		}
		_opened = true;

		if (AvErrorWrap error = avformat_find_stream_info(fmtContext, 0)) {

			return false;
		}

		streamId = av_find_best_stream(fmtContext, AVMEDIA_TYPE_AUDIO, -1, -1, &codec, 0);
		if (streamId < 0) {

			return false;
		}

		const auto stream = fmtContext->streams[streamId];
		const auto params = stream->codecpar;
		_samplesFrequency = params->sample_rate;
		if (stream->duration != AV_NOPTS_VALUE) {
			_duration = Mul(stream->duration * 1000, stream->time_base);
		}
		else {
			_duration = Mul(fmtContext->duration * 1000, { 1, AV_TIME_BASE });
		}
		_startedAtSample = (positionMs * _samplesFrequency) / 1000LL;

		return true;
	}

	AbstractAudioReader::~AbstractAudioReader() {
		if (_opened) {
			avformat_close_input(&fmtContext);
		}
		if (ioContext) {
			av_freep(&ioContext->buffer);
			av_freep(&ioContext);
		}
		else if (ioBuffer) {
			av_freep(&ioBuffer);
		}
		if (fmtContext) avformat_free_context(fmtContext);
	}

	int AbstractAudioReader::Read(void* opaque, uint8_t* buf, int buf_size) {
		auto l = reinterpret_cast<AbstractAudioReader*>(opaque);

		auto nbytes = qMin(l->_data.size() - l->_dataPos, (buf_size));
		if (nbytes <= 0) {
			return AVERROR_EOF;
		}

		memcpy(buf, l->_data.constData() + l->_dataPos, nbytes);
		l->_dataPos += nbytes;
		return nbytes;
	}

	int64_t AbstractAudioReader::Seek(void* opaque, int64_t offset, int whence) {
		auto l = reinterpret_cast<AbstractAudioReader*>(opaque);

		int32_t newPos = -1;
		switch (whence) {
		case SEEK_SET: newPos = offset; break;
		case SEEK_CUR: newPos = l->_dataPos + offset; break;
		case SEEK_END: newPos = l->_data.size() + offset; break;
		case AVSEEK_SIZE: {

			return l->_data.size();
		} break;
		}
		if (newPos < 0 || newPos > l->_data.size()) {
			return -1;
		}
		l->_dataPos = newPos;
		return l->_dataPos;
	}

} // namespace FFmpeg