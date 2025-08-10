#pragma once

#include <base/media/ffmpeg/audio/AbstractAudioFFmpegReader.h>

namespace base::media::ffmpeg::audio {

	class AudioReader : public AbstractAudioFFmpegReader {
	public:
		AudioReader(const QByteArray& data);

		bool open(Time::time_t positionMs, float speed = 1.) override;

		  Time::time_t position() const noexcept;

		~AudioReader();

	private:
		ReadResult readMore() override;

		bool openCodecContext();
		bool seekTo(Time::time_t positionMs);

		std::vector<uchar> _samples;

		AVCodecContext* _codecContext = nullptr;
		AVPacket _packet;
		bool _readTillEnd = false;

		Time::time_t _position = 0;
		float _volume = 100;
	};

} // namespace base::media::ffmpeg::audio
