#include <base/media/ffmpeg/video/ThumbnailGenerator.h>

namespace base::media::ffmpeg::video {
	QImage ThumbnailGenerator::generate(
		const QString& path,
		int swscaleFlags)
	{
		const auto ms = Time::now();
		const auto timer = gsl::finally([=] { qDebug() << "ThumbnailGenerator::generate: " << Time::now() - ms << " ms"; });

		auto generator = FrameGenerator(path, swscaleFlags, false);
		return generator.renderNext(QSize(), Qt::IgnoreAspectRatio, false).image;
	}

	QSize ThumbnailGenerator::resolution(const QString& path) {
		const auto ms = Time::now();
		const auto timer = gsl::finally([=] { qDebug() << "ThumbnailGenerator::resolution: " << Time::now() - ms << " ms"; });

		auto generator = FrameGenerator(path, 0, false, false);
		return generator.resolution();
	}
} // namespace base::media::ffmpeg::video