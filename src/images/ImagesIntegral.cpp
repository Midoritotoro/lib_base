#include <base/images/ImagesIntegral.h>


namespace base::images {
    IntegralImage::IntegralImage() {

    }

    IntegralImage::IntegralImage(const std::vector<std::vector<int>>& image) :
        _integralImage(image)
    {}

    IntegralImage::IntegralImage(const Image& image) :
        _integralImage(integralImageFromImage(image))
    {}

    const std::vector<std::vector<int>>& IntegralImage::toVector() const noexcept {
        return _integralImage;
    }

    int IntegralImage::width() const noexcept {
        return _integralImage[0].size();
    }

    int IntegralImage::height() const noexcept {
        return _integralImage.size();
    }

    Size IntegralImage::size() const noexcept {
        return Size(width(), height());
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

    bool IntegralImage::operator==(const std::vector<std::vector<int>>& other) const noexcept {
        return _integralImage == other;
    }

    std::vector<std::vector<int>> IntegralImage::integralImageFromImage(const Image& image) {
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