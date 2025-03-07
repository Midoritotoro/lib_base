#include <base/images/formats/PngHandler.h>
#include <base/images/ImagesUtility.h>

#include <base/images/formats/BmpHandler.h>
#include <base/images/formats/JpegHandler.h>

#include <png.h>


__BASE_IMAGES_NAMESPACE_BEGIN

void PngHandler::write(
	ImageData* data,
	const char* path)
{
	int32 success = 0;
	std::string outputImageFormat = GetExtensionFromPath(path);

	if (IsFormatsEqual(data->handler->format(), outputImageFormat.c_str()) == false) {
		convertToFormat(data, outputImageFormat.c_str());
		return data->handler->write(data, path);
	}

	// success = write png

	AssertLog(success != 0, "base::images::Image::writeImageToFile: Error while writing");
}

void PngHandler::read(ImageData* data)
{
    


	//qDebug() << "width: " << data->width << "height: " << data->height;
}

void PngHandler::convertToFormat(
	ImageData* data,
	const char* format)
{
	if (IsFormatsEqual(data->handler->format(), format))
		return;

	if (IsJpeg(format)) {
		data->handler = new JpegHandler();
	// convert to jpg
	}

	else if (IsBmp(format)) {
		data->handler = new BmpHandler();
		// convert to bmp
	}

	AssertLog(IsFormatSupported(format),
		std::string("base::images::PngHandler::convertToFormat: \""
			+ std::string(format) + "\"" + std::string(" is not supported")).c_str());
}

const char* PngHandler::format() const noexcept {
	return "png";
}

__BASE_IMAGES_NAMESPACE_END
