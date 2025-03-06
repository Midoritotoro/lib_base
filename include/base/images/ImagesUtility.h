#pragma once 

#include <base/images/Images.h>


__BASE_IMAGES_NAMESPACE_BEGIN

#ifdef LIB_BASE_ENABLE_QT 
	NODISCARD int GetChannelsCountByFormat(const QImage& image);
	NODISCARD int GetChannelsCountByBytes(const QImage& image);
#endif

NODISCARD bool IsJpeg(const char* ext);
NODISCARD bool IsPng(const char* ext);

NODISCARD bool IsBmp(const char* ext);
NODISCARD bool IsFormatSupported(const char* format);

NODISCARD bool IsFormatsEqual(
	const char* firstFormat,
	const char* secondFormat);

std::string GetExtensionFromPath(const std::string& path);
int32 CountBytesPerLine(ImageData* data);

__BASE_IMAGES_NAMESPACE_END