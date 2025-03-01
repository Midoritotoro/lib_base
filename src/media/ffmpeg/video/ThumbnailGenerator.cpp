#include <base/media/ffmpeg/video/ThumbnailGenerator.h>

__BASE_MEDIA_FFMPEG_NAMESPACE_BEGIN

QImage ThumbnailGenerator::generate(
	const QString& path,
	media::Quality quality)
{
	auto generator = video::FrameGenerator(path, media::QualityToSwscaleFlags(quality), false);
	return generator.renderNext(QSize(), Qt::IgnoreAspectRatio, false).image;
}

QSize ThumbnailGenerator::resolution(const QString& path) {
	auto generator = video::FrameGenerator(path, 0, false, false);
	return generator.resolution();
}

__BASE_MEDIA_FFMPEG_NAMESPACE_END
