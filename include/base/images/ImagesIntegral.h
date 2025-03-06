#pragma once 

#include <base/core/utility/Algorithm.h>
#include <base/images/Images.h>


__BASE_IMAGES_NAMESPACE_BEGIN

class IntegralImage {
public:
    IntegralImage();

    IntegralImage(const std::vector<std::vector<int32>>& image);
    IntegralImage(const Image& image);

    NODISCARD const std::vector<std::vector<int32>>& toVector() const noexcept;

    NODISCARD int32 width() const noexcept;
    NODISCARD int32 height() const noexcept;

    NODISCARD Size<int32> size() const noexcept;
    NODISCARD Point<int32> randomFreePositionForSize(const Size<int32>& size) const;

    NODISCARD std::vector<int>& operator[](const size_t position) noexcept;

    NODISCARD IntegralImage& operator=(const std::vector<std::vector<int32>>& other) noexcept;
    NODISCARD IntegralImage& operator=(const Image& image) noexcept;

    NODISCARD bool operator==(const IntegralImage& other) const noexcept;
    NODISCARD bool operator==(const std::vector<std::vector<int32>>& other) const noexcept;
private:
    std::vector<std::vector<int32>> integralImageFromImage(const Image& image);

    std::vector<std::vector<int32>> _integralImage;
};

__BASE_IMAGES_NAMESPACE_END

