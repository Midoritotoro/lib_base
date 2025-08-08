#include <base/images/ImagesUtility.h>
#include <base/core/utility/Algorithm.h>

#include <base/core/utility/OverflowCheck.h>
#include <iostream>
#include <regex>


__BASE_IMAGES_NAMESPACE_BEGIN

#ifdef LIB_BASE_ENABLE_QT
	int GetChannelsCountByFormat(const QImage& image) {
		auto format = image.format();

		switch (format) {
			case QImage::Format_Mono:
			case QImage::Format_MonoLSB:
				return 1;

			case QImage::Format_Grayscale8:
				return 1;

			case QImage::Format_Indexed8:
				return 1;

			case QImage::Format_RGB32:
			case QImage::Format_ARGB32:
			case QImage::Format_ARGB32_Premultiplied:
			case QImage::Format_RGBA8888:
			case QImage::Format_RGBA8888_Premultiplied:

			case QImage::Format_RGB888:
			case QImage::Format_BGR888:
				return 3;
		}

		return -1;
	}

	base_nodiscard int GetChannelsCountByBytes(const QImage& image) {
		const auto width = image.width();
		const auto bytesPerLine = image.bytesPerLine();

		if (width <= 0 || bytesPerLine <= 0)
			return -1;

		const auto bytesPerPixel = bytesPerLine / width;

		switch (bytesPerPixel) {
			case 1:  return 1; // Grayscale, Mono
			case 3:  return 3; // RGB
			case 4: return 4;  // RGBA
		}

		return -1;
	}
#endif

bool IsJpeg(const char* ext) {
	return strcmp(ext, "jpg") == 0 || strcmp(ext, "jpeg") == 0
		|| strcmp(ext, "jpe") == 0;
}

bool IsPng(const char* ext) {
	return (strcmp(ext, "png") == 0);
}

bool IsBmp(const char* ext) {
	return (strcmp(ext, "bmp") == 0);
}

bool IsFormatSupported(const char* format) {
	return IsJpeg(format) || IsBmp(format) || IsPng(format);
}

bool IsFormatsEqual(
	const char* firstFormat,
	const char* secondFormat)
{
	if (IsJpeg(firstFormat) && IsJpeg(secondFormat))
		return true;

	return (strcmp(firstFormat, secondFormat) == 0);
}


std::string GetExtensionFromPath(const std::string& path) {
	std::regex regex(R"(\.(bmp|jpe?g|png)(?=[^.]*$))", std::regex::icase);
	std::smatch match;

	if (std::regex_search(path, match, regex)) {
		std::string result = match[1].str().erase(0, 0);
		return result;
	}

	size_t pos = path.rfind('.');
	if (pos != std::string::npos)
		return path.substr(pos);

	return "";
}

int32 CountBytesPerLine(ImageData* data)
{
	//qDebug() << "base::images::Image::recountBytesPerLine: depth - " << data->depth;
	//int32 invalid = -1;

	//if (data->height <= 0)
	//	return invalid;

	//auto _bytesPerLine = int32(0);

	//if (MultiplyOverflow(data->width, data->depth, &_bytesPerLine))
	//	return invalid;
	//if (AdditionOverflow(_bytesPerLine, 31, &_bytesPerLine))
	//	return invalid;

	//_bytesPerLine = (_bytesPerLine >> 5) << 2;    // can't overflow

	//auto dummy = sizetype(0);
	//if (MultiplyOverflow(data->height, sizetype(sizeof(uchar*)), &dummy))
	//	return invalid;

	//if (data->width > (INT_MAX - 31) / data->depth)
	//	return invalid;

	//return _bytesPerLine;
	return 0;
}

__BASE_IMAGES_NAMESPACE_END