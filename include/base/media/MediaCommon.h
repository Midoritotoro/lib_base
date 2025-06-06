#pragma once 

#include <base/core/Types.h>
#include <base/core/arch/KeywordSupport.h>

#include <QString>

#include <QSize>
#include <QPixmap>


__BASE_MEDIA_NAMESPACE_BEGIN

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

NODISCARD int QualityToSwscaleFlags(Quality quality);
NODISCARD Type detectMediaType(const QString& path);
	
NODISCARD QPixmap FindPreviewInCache(const QString& key);
NODISCARD QSize MediaResolution(
	const QString&	path,
	Type			type = Type::Unknown);

NODISCARD QPixmap MediaPreview(
	const QString&	path,
	Quality			quality = Quality::Medium);
NODISCARD QPixmap GenerateThumbnail(
	const QString&	path,
	const QSize&	targetSize,
	Quality			quality = Quality::Medium);

__BASE_MEDIA_NAMESPACE_END
