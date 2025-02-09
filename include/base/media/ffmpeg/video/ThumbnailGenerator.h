#pragma once 

#include <QString>
#include <base/media/ffmpeg/video/FrameGenerator.h>


namespace base::media::ffmpeg::video {
	class ThumbnailGenerator {
	public:
		[[nodiscard]] static QImage generate(
			const QString& path,
			int swscaleFlags = SWS_BICUBIC);
		[[nodiscard]] static QSize resolution(const QString& path);
	};
} // namespace base::media::ffmpeg::video