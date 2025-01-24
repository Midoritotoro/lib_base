#include <src/images/ImagesIntegralHelpers.h>
#include <base/Assert.h>

#include <iostream>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#define free_null(a) do { free( a ); a = NULL; } while(0)

#define empty_str(str) (!str || !*str)


void ExtractByIndices(
    const uchar* src,
    const __m512i indices_arr,
    __m128i* out)
{
    __m128i result_vector;

    __m128i low_indices = _mm512_castsi512_si128(indices_arr);
    __m128i low_mid_indices = _mm512_extracti32x4_epi32(indices_arr, 1);
    __m128i high_mid_indices = _mm512_extracti32x4_epi32(indices_arr, 2);
    __m128i high_indices = _mm512_extracti32x4_epi32(indices_arr, 3);

  /*  for (int32 i = 0; i < 4; ++i) {
        std::cout << high_indices.m128i_i32[i] << std::endl;
    }*/

    __m128i low_bytes;
    __m128i low_mid_bytes;
    __m128i high_mid_bytes;
    __m128i high_bytes;

    low_bytes = _mm_i32gather_epi32((const int*)src, low_indices, 1);
    low_mid_bytes = _mm_i32gather_epi32((const int*)src, low_mid_indices, 1);
    high_mid_bytes = _mm_i32gather_epi32((const int*)src, high_mid_indices, 1);
    high_bytes = _mm_i32gather_epi32((const int*)src, high_indices, 1);

    // Объединяем все загруженные данные в один вектор
    result_vector = _mm_unpacklo_epi32(low_bytes, low_mid_bytes);
    result_vector = _mm_unpacklo_epi64(result_vector,
        _mm_unpacklo_epi32(high_mid_bytes, high_bytes));

    *out = _mm_shuffle_epi8(result_vector, _mm_set_epi8(15, 11, 7, 3, 14, 10, 6, 2, 13, 9, 5, 1, 12, 8, 4, 0));
}

[[nodiscard]] __m512i GetVectorized512BitIndex(
    int32 width, 
    int32 x, int32 y)
{
    __m512i yOffsetVector = _mm512_set_epi32(
        y + 15, y + 14, y + 13, y + 12, y + 11,
        y + 10, y + 9, y + 8, y + 7, y + 6, y + 5,
        y + 4, y + 3, y + 2, y + 1, y);

    __m512i vectorizedIndex = _mm512_add_epi32(
        _mm512_mul_epi32(yOffsetVector,
            _mm512_set1_epi32(width)),
        _mm512_set1_epi32(x));

    return vectorizedIndex;
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
            
            ulong* integralImage = (ulong*)aligned_malloc(width * _height, 64);

            int32 x, y;
            for (x = 0; x < width; ++x) {
                ulong suma = 0;
                __m512i sum = _mm512_set1_epi32(0);
                for (y = 0; y + 15 < height; y += 16) {
                    __m512i index = GetVectorized512BitIndex(width, x, y);
                    
                    __m128i tempSum;
                    ExtractByIndices(src, index, &tempSum);

                    __m256i srcSum = _mm512_castsi512_si256(_mm512_castsi128_si512(tempSum));

                    __m128i low = _mm256_castsi256_si128(srcSum);
                    __m128i high = _mm256_extracti128_si256(srcSum, 1);

                    __m128i lo_64_01 = _mm_unpacklo_epi32(low, _mm_setzero_si128()); // 1, 0, 2, 0, 3, 0, 4, 0
                    __m128i hi_64_01 = _mm_unpackhi_epi32(low, _mm_setzero_si128()); // 5, 0, 6, 0, 7, 0, 8, 0

                    __m128i lo_64_23 = _mm_unpacklo_epi32(high, _mm_setzero_si128()); // 1, 0, 2, 0, 3, 0, 4, 0
                    __m128i hi_64_23 = _mm_unpackhi_epi32(high, _mm_setzero_si128()); // 5, 0, 6, 0, 7, 0, 8, 0

                    ulong packed_sum[16];

                    if (y == 0) {
                        // Записываем в массив по два 32-битных значения, упакованных в ulong
                        _mm_storel_epi64((__m128i*) (integralImage + 0), lo_64_01);
                        _mm_storel_epi64((__m128i*) (integralImage + 1), hi_64_01);
                        _mm_storel_epi64((__m128i*) (integralImage + 2), lo_64_23);
                        _mm_storel_epi64((__m128i*) (integralImage + 3), hi_64_23);
                    }
                    else {
                       /* _mm_storel_epi64((__m128i*)(packed_sum), lo_64_01);
                        _mm_storel_epi64((__m128i*)(packed_sum + 1), hi_64_01);
                        _mm_storel_epi64((__m128i*)(packed_sum + 2), lo_64_23);
                        _mm_storel_epi64((__m128i*)(packed_sum + 3), hi_64_23);*/

                        for (int32 i = 0; i < 16; ++i) {

                            int32 indx = index.m512i_i32[i];
                            suma += src[indx];

                            integralImage[indx] = integralImage[indx - 1] + suma;
                        }
                    }
                }
            }


            for (; x < width; ++x) {
                long sum = 0;
                for (; y < height; ++y) {
                    int32 index = y * width + x;


                    sum += src[index];

                    if (y == 0)
                        integralImage[index] = sum;
                    else
                        integralImage[index] = integralImage[index - 1] + sum;
                }
            }

            for (int32 x = 0; x < width; ++x) {
                for (int32 y = 0; y < height; ++y) {
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