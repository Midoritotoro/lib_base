#pragma once 

#include <base/Utility.h>
#include <base/images/Images.h>

#include <vector>


namespace base::images {
    class IntegralImage {
    public:
        IntegralImage();

        IntegralImage(const std::vector<std::vector<int32>>& image);
        IntegralImage(const Image& image);

        [[nodiscatd]] const std::vector<std::vector<int32>>& toVector() const noexcept;

        [[nodiscard]] int32 width() const noexcept;
        [[nodiscard]] int32 height() const noexcept;

        [[nodiscard]] Size<int32> size() const noexcept;
        [[nodiscard]] Point<int32> randomFreePositionForSize(const Size<int32>& size) const;


        [[nodiscard]] std::vector<int>& operator[](const size_t position) noexcept;

        [[nodiscard]] IntegralImage& operator=(const std::vector<std::vector<int32>>& other) noexcept;
        [[nodiscard]] IntegralImage& operator=(const Image& image) noexcept;

        [[nodiscard]] bool operator==(const IntegralImage& other) const noexcept;
        [[nodiscard]] bool operator==(const std::vector<std::vector<int32>>& other) const noexcept;
    private:
        std::vector<std::vector<int32>> integralImageFromImage(const Image& image);



        std::vector<std::vector<int32>> _integralImage;
    };
}