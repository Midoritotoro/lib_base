#pragma once

#include "AbstractAudioFFmpegReader.h"

namespace FFmpeg {

	class AudioReader : public AbstractAudioFFmpegReader {
	public:
		AudioReader(const QByteArray& data);

		bool open(Time::time positionMs, float speed = 1.) override;

		[[nodiscard]] Time::time position() const noexcept;

		~AudioReader();

	private:
		ReadResult readMore() override;

		bool openCodecContext();
		bool seekTo(Time::time positionMs);

		std::vector<uchar> _samples;

		AVCodecContext* _codecContext = nullptr;
		AVPacket _packet;
		bool _readTillEnd = false;

		Time::time _position = 0;
		float _volume = 100;
	};

} // namespace FFmpeg
