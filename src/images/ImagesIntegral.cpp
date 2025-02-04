#include <base/images/ImagesIntegral.h>
#include <base/images/ImagesImage.h>

#include <private/qimage_p.h>
#include <qDebug>

#include <base/system/Platform.h>

#include <QImageReader>
#include <QImageWriter>


namespace base::images {
    IntegralImage::IntegralImage() {
       // const char* out = "D:/pp.bmp"; error
        const char* out = "D:/pp.png"; 
        const char* in = "C:\\Users\\danya\\Downloads\\img_.png";
        

        Image image;
        {
            measureExecutionTime("Image::loadFromFile: ")
            image.load(in);
        }


       qDebug() << image.width() << image.height() << strlen((char*)image.data_ptr()->data) << image.bytesPerLine() << image.depth();

      /* {
            measureExecutionTime("IntegralImage: ")
            BradleyThreshold(
                image.data_ptr()->data, image.data_ptr()->data,
                image.data_ptr()->width, image.data_ptr()->height,
                image.depth() / 8);
        }*/

        {
            measureExecutionTime("Image::save: ")
            image.save(out);
        }

       // qDebug() << image.width() << image.height() << strlen((char*)image.data_ptr()->data) << image.bytesPerLine() << image.depth();
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