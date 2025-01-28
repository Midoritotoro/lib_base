#include <base/images/ImagesUtility.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <base/images/StbImage.h>


namespace base::images {
#ifdef LIB_BASE_ENABLE_QT
    int getChannelsCountByFormat(const QImage& image) {
        auto format = image.format();

        switch (format) {
            case QImage::Format_Mono:
            case QImage::Format_MonoLSB:
                return 1;

            case QImage::Format_Grayscale8:
                return 1;

            case QImage::Format_Indexed8:
                return 1;

            case QImage::Format_RGB32:
            case QImage::Format_ARGB32:
            case QImage::Format_ARGB32_Premultiplied:
            case QImage::Format_RGBA8888:
            case QImage::Format_RGBA8888_Premultiplied:

            case QImage::Format_RGB888:
            case QImage::Format_BGR888:
                return 3;
        }

        return -1;
    }

    [[nodiscard]] int getChannelsCountByBytes(const QImage& image) {
        const auto width = image.width();
        const auto bytesPerLine = image.bytesPerLine();

        if (width <= 0 || bytesPerLine <= 0)
            return -1;

        const auto bytesPerPixel = bytesPerLine / width;

        switch (bytesPerPixel) {
            case 1:  return 1; // Grayscale, Mono
            case 3:  return 3; // RGB
            case 4: return 4;  // RGBA
        }

        return -1;
    }
#endif

    const char* GetImageExtension(uchar* image) {
        stbi__context s;

        if (stbi__png_test(s))
            return "png";
        else if (stbi__bmp_test(s))
            return "bmp";
        else if (stbi__gif_test(s)) 
        	return "gif";
        else if (stbi__psd_test(s))
        	return "psd";
        else if (stbi__pic_test(s))
        	return "pic";
        else if (stbi__jpeg_test(s))
        	return "jpeg";
        else if (stbi__pnm_test(s))
        	return "pnm";
        else if (stbi__tga_test(s))
        	return "tga";

        return nullptr;
} // namespace base::images