#pragma once 

#include <base/core/utility/Algorithm.h>
#include <base/images/Images.h>


__BASE_IMAGES_NAMESPACE_BEGIN

class IntegralImage {
public:
    IntegralImage();

    IntegralImage(const std::vector<std::vector<int32>>& image);
    IntegralImage(const Image& image);

      const std::vector<std::vector<int32>>& toVector() const noexcept;

      int32 width() const noexcept;
      int32 height() const noexcept;

      Size<int32> size() const noexcept;
      Point<int32> randomFreePositionForSize(const Size<int32>& size) const;

      std::vector<int>& operator[](const size_t position) noexcept;

      IntegralImage& operator=(const std::vector<std::vector<int32>>& other) noexcept;
      IntegralImage& operator=(const Image& image) noexcept;

      bool operator==(const IntegralImage& other) const noexcept;
      bool operator==(const std::vector<std::vector<int32>>& other) const noexcept;
private:
    std::vector<std::vector<int32>> integralImageFromImage(const Image& image);

    std::vector<std::vector<int32>> _integralImage;
};

__BASE_IMAGES_NAMESPACE_END

