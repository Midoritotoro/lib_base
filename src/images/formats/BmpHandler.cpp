#include <base/images/formats/BmpHandler.h>
#include <base/images/ImagesUtility.h>

#include <base/images/formats/JpegHandler.h>
#include <base/images/formats/PngHandler.h>


namespace base::images {
	void BmpHandler::write(
		ImageData* data,
		const char* path)
	{
		int32 success = 0;
		std::string outputImageFormat = Utility::GetExtensionFromPath(path);

		if (Utility::IsFormatsEqual(data->imageExtension, outputImageFormat.c_str()) == false) {
			convertToFormat(data, outputImageFormat.c_str());
			return data->handler->write(data, path);
		}

	// success - write bmp

		AssertLog(success != 0, "base::images::Image::writeImageToFile: Error while writing");
	}

	void BmpHandler::convertToFormat(
		ImageData* data,
		const char* format)
	{
		if (Utility::IsFormatsEqual(data->imageExtension, format))
			return;

		data->imageExtension = format;

		if (Utility::IsPng(format)) {
			data->handler = new PngHandler();
		// convert to Png
		}
		
		else if (Utility::IsJpeg(format)) {
			data->handler = new JpegHandler();
			// convert to jpeg
		}

		AssertLog(Utility::IsFormatSupported(format),
			std::string("base::images::BmpHandler::convertToFormat: \""
				+ std::string(format) + "\"" + std::string(" is not supported")).c_str());
	}

	const char* BmpHandler::format() const noexcept {
		return "bmp";
	}
} // namespace base::images