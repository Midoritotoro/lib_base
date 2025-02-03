#include <base/images/formats/JpegHandler.h>
#include <base/images/ImagesUtility.h>


#include <base/images/formats/BmpHandler.h>
#include <base/images/formats/PngHandler.h>


namespace base::images {
	void JpegHandler::write(
		ImageData* data,
		const char* path)
	{
		int32 success = 0;
		std::string outputImageFormat = Utility::GetExtensionFromPath(path);

		if (Utility::IsFormatsEqual(data->imageExtension, outputImageFormat.c_str()) == false) {
			convertToFormat(data, outputImageFormat.c_str());
			return data->handler->write(data, path);
		}

	// success = write jpeg

		AssertLog(success != 0, "base::images::JpegHandler::write: Error while writing");
	}

	void JpegHandler::convertToFormat(
		ImageData* data,
		const char* format)
	{
		if (Utility::IsFormatsEqual(data->imageExtension, format))
			return;

		data->imageExtension = format;

		if (Utility::IsPng(format)) {
			data->handler = new PngHandler();
		// convert to png
		}

		else if (Utility::IsBmp(format)) {
			data->handler = new BmpHandler();
			// convert to bmp
		}

		AssertLog(Utility::IsFormatSupported(format),
			std::string("base::images::PngHandler::convertToFormat: \""
				+ std::string(format) + "\"" + std::string(" is not supported")).c_str());
	}

	const char* JpegHandler::format() const noexcept {
		return "jpeg";
	}
} // namespace base::images