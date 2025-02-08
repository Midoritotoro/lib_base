#include "AudioReader.h"

extern "C" {
	#include <libavutil/opt.h>
}

#include <qDebug>

namespace FFmpeg {
	//namespace {
	//	ALuint CreateSource() {
	//		auto source = ALuint(0);
	//		alGenSources(1, &source);

	//		alSourcef(source, AL_PITCH, 1.f);
	//		alSourcef(source, AL_GAIN, 1.f);

	//		alSource3f(source, AL_POSITION, 0, 0, 0);
	//		alSource3f(source, AL_VELOCITY, 0, 0, 0);
	//		
	//		return source;
	//	}

	//	ALuint CreateBuffer() {
	//		auto buffer = ALuint(0);
	//		alGenBuffers(1, &buffer);

	//		return buffer;
	//	}
	//} // namespace

	AudioReader::AudioReader(const QByteArray& data):
		AbstractAudioFFmpegReader(data) 
	{
		
		if (!open(0, 1))
			return;

		auto position = qint64(0);
		int64_t _samplesCount = 0;

		do {
			using Error = ReadError;
			const auto result = readMore();
	
			if (result.errorCode == Error::Retry) {
				if (result.bytes.isNull()) {
					qDebug() << "Retry";
					continue;
				}
			}
			else if (result.errorCode == Error::EndOfFile) {
				qDebug() << "End of file";
				break;
			}
			else if (result.errorCode == Error::Other || result.errorCode == Error::Wait) {
				if (result.bytes.isNull()) {
					qDebug() << "break";
					break;
				}
			}

			const auto sampleBytes = result.bytes;

			_samplesCount += sampleBytes.size() / sampleSize();
			_samples.insert(_samples.end(), sampleBytes.data(), sampleBytes.data() + sampleBytes.size());
		} while (true);

		//qDebug() << "using al...";
		//qDebug() << _samples.data() << _samples.size();
		//qDebug() << "_samplesCount: " << _samplesCount;

		//alGetError();

		//ALuint _alSource = CreateSource();
		//ALuint _alBuffer = CreateBuffer();

		//alGetError();

		//alBufferData(_alBuffer, format(), _samples.data(), _samples.size(), samplesFrequency());
		//alSourcei(_alSource, AL_BUFFER, _alBuffer);

		//alGetError();

		//alSourcei(_alSource, AL_LOOPING, 1);
		//alSourcef(_alSource, AL_GAIN, _volume);
		//alSourcei(_alSource, AL_SAMPLE_OFFSET, static_cast<ALint>(_position));
		//
		//alGetError();

		//ALint state;
		//alGetSourcei(_alSource, AL_SOURCE_STATE, &state);

		//alGetError();

		//if (state != AL_PLAYING)
		//	alSourcePlay(_alSource);

		//alGetError();

		//alGetSourcei(_alSource, AL_SOURCE_STATE, &state);
		//qDebug() << (state == AL_PLAYING);

		//alGetError();
	}

	bool AudioReader::open(Time::time positionMs, float speed) {
		return AbstractAudioReader::open(positionMs)
				&& openCodecContext()
				&& initUsingContext(_codecContext, speed)
				&& seekTo(positionMs);
	}

	bool AudioReader::openCodecContext() {
		_codecContext = avcodec_alloc_context3(nullptr);
		if (!_codecContext) {
			return false;
		}

		const auto stream = fmtContext->streams[streamId];
		AvErrorWrap error = avcodec_parameters_to_context(
			_codecContext,
			stream->codecpar);
		if (error) {

			return false;
		}
		_codecContext->pkt_timebase = stream->time_base;
		av_opt_set_int(_codecContext, "refcounted_frames", 1, 0);

		if (AvErrorWrap error = avcodec_open2(_codecContext, codec, 0)) {
			
			return false;
		}
		return true;
	}

	bool AudioReader::seekTo(Time::time positionMs) {
		const auto stream = fmtContext->streams[streamId];
		const auto timeBase = stream->time_base;

		const auto timeStamp = (positionMs * timeBase.den)
			/ (1000LL * timeBase.num);

		if (av_seek_frame(fmtContext, streamId, timeStamp, AVSEEK_FLAG_BACKWARD) < 0)
			av_seek_frame(fmtContext, streamId, timeStamp, 0);

		avcodec_flush_buffers(_codecContext);

		return true;
	}

	AudioReader::ReadResult AudioReader::readMore() {
		ReadResult result;

		if (_readTillEnd) {
			result.errorCode = ReadError::EndOfFile;
			return result;
		}

		const auto readResult = readFromReadyContext(_codecContext);

		if (readResult.errorCode != ReadError::Wait) {
			if (readResult.errorCode == ReadError::EndOfFile) {
				_readTillEnd = true;
			}
			result.bytes = readResult.bytes;
			return result;
		}

		if (AvErrorWrap error = av_read_frame(fmtContext, &_packet)) {
			if (error.code() != AVERROR_EOF) {
				result.errorCode = ReadError::Other;
				return result;
			}
			error = avcodec_send_packet(_codecContext, nullptr);
			if (!error) {
				result.errorCode = ReadError::Retry;
				return result;
			}
			
			result.errorCode = ReadError::Other;
			return result;
		}

		if (_packet.stream_index == streamId) {
			AvErrorWrap error = avcodec_send_packet(_codecContext, &_packet);
			if (error) {
				av_packet_unref(&_packet);
				result.errorCode = ReadError::Other;
				return result;
			}
		}
		av_packet_unref(&_packet);
		result.errorCode = ReadError::Retry;

		return result;
	}

	AudioReader::~AudioReader() {
		if (_codecContext) {
			avcodec_free_context(&_codecContext);
		}
	}

	Time::time AudioReader::position() const noexcept {
		return 0;
	}

} // namespace FFmpeg
