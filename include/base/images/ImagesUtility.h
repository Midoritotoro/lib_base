#pragma once 

#include <base/images/Images.h>


__BASE_IMAGES_NAMESPACE_BEGIN

#ifdef LIB_BASE_ENABLE_QT 
	base_nodiscard int GetChannelsCountByFormat(const QImage& image);
	base_nodiscard int GetChannelsCountByBytes(const QImage& image);
#endif

base_nodiscard bool IsJpeg(const char* ext);
base_nodiscard bool IsPng(const char* ext);

base_nodiscard bool IsBmp(const char* ext);
base_nodiscard bool IsFormatSupported(const char* format);

base_nodiscard bool IsFormatsEqual(
	const char* firstFormat,
	const char* secondFormat);

std::string GetExtensionFromPath(const std::string& path);
int32 CountBytesPerLine(ImageData* data);

__BASE_IMAGES_NAMESPACE_END