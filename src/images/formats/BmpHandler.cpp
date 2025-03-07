#include <base/images/formats/BmpHandler.h>
#include <base/images/ImagesUtility.h>

#include <base/images/formats/JpegHandler.h>
#include <base/images/formats/PngHandler.h>


__BASE_IMAGES_NAMESPACE_BEGIN

void BmpHandler::write(
	ImageData* data,
	const char* path)
{
	int32 success = 0;
	std::string outputImageFormat = GetExtensionFromPath(path);

	if (IsFormatsEqual(data->handler->format(), outputImageFormat.c_str()) == false) {
		convertToFormat(data, outputImageFormat.c_str());
		return data->handler->write(data, path);
	}

// success - write bmp

	AssertLog(success != 0, "base::images::Image::writeImageToFile: Error while writing");
}

void BmpHandler::read(ImageData* data)
{

}

void BmpHandler::convertToFormat(
	ImageData* data,
	const char* format)
{
	if (IsFormatsEqual(data->handler->format(), format))
		return;

	if (IsPng(format)) {
		data->handler = new PngHandler();
	// convert to Png
	}
		
	else if (IsJpeg(format)) {
		data->handler = new JpegHandler();
		// convert to jpeg
	}

	AssertLog(IsFormatSupported(format),
		std::string("base::images::BmpHandler::convertToFormat: \""
			+ std::string(format) + "\"" + std::string(" is not supported")).c_str());
}

const char* BmpHandler::format() const noexcept {
	return "bmp";
}

__BASE_IMAGES_NAMESPACE_END
