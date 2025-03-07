#include <base/images/formats/JpegHandler.h>
#include <base/images/ImagesUtility.h>


#include <base/images/formats/BmpHandler.h>
#include <base/images/formats/PngHandler.h>

#include <turbojpeg.h>


__BASE_IMAGES_NAMESPACE_BEGIN

void JpegHandler::write(
	ImageData* data,
	const char* path)
{
	int32 success = 0;
	std::string outputImageFormat = GetExtensionFromPath(path);

	if (IsFormatsEqual(data->handler->format(), outputImageFormat.c_str()) == false) {
		convertToFormat(data, outputImageFormat.c_str());
		return data->handler->write(data, path);
	}

// success = write jpeg

	AssertLog(success != 0, "base::images::JpegHandler::write: Error while writing");
}


void JpegHandler::read(ImageData* data)
{
		
}

void JpegHandler::convertToFormat(
	ImageData* data,
	const char* format)
{
	if (IsFormatsEqual(data->handler->format(), format))
		return;

	if (IsPng(format)) {
		data->handler = new PngHandler();
	// convert to png
	}

	else if (IsBmp(format)) {
		data->handler = new BmpHandler();
		// convert to bmp
	}

	AssertLog(IsFormatSupported(format),
		std::string("base::images::PngHandler::convertToFormat: \""
			+ std::string(format) + "\"" + std::string(" is not supported")).c_str());
}

const char* JpegHandler::format() const noexcept {
	return "jpeg";
}

__BASE_IMAGES_NAMESPACE_END