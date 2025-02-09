#include <base/media/MediaCommon.h>

#include <base/images/ImagesPrepare.h>
#include <base/media/ffmpeg/video/ThumbnailGenerator.h>

#include <QMimeDataBase>
#include <QFile>

#include <base/Utility.h>
#include <base/qt/style/StyleScale.h>

#include <QPixmapCache>
#include <base/qt/common/Size.h>


namespace base::media {
	namespace {
		inline constexpr auto kPreviewPrefix = "_p";

		[[nodiscard]] int QualityToSwscaleFlags(Quality quality) {
			switch (quality) {
				case Quality::Low:
					return SWS_POINT;

				case Quality::Medium:
					return SWS_BICUBIC;

				case Quality::High:
					return SWS_BILINEAR;

				case Quality::Ultra:
					return SWS_SINC;

				default: 
					return SWS_BICUBIC;	
			}
		}
	} // namespace 

	Type detectMediaType(const QString& filePath) {
		const auto mimeType = QMimeDatabase().mimeTypeForFile(filePath).name();

		if (mimeType.contains("video"))
			return Type::Video;
		else if (mimeType.contains("image"))
			return Type::Photo;
		else if (mimeType.contains("audio"))
			return Type::Audio;

		return Type::Unknown;
	}

	QPixmap FindPreviewInCache(const QString& key) {
		auto temp = QPixmap();
		QPixmapCache::find(key, &temp);

		return temp;
	}

	QSize MediaResolution(
		const QString& path,
		Type type)
	{
		const auto ms = Time::now();
		const auto timer = gsl::finally([=] { qDebug() << "MediaResolution: " << Time::now() - ms << " ms"; });

		const auto mediaType = type == Type::Unknown
			? detectMediaType(path)
			: type;

		switch (mediaType) {
			case Type::Photo:
				return QPixmap(path).size();

			case Type::Video:
				return ffmpeg::video::ThumbnailGenerator::resolution(path);
		}

		return QSize();
	}

	QPixmap MediaPreview(
		const QString& path,
		Quality quality)
	{
		if (const auto _preview = FindPreviewInCache(kPreviewPrefix + path);
			_preview.isNull() == false)
			return _preview;

		auto preview = QPixmap();

		switch (detectMediaType(path)) {
			case Type::Photo:
				preview = QPixmap(path);
				break;

			case Type::Video:
				preview = images::PixmapFast(
					std::move(
						ffmpeg::video::ThumbnailGenerator::generate(path,
							QualityToSwscaleFlags(quality))));
				break;

			case Type::Audio:
				return QPixmap();

			case Type::Unknown:
				return QPixmap();
		}

		if (QPixmapCache::cacheLimit() > 0)
			QPixmapCache::insert(kPreviewPrefix + path, preview);

		return preview;
	}

	QPixmap GenerateThumbnail(
		const QString& path,
		const QSize& targetSize,
		Quality quality)
	{
		if (targetSize.isNull())
			return QPixmap();

		auto thumbnail = QPixmap();

		if (QPixmapCache::find(path, &thumbnail))
			return thumbnail;
	
		auto thumbnailImage = MediaPreview(path).toImage();
		if (thumbnailImage.isNull())
			return QPixmap();

		thumbnailImage = images::Prepare(
			std::move(thumbnailImage),
			qt::common::GetMinimumSizeWithAspectRatio(
				thumbnailImage.size(),
				targetSize.width()));

		if (qt::common::PartiallyEqual(thumbnailImage.size(),
			targetSize, 1) == false)
			return QPixmap();

		thumbnailImage = images::Opaque(std::move(thumbnailImage));
		thumbnail = images::PixmapFast(std::move(thumbnailImage));

		if (QPixmapCache::cacheLimit() > 0) {
			auto mediaPreview = QPixmap();

			if (QPixmapCache::find(kPreviewPrefix + path, &mediaPreview))
				QPixmapCache::remove(kPreviewPrefix + path);

			QPixmapCache::insert(path, thumbnail);
		}

		return thumbnail;
	}
} // namespace Media