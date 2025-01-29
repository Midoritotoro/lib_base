#include <base/images/ImagesUtility.h>
#include <iostream>


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
} // namespace base::images