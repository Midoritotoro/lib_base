#include <base/images/ImagesIntegral.h>
#include <base/images/gl/ImagesGLImage.h>

#include <private/qimage_p.h>
#include <qDebug>

#include <base/Platform.h>
#include <qsimd.h>

static void Bradley_threshold(unsigned char* src, unsigned char* res, int width, int height) {
    const int S = width / 8;
    int s2 = S / 2;
    const float t = 0.15;
    unsigned long* integral_image = 0;
    long sum = 0;
    int count = 0;
    int index;
    int x1, y1, x2, y2;
    int h = height * sizeof(unsigned long);

    integral_image = new unsigned long[width * h];

    for (int j = 0; j < h; j++) {
        sum = 0;
        for (int i = 0; i < width; i++) {
            index = j * width + i;
            sum += src[index];
            if (j == 0 && i == 0)
                integral_image[index] = sum;
            else if (j == 0)
                integral_image[index] = integral_image[index - 1] + sum;
            else if (i == 0)
                integral_image[index] = integral_image[index - width] + sum;
            else
                integral_image[index] = integral_image[index - 1] + integral_image[index - width] - integral_image[index - width - 1] + src[index];

        }
    }

    //íàõîäèì ãðàíèöû äëÿ ëîêàëüíûå îáëàñòåé
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < h; j++) {
            index = j * width + i;

            x1 = i - s2;
            x2 = i + s2;
            y1 = j - s2;
            y2 = j + s2;

            if (x1 < 0)
                x1 = 0;
            if (x2 >= width)
                x2 = width - 1;
            if (y1 < 0)
                y1 = 0;
            if (y2 >= h)
                y2 = h - 1;

            count = (x2 - x1 + 1) * (y2 - y1 + 1);

            // Ïðàâèëüíîå âû÷èñëåíèå sum ñ ó÷¸òîì ãðàíèö
            sum = integral_image[y2 * width + x2];
            if (y1 > 0) {
                sum -= integral_image[(y1 - 1) * width + x2];
            }
            if (x1 > 0) {
                sum -= integral_image[y2 * width + (x1 - 1)];
            }

            if (y1 > 0 && x1 > 0) {
                sum += integral_image[(y1 - 1) * width + (x1 - 1)];
            }

            if ((long)(src[index] * count) < (long)(sum * (1.0 - t)))
                res[index] = 0;
            else
                res[index] = 255;
        }
    }
    delete[] integral_image;
}


namespace base::images {
    IntegralImage::IntegralImage() {

        const char* out = "D:/pp.png";
        const char* in = "C:\\Users\\danya\\Downloads\\img_.png";

        QImage image(in);

        int total = 0;
        int val = 0;
        for (int i = 0; i < 100; ++i) {
            {
                measureExecutionTimeToValue(val)
                Bradley_threshold(image.data_ptr()->data, image.data_ptr()->data, 1920, 1080);
            }
            total += val;
            qDebug() << "Time: " << val;
        }

        qDebug() << "Average time  " << total / 100;
        //   if (res == nullptr) {
        //       qDebug() << "cannot copy img from src";
        //       return;
        //}
        //   //qDebug() << "ressize: " << sizeof(*res) / sizeof(uchar);

        //   for (auto i = 0; i < 1920 * 1080; ++i)
        //       qDebug() << res[i];

        image.save(out);
    }

    IntegralImage::IntegralImage(const std::vector<std::vector<int32>>& image) :
        _integralImage(image)
    {}

    IntegralImage::IntegralImage(const Image& image) :
        _integralImage(integralImageFromImage(image))
    {}

    const std::vector<std::vector<int32>>& IntegralImage::toVector() const noexcept {
        return _integralImage;
    }

    int32 IntegralImage::width() const noexcept {
        return _integralImage[0].size();
    }

    int32 IntegralImage::height() const noexcept {
        return _integralImage.size();
    }

    Size<int32> IntegralImage::size() const noexcept {
        return Size<int32>(width(), height());
    }

    Point<int32> IntegralImage::randomFreePositionForSize(const Size<int32>& size) const {
        return { 0, 0 };
    }

    std::vector<int>& IntegralImage::operator[](const size_t position) noexcept {
        return _integralImage[position];
    }

    IntegralImage& IntegralImage::operator=(const std::vector<std::vector<int>>& other) noexcept {
        _integralImage = other;
        return *this;
    }

    IntegralImage& IntegralImage::operator=(const Image& image) noexcept {
        _integralImage = integralImageFromImage(image);
        return *this;
    }

    bool IntegralImage::operator==(const IntegralImage& other) const noexcept {
        return _integralImage == other._integralImage;
    }

    bool IntegralImage::operator==(const std::vector<std::vector<int32>>& other) const noexcept {
        return _integralImage == other;
    }

    std::vector<std::vector<int32>> IntegralImage::integralImageFromImage(const Image& image) {
        if (image.isNull())
            return {};

        const auto columns = image.width();
        const auto rows = image.height();

        auto result = std::vector<std::vector<int>>(rows, std::vector<int>(columns));

        for (auto row = 0; row < rows; row++)
            for (auto column = 0; column < columns; column++)
                result[row][column] = result[row][column] + result[row][column]
                - result[row][column] + image.pixel(column, row);

        return result;
    }
} // namespace base::images