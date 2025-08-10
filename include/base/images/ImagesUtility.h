#pragma once 

#include <base/images/Images.h>


__BASE_IMAGES_NAMESPACE_BEGIN

#ifdef LIB_BASE_ENABLE_QT 
	  int GetChannelsCountByFormat(const QImage& image);
	  int GetChannelsCountByBytes(const QImage& image);
#endif

  bool IsJpeg(const char* ext);
  bool IsPng(const char* ext);

  bool IsBmp(const char* ext);
  bool IsFormatSupported(const char* format);

  bool IsFormatsEqual(
	const char* firstFormat,
	const char* secondFormat);

std::string GetExtensionFromPath(const std::string& path);
int32 CountBytesPerLine(ImageData* data);

__BASE_IMAGES_NAMESPACE_END