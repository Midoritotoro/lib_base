#include <base/images/ImagesIntegral.h>
#include <base/images/ImagesImage.h>

#include <base/images/filters/ImageFilter.h>

#include <private/qimage_p.h>
#include <qDebug>

#include <base/system/Platform.h>

#include <QImageReader>
#include <QImageWriter>

class IntegralOccupancyMap {
public:
    static std::pair<int, int> query_integral_image(const std::vector<std::vector<uint32>>& integral_, int size_x, int size_y, std::mt19937& random_state) {
        int x = integral_.size();
        int y = (x > 0) ? integral_[0].size() : 0;
        int area, i, j;
        int hits = 0;

        // count how many possible locations
        for (i = 0; i < x - size_x; ++i) {
            for (j = 0; j < y - size_y; ++j) {
                area = integral_[i][j] + integral_[i + size_x][j + size_y]
                    - integral_[i + size_x][j] - integral_[i][j + size_y];
                if (area == 0) {
                    hits++;
                }
            }
        }

        if (hits == 0) {
            // no room left
            return { -1, -1 }; // Indicate failure with {-1, -1} or use std::optional
        }

        // pick a location at random
        std::uniform_int_distribution<> dist(0, hits - 1); // distribution for random number
        int goal = dist(random_state);
        hits = 0;

        for (i = 0; i < x - size_x; ++i) {
            for (j = 0; j < y - size_y; ++j) {
                area = integral_[i][j] + integral_[i + size_x][j + size_y]
                    - integral_[i + size_x][j] - integral_[i][j + size_y];
                if (area == 0) {
                    hits++;
                    if (hits == goal) {
                        return { i, j };
                    }
                }
            }
        }

        // Should not reach here, but return a default value if it does. This indicates an error.
        std::cerr << "Error: Should not reach this point in query_integral_image" << std::endl;
        return { -1, -1 }; // Indicate failure if the loop completes without finding a solution
    }

    IntegralOccupancyMap(int height, int width, const std::vector<std::vector<bool>>& mask) : height_(height), width_(width) {
        if (!mask.empty()) { // Check if mask is empty, not nullptr
            std::cout << "cumsum" << std::endl;
            integral_.resize(height_, std::vector<unsigned int>(width_, 0));

            // Simulate np.cumsum(np.cumsum(255 * mask, axis=1), axis=0)
            for (int i = 0; i < height_; ++i) {
                for (int j = 0; j < width_; ++j) {
                    unsigned int val = (mask[i][j] ? 255 : 0); // Simulate 255 * mask

                    // Cumsum along axis 1
                    if (j > 0) {
                        val += integral_[i][j - 1];
                    }

                    // Cumsum along axis 0
                    if (i > 0) {
                        val += integral_[i - 1][j];
                    }
                    if (i > 0 && j > 0) {
                        val -= integral_[i - 1][j - 1];
                    }
                    integral_[i][j] = val;
                }
            }

        }
        else {
            std::cout << "Mask is None" << std::endl;
            integral_.resize(height_, std::vector<unsigned int>(width_, 0)); // Initialize with zeros
        }
    }

    std::pair<int, int> sample_position(int size_x, int size_y, std::mt19937& random_state) {
        return query_integral_image(integral_, size_x, size_y, random_state); // Assuming this function is defined elsewhere
    }

    void update(const std::vector<std::vector<unsigned char>>& img_array, int pos_x, int pos_y) {
        int img_height = img_array.size();
        int img_width = (img_height > 0) ? img_array[0].size() : 0;
        int partial_height = img_height - pos_x;
        int partial_width = img_width - pos_y;

        std::vector<std::vector<unsigned int>> partial_integral(partial_height, std::vector<unsigned int>(partial_width, 0));

        // Simulate partial_integral = np.cumsum(np.cumsum(img_array[pos_x:, pos_y:], axis=1), axis=0)
        for (int i = 0; i < partial_height; ++i) {
            for (int j = 0; j < partial_width; ++j) {
                unsigned int val = img_array[pos_x + i][pos_y + j];

                // Cumsum along axis 1
                if (j > 0) {
                    val += partial_integral[i][j - 1];
                }

                // Cumsum along axis 0
                if (i > 0) {
                    val += partial_integral[i - 1][j];
                }
                if (i > 0 && j > 0) {
                    val -= partial_integral[i - 1][j - 1];
                }
                partial_integral[i][j] = val;
            }
        }

        std::cout << "Source image: " << img_width << " " << img_height << std::endl;
        std::cout << "partialIntegral: " << partial_width << " " << partial_height << std::endl;
        std::cout << "pos_x:, pos_y: " << pos_y << " " << pos_x << std::endl;

        // Paste recomputed part into old image
        for (int i = 0; i < partial_height; ++i) {
            for (int j = 0; j < partial_width; ++j) {
                unsigned int additional_val = 0;
                if (pos_x > 0) {
                    if (pos_y > 0) {
                        additional_val = (integral_[pos_x - 1][pos_y + j] - integral_[pos_x - 1][pos_y - 1]);
                    }
                    else {
                        additional_val = integral_[pos_x - 1][pos_y + j];
                    }
                }
                if (pos_y > 0) {
                    additional_val += integral_[pos_x + i][pos_y - 1];
                    if (pos_x > 0) {
                        additional_val -= integral_[pos_x - 1][pos_y - 1];
                    }
                }

                integral_[pos_x + i][pos_y + j] = partial_integral[i][j] + additional_val;
            }
        }
    }

    int height_;
    int width_;

    std::vector<std::vector<uint32>> integral_;
};

struct Image {
    int width;
    int height;

    Image() {}
    Image(int w, int h) :width(w), height(h) {}
};
enum Rotation {
    NONE,
    ROTATE_90
};

// Replace with your actual image drawing function
// void drawText(Image& image, int x, int y, const std::string& text, const std::string& fontPath, int fontSize, const std::string& color, Rotation orientation);
struct BoxSize {
    int x1, y1, x2, y2;

    BoxSize(int a, int b, int c, int d) :x1(a), y1(b), x2(c), y2(d) {}
};
// Replace with your actual function to get text bounding box size
BoxSize getTextBBox(const std::string& text, const std::string& fontPath, int fontSize, Rotation orientation);

// Replace with your actual implementation for color function
std::string colorFunc(const std::string& word, int fontSize, std::pair<int, int> position, Rotation orientation, std::mt19937& random_state, const std::string& fontPath);

// Need img_grey from outside so pass it as argument
bool generateFromFrequencies(
    std::vector<std::pair<std::string, int>>& frequencies,
    IntegralOccupancyMap& occupancy,
    Image& img_grey,
    std::mt19937& random_state,
    const std::string& font_path,
    int min_font_size,
    int font_step,
    double relative_scaling,
    double prefer_horizontal,
    int margin,
    std::vector<std::pair<int, int>>& positions,
    std::vector<Rotation>& orientations,
    std::vector<int>& font_sizes,
    std::vector<std::string>& colors,
    const std::vector<std::vector<bool>>& mask,
    const std::vector<std::vector<bool>>& boolean_mask,
    std::vector<std::vector<unsigned char>>& img_array
) {
    int last_freq = 1; // Initialize to 1 to avoid division by zero

    // Assuming there is draw function you provide to draw text.
    std::pair<int, int> result;
    for (auto& pair : frequencies) {
        std::string word = pair.first;
        int freq = pair.second;

        if (freq == 0) {
            continue;
        }

        double rs = relative_scaling;
        int font_size = 50; // Some value
        if (rs != 0) {
            font_size = static_cast<int>(std::round((rs * (static_cast<double>(freq) / static_cast<double>(last_freq)) + (1 - rs)) * font_size));
        }

        Rotation orientation = (std::generate_canonical<double, 10>(random_state) < prefer_horizontal) ? NONE : ROTATE_90;
        bool tried_other_orientation = false;

        while (true) {
            if (font_size < min_font_size) {
                break;
            }

            // Replace this font with one of your own (truetype or similar)
            // ImageFont::Font font = ImageFont::truetype(font_path, font_size);

            // Create a transposed font
            // ImageFont::TransposedFont transposed_font(font, orientation);
            BoxSize box_size = getTextBBox(word, font_path, font_size, orientation);

            result = IntegralOccupancyMap::query_integral_image(
                occupancy.integral_, box_size.y2 + margin, box_size.x2 + margin, random_state);

            if (result.first != -1 && result.second != -1) {
                // Found a place
                break;
            }

            if (!tried_other_orientation && prefer_horizontal < 1) {
                orientation = (orientation == NONE) ? ROTATE_90 : NONE;
                tried_other_orientation = true;
            }
            else {
                font_size -= font_step;
                orientation = NONE;
            }
        }

        if (font_size < min_font_size) {
            break;
        }

        std::pair<int, int> xy = { result.first + margin / 2, result.second + margin / 2 };
        //drawText(img, xy.second, xy.first, word, font_path, font_size, "white", orientation); // Example

        positions.push_back(xy);
        orientations.push_back(orientation);
        font_sizes.push_back(font_size);
        colors.push_back(colorFunc(word, font_size, xy, orientation, random_state, font_path));

        // Recompute integral image
        std::vector<std::vector<unsigned char>> img_array2 = img_array;
        if (mask.empty()) {
            //img_array = img_grey; // Assign
        }
        else {
            // Combine
        }

        occupancy.update(img_array2, xy.first, xy.second);

        last_freq = freq;
    }
    return true;
}

// Dummy implementations
Image getImage() {
    return Image{ 10, 20 };
}
BoxSize getTextBBox(const std::string& text, const std::string& fontPath, int fontSize, Rotation orientation) {
    return BoxSize(1, 2, 3, 4);
}
std::string colorFunc(const std::string& word, int fontSize, std::pair<int, int> position, Rotation orientation, std::mt19937& random_state, const std::string& fontPath) {
    return "black";
}



namespace base::images {
    IntegralImage::IntegralImage() {
       // const char* out = "D:/pp.bmp"; error
        const char* out = "D:/pp.png"; 
        const char* in = "C:\\Users\\danya\\Downloads\\bmphqi.bmp";

        QImage image(in);

        {
            measureExecutionTime("IntegralImage: ")
            ImageFilter(&image).filter(Filter::BradleyThreshold);
        }

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