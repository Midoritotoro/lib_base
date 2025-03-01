#pragma once 

#include <QString>
#include <base/media/ffmpeg/video/FrameGenerator.h>
#include <base/media/MediaCommon.h>

__BASE_MEDIA_FFMPEG_NAMESPACE_BEGIN

class ThumbnailGenerator {
public:
	[[nodiscard]] static QImage generate(
		const QString& path,
		media::Quality quality = media::Quality::Medium);
	[[nodiscard]] static QSize resolution(const QString& path);
};

__BASE_MEDIA_FFMPEG_NAMESPACE_END
