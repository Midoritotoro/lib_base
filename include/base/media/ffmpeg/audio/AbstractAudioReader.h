#pragma once

#include <base/media/ffmpeg/audio/AbstractAudioLoader.h>

namespace base::media::ffmpeg::audio {

	class AbstractAudioReader : public AbstractAudioLoader {
	public:
		AbstractAudioReader(const QByteArray& data)
			: AbstractAudioLoader(data) {
				fmtContext = MakeFormatPointer(
					static_cast<void*>(this),
					&AbstractAudioReader::Read,
					nullptr,
					&AbstractAudioReader::Seek).release();

				ioContext = MakeIOPointer(
					static_cast<void*>(this),
					&AbstractAudioReader::Read,
					nullptr,
					&AbstractAudioReader::Seek);
		}

		bool open(Time::time_t positionMs, float speed = 1.) override;

		Time::time_t duration() override {
			return _duration;
		}
		void overrideDuration(int64_t startedAtSample, Time::time_t duration) {
			_startedAtSample = startedAtSample;
			_duration = duration;
		}

		int samplesFrequency() override {
			return _samplesFrequency;
		}

#if !DA_FFMPEG_NEW_CHANNEL_LAYOUT
		static uint64_t ComputeChannelLayout(
			uint64_t channel_layout,
			int channels);
#endif // !DA_FFMPEG_NEW_CHANNEL_LAYOUT

		  int64_t startedAtSample() const {
			return _startedAtSample;
		}

		~AbstractAudioReader();

	protected:
		static int64_t Mul(int64_t value, AVRational rational);

		int _samplesFrequency = 48000;
		int64_t _startedAtSample = 0;
		Time::time_t _duration = 0;

		uchar* ioBuffer = nullptr;
		IOPointer ioContext = nullptr;
		AVFormatContext* fmtContext = nullptr;
#if LIBAVFORMAT_VERSION_MAJOR >= 59
		const AVCodec* codec = nullptr;
#else
		AVCodec* codec = nullptr;
#endif
		int streamId = 0;

		bool _opened = false;
		int newPos = 0;
	private:
		static int Read(void* opaque, uint8_t* buf, int buf_size);
		static int64_t Seek(void* opaque, int64_t offset, int whence);
	};

} // namespace base::media::ffmpeg::audio