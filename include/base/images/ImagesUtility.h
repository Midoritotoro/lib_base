#pragma once 

#include <base/images/Images.h>


namespace base::images::Utility {
#ifdef LIB_BASE_ENABLE_QT 
    int GetChannelsCountByFormat(const QImage& image);
    [[nodiscard]] int GetChannelsCountByBytes(const QImage& image);
#endif
	[[nodiscard]] bool IsJpeg(const char* ext);
	[[nodiscard]] bool IsPng(const char* ext);

	[[nodiscard]] bool IsBmp(const char* ext);
	[[nodiscard]] bool IsFormatSupported(const char* format);

	[[nodiscard]] bool IsFormatsEqual(
		const char* firstFormat,
		const char* secondFormat);

	std::string GetExtensionFromPath(const std::string& path);
	int32 CountBytesPerLine(ImageData* data);
} // namespace base::images