#pragma once 

#include <QImage>


namespace base::images {
    int getChannelCount(const QImage& image) {
        QImage::Format format = image.format();

        switch (format) {
        case QImage::Format_Mono:
        case QImage::Format_MonoLSB:
            return 1; // Монохромное изображение

        case QImage::Format_Grayscale8:
            return 1; // 8-битное градации серого

        case QImage::Format_Indexed8:
            return 1; // Индексированное изображение

        case QImage::Format_RGB32:
        case QImage::Format_ARGB32:
        case QImage::Format_ARGB32_Premultiplied:
        case QImage::Format_RGBA8888:
        case QImage::Format_RGBA8888_Premultiplied:
        case QImage::Format_BGRA8888:
        case QImage::Format_BGRA8888_Premultiplied:
            return 4; // 3 или 4 канала (RGB или RGBA)

        case QImage::Format_RGB888:
        case QImage::Format_BGR888:
            return 3; // 3 канала (RGB)

            // Добавьте другие форматы, если они вам нужны.

        default:
            qDebug() << "Warning: Unknown QImage format:" << format;
            return -1; // Неизвестный формат
        }
    }

    [[nodiscard]] int getChannelCountByBytes(const QImage& image) {
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
} // namespace base::images