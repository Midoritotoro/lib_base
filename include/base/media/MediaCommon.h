#pragma once 

#include <base/Types.h>
#include <QString>

#include <QSize>
#include <QPixmap>


namespace base::media {
	enum class Type : uchar {
		Photo = 0x01,
		Video = 0x02,
		Audio = 0x04,
		Unknown = 0x08
	};

	enum class Quality : uchar {
		Low = 0x01,
		Medium = 0x02,
		High = 0x04,
		Ultra = 0x08,
	};

	[[nodiscard]] Type detectMediaType(const QString& path);
	
	[[nodiscard]] QPixmap FindPreviewInCache(const QString& key);
	[[nodiscard]] QSize MediaResolution(
		const QString& path,
		Type type = Type::Unknown);

	[[nodiscard]] QPixmap MediaPreview(
		const QString& path,
		Quality quality = Quality::Medium);
	[[nodiscard]] QPixmap GenerateThumbnail(
		const QString& path,
		const QSize& targetSize,
		Quality quality = Quality::Medium);
} // namespace Media