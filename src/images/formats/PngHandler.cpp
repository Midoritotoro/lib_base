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
		FILE* file)
	{
    

        //if (setjmp(png_jmpbuf(png_ptr))) {
        //    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        //    png_ptr = nullptr;
        //    amp.deallocate();
        //    state = Error;
        //    return false;
        //}

        //if (gamma != 0.0 && fileGamma != 0.0) {
        //    // This configuration forces gamma correction and
        //    // thus changes the output colorspace
        //    png_set_gamma(png_ptr, 1.0f / gamma, fileGamma);
        //    colorSpace.setTransferFunction(QColorSpace::TransferFunction::Gamma, 1.0f / gamma);
        //    colorSpaceState = GammaChrm;
        //}

        //bool doScaledRead = false;
        //if (!setup_qt(*outImage, png_ptr, info_ptr, scaledSize, &doScaledRead)) {
        //    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        //    png_ptr = nullptr;
        //    amp.deallocate();
        //    state = Error;
        //    return false;
        //}

        //if (doScaledRead) {
        //    read_image_scaled(outImage, png_ptr, info_ptr, amp, scaledSize);
        //}
        //else {
        //    png_uint_32 width = 0;
        //    png_uint_32 height = 0;
        //    png_int_32 offset_x = 0;
        //    png_int_32 offset_y = 0;

        //    int bit_depth = 0;
        //    int color_type = 0;
        //    int unit_type = PNG_OFFSET_PIXEL;
        //    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, nullptr, nullptr, nullptr);
        //    png_get_oFFs(png_ptr, info_ptr, &offset_x, &offset_y, &unit_type);
        //    uchar* data = outImage->bits();
        //    qsizetype bpl = outImage->bytesPerLine();
        //    amp.row_pointers = new png_bytep[height];

        //    for (uint y = 0; y < height; y++)
        //        amp.row_pointers[y] = data + y * bpl;

        //    png_read_image(png_ptr, amp.row_pointers);
        //    amp.deallocate();

        //    outImage->setDotsPerMeterX(png_get_x_pixels_per_meter(png_ptr, info_ptr));
        //    outImage->setDotsPerMeterY(png_get_y_pixels_per_meter(png_ptr, info_ptr));

        //    if (unit_type == PNG_OFFSET_PIXEL)
        //        outImage->setOffset(QPoint(offset_x, offset_y));

        //    // sanity check palette entries
        //    if (color_type == PNG_COLOR_TYPE_PALETTE && outImage->format() == QImage::Format_Indexed8) {
        //        int color_table_size = outImage->colorCount();
        //        for (int y = 0; y < (int)height; ++y) {
        //            uchar* p = FAST_SCAN_LINE(data, bpl, y);
        //            uchar* end = p + width;
        //            while (p < end) {
        //                if (*p >= color_table_size)
        //                    *p = 0;
        //                ++p;
        //            }
        //        }
        //    }
        //}

        //state = ReadingEnd;
        //png_read_end(png_ptr, end_info);

        //readPngTexts(end_info);
        //for (int i = 0; i < readTexts.size() - 1; i += 2)
        //    outImage->setText(readTexts.at(i), readTexts.at(i + 1));

        //png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        //png_ptr = nullptr;
        //amp.deallocate();
        //state = Ready;

        //if (scaledSize.isValid() && outImage->size() != scaledSize)
        //    *outImage = outImage->scaled(scaledSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

		fclose(file);
	//	png_destroy_read_struct(&png, &info, NULL);

		qDebug() << "width: " << data->width << "height: " << data->height;
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