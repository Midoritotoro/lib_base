#include <src/images/ImagesIntegralHelpers.h>
#include <base/Assert.h>

#include <iostream>

namespace base::images {
    namespace {
#ifdef LIB_BASE_ENABLE_avx2
        void BradleyHelperAvx2(
            uchar* src, uchar* res,
            int32 width, int32 height,
            int32 _S, int32 _T)
        {
        }
#endif
#ifdef LIB_BASE_ENABLE_sse4_2
        void BradleyHelperSse4_2(
            uchar* src, uchar* res,
            int32 width, int32 height,
            int32 _S, int32 _T)
        {
        }
#endif
#ifdef LIB_BASE_ENABLE_sse4_1
        void BradleyHelperSse4_1(
            uchar* src, uchar* res,
            int32 width, int32 height,
            int32 _S, int32 _T)
        {

        }
#endif
    void BradleyHelperNoSIMD(
        uchar* src, uchar* res,
        int32 width, int32 height,
        int32 _S, int32 _T)
    {
        int SHalf = _S / 2;
        ulong* integralImage = new ulong[width * height * sizeof(ulong*)];

        for (int32 x = 0; x < width; x++) {
            long sum = 0;
            for (int32 y = 0; y < height; y++) {
                int32 index = y * width + x;

                sum += src[index];

                if (y == 0)
                    integralImage[index] = sum;
                else
                    integralImage[index] = integralImage[index - 1] + sum;
            }
        }

        for (int32 x = 0; x < width; x++) {
            for (int32 y = 0; y < height; y++) {
                int32 index = y * width + x;

                int32 rectLeftX = x - SHalf;
                int32 rectRightX = x + SHalf;
                int32 rectTopY = y - SHalf;
                int32 rectBottomY = y + SHalf;

                if (rectLeftX < 0)
                    rectLeftX = 0;
                if (rectRightX >= width)
                    rectRightX = width - 1;
                if (rectTopY < 0)
                    rectTopY = 0;
                if (rectBottomY >= height)
                    rectBottomY = height - 1;

                int32 count = (rectRightX - rectLeftX) * (rectBottomY - rectTopY);

                long sum = integralImage[rectBottomY * width + rectRightX] 
                    - integralImage[rectTopY * width + rectRightX] 
                    - integralImage[rectBottomY * width + rectLeftX] 
                    + integralImage[rectTopY * width + rectLeftX];

                if ((long)(src[index] * count) < (long)(sum * (1.0 - _T)))
                    res[index] = 0;
                else
                    res[index] = 255;
            }
        }

        delete[] integralImage;
    }
     } // namespace

    void BradleyThreshold(
        uchar* src, uchar* res,
        int32 width, int32 height)
    {
        const int32 S = width / 8;
        static const float t = 0.15;

//#ifdef LIB_BASE_ENABLE_sse4_1
//    return BradleyHelperSse4_1(
//        src, res, width, height,
//        S, t);
//#endif
//#ifdef LIB_BASE_ENABLE_avx2
//    return BradleyHelperSse4_1(
//        src, res, width, height,
//        S, t);
//#endif
//#ifdef LIB_BASE_ENABLE_sse4_2
//    return BradleyHelperSse4_2(
//        src, res, width, height,
//        S, t);
//#endif
    return BradleyHelperNoSIMD(
        src, res, width, height,
        S, t);
    }
} // namespace base::images