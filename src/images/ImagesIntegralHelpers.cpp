#include <src/images/ImagesIntegralHelpers.h>
#include <base/Assert.h>

#include <iostream>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))


void GetVectorized512BitIndex(
    int32 width, 
    int32 x, int32 y,
    int32* out)
{
    __m512i x_offset_vec = _mm512_set_epi32(
        15, 14, 13, 12, 11, 10, 9, 
        8, 7, 6, 5, 4, 3, 2, 1, 0);

    __m512i width_vec = _mm512_set1_epi32(width);

    __m512i y_offset_vec = _mm512_set_epi32(
        y + 15, y + 14, y + 13, y + 12, y + 11, y + 10, y + 9, y + 8,
        y + 7, y + 6, y + 5, y + 4, y + 3, y + 2, y + 1, y + 0);

    __m512i x_vec_idx = _mm512_add_epi32(x_offset_vec, _mm512_set1_epi32(x));
    __m512i index_vec_1 = _mm512_add_epi32(
        _mm512_mullo_epi32(y_offset_vec, width_vec),
        x_vec_idx);

    _mm512_storeu_epi32((__m512i*)(out + (y * width + x)), index_vec_1);
}

namespace base::images {
    namespace {
        size_t strlen(uchar* s)
        {
            size_t i = 0;
            while (s[i++]);
            return i - 1;
        }

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
            const long _height = height * 4;
            const int32 SHalf = _S / 2;
            
            ulong* integralImage = (ulong*)aligned_malloc(width * _height, 16);

            for (int32 x = 0; x < width; x += 16) {
                for (int32 y = 0; y < _height; ++y) {
                    int32* temp = (int32*)aligned_malloc(16 * sizeof(int32), 16);

                    __m512i index = GetVectorized512BitIndex(width, x, y, );
                    __m512i sum = _mm512_set1_epi32(0);

                    __m256i srcValuesFirstPart = _mm512_extracti32x8_epi32(*((__m512i*)src), 8);
                    __m256i srcValuesSecondPart = _mm512_extracti32x8_epi32(*((__m512i*)src + 8), 16);

                    __m512i tempVectorizedSrc = _mm512_inserti64x4(
                        _mm512_castsi256_si512(srcValuesFirstPart), 
                        srcValuesSecondPart, 1);

                    sum = _mm512_add_epi32(sum, tempVectorizedSrc);

                    if (y == 0)
                        integralImage[];

                    aligned_free(temp);
                }
            }

            aligned_free(integralImage);
        }
#endif
    void BradleyHelperNoSIMD(
        uchar* src, uchar* res,
        int32 width, int32 height,
        int32 _S, int32 _T)
    {
        const long _height = height * 4;
        const int32 SHalf = _S / 2;

        ulong* integralImage = new ulong[width * _height];

        for (int32 x = 0; x < width; ++x) {
            long sum = 0;
            for (int32 y = 0; y < _height; ++y) {
                int32 index = y * width + x;

                sum += src[index];

                if (y == 0)
                    integralImage[index] = sum;
                else
                    integralImage[index] = integralImage[index - 1] + sum;
            }
        }

        for (int32 x = 0; x < width; ++x) {
            for (int32 y = 0; y < _height; ++y) {
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

                if (rectBottomY >= _height)
                    rectBottomY = _height - 1;

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

#ifdef LIB_BASE_ENABLE_sse4_1
    return BradleyHelperSse4_1(
        src, res, width, height,
        S, t);
#endif
#ifdef LIB_BASE_ENABLE_avx2
    return BradleyHelperSse4_1(
        src, res, width, height,
        S, t);
#endif
#ifdef LIB_BASE_ENABLE_sse4_2
    return BradleyHelperSse4_2(
        src, res, width, height,
        S, t);
#endif
    return BradleyHelperNoSIMD(
        src, res, width, height,
        S, t);
    }
} // namespace base::images