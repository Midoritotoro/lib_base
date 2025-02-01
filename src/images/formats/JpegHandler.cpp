#include <base/images/formats/JpegHandler.h>
#include <base/images/ImagesUtility.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <base/images/stb/StbImageWrite.h>


namespace base::images {
	void JpegHandler::write(
		ImageData* data,
		const char* path)
	{
		int32 success = 0;
		std::string outputImageFormat = Utility::GetExtensionFromPath(path);

		*this = *(JpegHandler*)convertToFormat(data, outputImageFormat.c_str());

		success = stbi_write_jpg(
			path, data->width, data->height,
			data->channels, data->data, data->jpegQuality.has_value()
				? data->jpegQuality.value()
				: kDefaultStbiJpegQuality);

		AssertLog(success != 0, "base::images::JpegHandler::write: Error while writing");
	}

	AbstractFormatHandler* JpegHandler::convertToFormat(
		ImageData* data,
		const char* format)
	{
		if (Utility::IsPng(format))
			stbi_write_png_to_mem(
				data->data, data->width * data->channels,
				data->width, data->height,
				data->channels, &data->sizeInBytes);
		
		else if (Utility::IsBmp(format))
			stbi_write_bmp_to_func(Utility::StbiWrite,
				data, data->width, data->height,
				data->channels, data->data);

		data->imageExtension = format;
	}
} // namespace base::images