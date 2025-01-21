#include <src/images/ImagesIntegralHelpers.h>


namespace base::images {
    namespace {
        namespace {
#ifdef LIB_BASE_ENABLE_avx2
            void BradleyHelperAvx2(
                uchar* src, uchar* res,
                int32 width, int32 height)
            {

            }
#endif
#ifdef LIB_BASE_ENABLE_sse4_2
            void BradleyHelperSse4_2(
                uchar* src, uchar* res,
                int32 width, int32 height) 
            {

            }
#endif
#ifdef LIB_BASE_ENABLE_sse3
            void BradleyHelperSse3(
                uchar* src, uchar* res,
                int32 width, int32 height)
            {

            }
#endif
        } // namespace


    } // namespace 
    void BradleyThreshold(
        uchar* src, uchar* res,
        int32 width, int32 height)
    {
        const int32 S = width / 8;
        const int32 s2 = S / 2;
        static const float t = 0.15;
        ulong* integral_image = 0;
        long sum = 0;
        int32 count = 0;
        int32 index;
        int32 x1, y1, x2, y2;
        int32 h = height * sizeof(ulong);

        integral_image = new ulong[width * h];

        for (int32 j = 0; j < h; j++) {
            sum = 0;
            for (int32 i = 0; i < width; i++) {
                index = j * width + i;
                sum += src[index];
                if (j == 0 && i == 0)
                    integral_image[index] = sum;
                else if (j == 0)
                    integral_image[index] = integral_image[index - 1] + sum;
                else if (i == 0)
                    integral_image[index] = integral_image[index - width] + sum;
                else
                    integral_image[index] = integral_image[index - 1]
                        + integral_image[index - width]
                        - integral_image[index - width - 1] + src[index];

            }
        }

        for (int32 i = 0; i < width; i++) {
            for (int32 j = 0; j < h; j++) {
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
} // namespace base::images