#include <base/images/formats/BmpHandler.h>
#include <base/images/ImagesUtility.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <base/images/stb/StbImageWrite.h>


namespace base::images {
	void BmpHandler::write(
		ImageData* data,
		const char* path)
	{
		int32 success = 0;
		std::string outputImageFormat = Utility::GetExtensionFromPath(path);

		*this = *(BmpHandler*)convertToFormat(data, outputImageFormat.c_str());

		success = stbi_write_bmp(
			path, data->width, data->height,
			data->channels, data->data);

		AssertLog(success != 0, "base::images::Image::writeImageToFile: Error while writing");
	}

	AbstractFormatHandler* BmpHandler::convertToFormat(
		ImageData* data,
		const char* format)
	{
		if (Utility::IsFormatsEqual(data->imageExtension, format))
			return;

		if (Utility::IsPng(format))
			stbi_write_png_to_mem(
				data->data, data->width * data->channels,
				data->width, data->height,
				data->channels, &data->sizeInBytes);
		
		else if (Utility::IsJpeg(format))
			stbi_write_jpg_to_func(Utility::StbiWrite,
				data, data->width, data->height,
				data->channels, data->data, data->jpegQuality.has_value()
					? data->jpegQuality.value()
					: kDefaultStbiJpegQuality);

		AssertLog(Utility::IsFormatSupported(format),
			std::string("base::images::BmpHandler::convertToFormat: \""
				+ std::string(format) + "\"" + std::string(" is not supported")).c_str());


		data->imageExtension = format;
	}
} // namespace base::images