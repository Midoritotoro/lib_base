#include <base/images/formats/PngHandler.h>
#include <base/images/ImagesUtility.h>

#include <base/images/formats/BmpHandler.h>
#include <base/images/formats/JpegHandler.h>

#include <png.h>


namespace base::images {
	void PngHandler::write(
		ImageData* data,
		const char* path)
	{
		int32 success = 0;
		std::string outputImageFormat = Utility::GetExtensionFromPath(path);

		if (Utility::IsFormatsEqual(data->handler->format(), outputImageFormat.c_str()) == false) {
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
		if (Utility::IsFormatsEqual(data->handler->format(), format))
			return;

		if (Utility::IsJpeg(format)) {
			data->handler = new JpegHandler();
		// convert to jpg
		}

		else if (Utility::IsBmp(format)) {
			data->handler = new BmpHandler();
			// convert to bmp
		}

		AssertLog(Utility::IsFormatSupported(format),
			std::string("base::images::PngHandler::convertToFormat: \""
				+ std::string(format) + "\"" + std::string(" is not supported")).c_str());
	}

	const char* PngHandler::format() const noexcept {
		return "png";
	}
} // namespace base::images