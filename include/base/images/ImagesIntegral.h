#pragma once 

#include <base/core/utility/Algorithm.h>
#include <base/images/Images.h>


__BASE_IMAGES_NAMESPACE_BEGIN

class IntegralImage {
public:
    IntegralImage();

    IntegralImage(const std::vector<std::vector<int32>>& image);
    IntegralImage(const Image& image);

    base_nodiscard const std::vector<std::vector<int32>>& toVector() const noexcept;

    base_nodiscard int32 width() const noexcept;
    base_nodiscard int32 height() const noexcept;

    base_nodiscard Size<int32> size() const noexcept;
    base_nodiscard Point<int32> randomFreePositionForSize(const Size<int32>& size) const;

    base_nodiscard std::vector<int>& operator[](const size_t position) noexcept;

    base_nodiscard IntegralImage& operator=(const std::vector<std::vector<int32>>& other) noexcept;
    base_nodiscard IntegralImage& operator=(const Image& image) noexcept;

    base_nodiscard bool operator==(const IntegralImage& other) const noexcept;
    base_nodiscard bool operator==(const std::vector<std::vector<int32>>& other) const noexcept;
private:
    std::vector<std::vector<int32>> integralImageFromImage(const Image& image);

    std::vector<std::vector<int32>> _integralImage;
};

__BASE_IMAGES_NAMESPACE_END

