#pragma once 

#include <base/Utility.h>
#include <base/images/Images.h>

#include <vector>


namespace base::images {
    class IntegralImage {
    public:
        IntegralImage();

        IntegralImage(const std::vector<std::vector<int>>& image);
        IntegralImage(const Image& image);

        [[nodiscatd]] const std::vector<std::vector<int>>& toVector() const noexcept;

        [[nodiscard]] int width() const noexcept;
        [[nodiscard]] int height() const noexcept;

        [[nodiscard]] Size size() const noexcept;

        [[nodiscard]] std::vector<int>& operator[](const size_t position) noexcept;

        [[nodiscard]] IntegralImage& operator=(const std::vector<std::vector<int>>& other) noexcept;
        [[nodiscard]] IntegralImage& operator=(const Image& image) noexcept;

        [[nodiscard]] bool operator==(const IntegralImage& other) const noexcept;
        [[nodiscard]] bool operator==(const std::vector<std::vector<int>>& other) const noexcept;
    private:
        std::vector<std::vector<int>> integralImageFromImage(const Image& image);

        std::vector<std::vector<int>> _integralImage;
    };
}