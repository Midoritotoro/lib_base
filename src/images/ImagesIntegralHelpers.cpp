#include <src/images/ImagesIntegralHelpers.h>
#include <base/Assert.h>

#include <iostream>
#include <vector>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#define free_null(a) do { free( a ); a = NULL; } while(0)

#define empty_str(str) (!str || !*str)


namespace base::images {
    namespace {
#ifdef LIB_BASE_ENABLE_sse2
        void BradleyHelperSse2(
            uchar* src, uchar* res,
            int32 width, int32 h,
            int32 _S, int32 _T)
        {
            
        }
#endif
    } // namespace

    void BradleyThreshold(
        uchar* src, uchar* res,
        int32 width, int32 height,
        ushort imageChannelsCount)
    {
        static const float bradleyT = 0.15;

        const auto Index = [=](int32 i, int32 j, int32 color) {
            return (i * width + j) * imageChannelsCount + color;
        };

        const auto Get = [=](int32 i, int32 j, int32 color) {
            return res[Index(i, j, color)];
        };

        const auto Set = [=](int32 i, int32 j, uint8 val) {
            for (uint8 color = 0; color < imageChannelsCount; ++color)
                src[Index(i, j, color)] = val;
        };

        int32 bradleyBlockWidth = width / 16;

        std::vector<std::vector<long>> integral_data(height);
        for (int32 i = 0; i < height; ++i) {
            integral_data[i] = std::vector<long>(width);
            int32 row = 0;

            for (int32 j = 0; j < width; ++j) {
                row += Get(i, j, 0);

                (i == 0)
                    ? integral_data[i][j] = row
                    : integral_data[i][j] = integral_data[i - 1][j] + row;
            }
        }

        for (int32 i = 0; i < height; ++i) {
            for (int32 j = 0; j < width; ++j) {

                int32 x1 = std::max(0, i - bradleyBlockWidth);
                int32 x2 = std::min(height - 1, i + bradleyBlockWidth);

                int32 y1 = std::max(0, j - bradleyBlockWidth);
                int32 y2 = std::min(width - 1, j + bradleyBlockWidth);

                int32 count = (y2 - y1) * (x2 - x1);

                long sum = integral_data[x2][y2] 
                    - integral_data[x1][y2] 
                    - integral_data[x2][y1] 
                    + integral_data[x1][y1];

                uint8 color = 255;

                if (1.0 * Get(i, j, 0) * count < sum * (1.0 - bradleyT))
                    color = 0;
            
                Set(i, j, color);
            }
        }
    }
} // namespace base::images