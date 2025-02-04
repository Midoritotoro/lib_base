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

	void PngHandler::read(
		ImageData* data,
		const char* path)
	{
		png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL,
			NULL);
		AssertLog(png_ptr != nullptr, "base::images::PngHandler::read: Невозможно прочитать изображение, переполнение памяти ");

		png_infop info_ptr = png_create_info_struct(png_ptr);
		if (!info_ptr) {
			png_destroy_read_struct(&png_ptr, NULL, NULL);
			AssertLog(false, "base::images::PngHandler::read: Невозможно прочитать изображение, переполнение памяти ");
		}

		if (png_jmpbuf(png_ptr)) {
			png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
			return;
		}
	
		png_init_io(png_ptr, infile);
		png_set_sig_bytes(png_ptr, 8);
		png_read_info(png_ptr, info_ptr);
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