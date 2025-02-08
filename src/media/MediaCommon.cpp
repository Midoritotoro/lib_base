#include "MediaCommon.h"

#include "images/ImagesPrepare.h"
#include "ffmpeg/video/ThumbnailGenerator.h"

#include <QMimeDataBase>
#include <QFile>

#include "../core/CoreUtility.h"
#include "../ui/style/StyleScale.h"


namespace Media {
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
			return Media::Type::Video;
		else if (mimeType.contains("image"))
			return Media::Type::Photo;
		else if (mimeType.contains("audio"))
			return Media::Type::Audio;

		return Media::Type::Unknown;
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
				return FFmpeg::ThumbnailGenerator::resolution(path);
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
						FFmpeg::ThumbnailGenerator::generate(path,
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
			core::utility::GetMinimumSizeWithAspectRatio(
				thumbnailImage.size(),
				targetSize.width()));

		if (core::utility::PartiallyEqual(thumbnailImage.size(),
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