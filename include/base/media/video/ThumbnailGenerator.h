#pragma once 

#include <QString>
#include "FrameGenerator.h"


namespace FFmpeg {
	class ThumbnailGenerator {
	public:
		[[nodiscard]] static QImage generate(
			const QString& path,
			int swscaleFlags = SWS_BICUBIC);
		[[nodiscard]] static QSize resolution(const QString& path);
	};
} // namespace FFmpeg