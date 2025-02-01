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

	void StbiWrite(
		void* context,
		void* data,
		int size);

	void ReadImage(
		ImageData* data,
		int32 forceChannelsCount = FORCE_IMAGE_CHANNELS);

	[[nodiscard]] int32 CountBytesPerLine(ImageData* data);
	std::string GetExtensionFromPath(const std::string& path);

	void ConvertToFormat(
		ImageData* src, 
		const char* format);

	void WriteImageToFile(
		ImageData* data,
		const std::string& path);
} // namespace base::images