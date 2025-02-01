#include <base/images/formats/PngHandler.h>
#include <base/images/ImagesUtility.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <base/images/stb/StbImageWrite.h>


namespace base::images {
	void PngHandler::write(
		ImageData* data,
		const char* path)
	{
		int32 success = 0;
		std::string outputImageFormat = Utility::GetExtensionFromPath(path);

		AssertLog(Utility::IsFormatSupported(outputImageFormat.c_str()) != false,
			std::string("base::images::Image::writeImageToFile: Given format: \""
				+ outputImageFormat + "\"" + std::string(" is not supported")).c_str());

		*this = *(PngHandler*)convertToFormat(data, outputImageFormat.c_str());

		success = stbi_write_png(
			path, data->width, data->height,
			data->channels, data->data, data->width * data->channels);

		AssertLog(success != 0, "base::images::Image::writeImageToFile: Error while writing");
	}

	AbstractFormatHandler* PngHandler::convertToFormat(
		ImageData* data,
		const char* format)
	{
		if (Utility::IsJpeg(format))
			stbi_write_jpg_to_func(Utility::StbiWrite,
				data, data->width, data->height,
				data->channels, data->data, data->jpegQuality.has_value()
					? data->jpegQuality.value()
					: kDefaultStbiJpegQuality);
		
		else if (Utility::IsBmp(format))
			stbi_write_bmp_to_func(Utility::StbiWrite,
				data, data->width, data->height,
				data->channels, data->data);
		

		data->imageExtension = format;
	}
} // namespace base::images