#include <src/images/ImagesIntegralHelpers.h>
#include <base/Assert.h>

#include <iostream>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#define free_null(a) do { free( a ); a = NULL; } while(0)

#define empty_str(str) (!str || !*str)


void ExtractByIndices(
    const unsigned char* src,
    const __m512i indices_arr,
    __m512i * out)
{
    __m128i low_indices = _mm512_castsi512_si128(indices_arr);
    __m128i low_mid_indices = _mm512_extracti32x4_epi32(indices_arr, 1);
    __m128i high_mid_indices = _mm512_extracti32x4_epi32(indices_arr, 2);
    __m128i high_indices = _mm512_extracti32x4_epi32(indices_arr, 3);

    __m128i low_bytes = _mm_i32gather_epi32((const int*)src, low_indices, 1);
    __m128i low_mid_bytes = _mm_i32gather_epi32((const int*)src, low_mid_indices, 1);
    __m128i high_mid_bytes = _mm_i32gather_epi32((const int*)src, high_mid_indices, 1);
    __m128i high_bytes = _mm_i32gather_epi32((const int*)src, high_indices, 1);

    //  Объединение 4-х  __m128i в  __m512i
    __m256i low_mid_result = _mm256_inserti128_si256(_mm256_castsi128_si256(low_bytes), low_mid_bytes, 1);
    __m256i high_mid_result = _mm256_inserti128_si256(_mm256_castsi128_si256(high_mid_bytes), high_bytes, 1);

    __m512i result_vector = _mm512_inserti32x8(_mm512_castsi256_si512(low_mid_result), high_mid_result, 1);

    *out = _mm512_shuffle_epi8(result_vector,
        _mm512_set_epi8(
            63, 59, 55, 51, 62, 58, 54, 50,
            47, 43, 39, 35, 46, 42, 38, 34,
            31, 27, 23, 19, 30, 26, 22, 18,
            15, 11, 7, 3, 14, 10, 6, 2,
            61, 57, 53, 49, 60, 56, 52, 48,
            45, 41, 37, 33, 44, 40, 36, 32,
            29, 25, 21, 17, 28, 24, 20, 16,
            13, 9, 5, 1, 12, 8, 4, 0
        )
    );
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

void ProcessImage(
    const uchar* src,
    const int32 width,
    const int32 height,
    const ulong* integralImage,
    uchar* res,
    const int32 SHalf,
    const double T
)
{
    const double _T = 1.0 - T;
    const int32_t y_step = 16;

    for (int32_t x = 0; x < width; ++x) {
        __m512i y_indices = _mm512_add_epi32(_mm512_set1_epi32(x), 
            _mm512_mullo_epi32(
                _mm512_set_epi32(
                    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15),
                _mm512_set1_epi32(width)));

        __m512i rectLeftX = _mm512_sub_epi32(
            _mm512_set1_epi32(x), 
            _mm512_set1_epi32(SHalf));
        __m512i rectRightX = _mm512_add_epi32(
            _mm512_set1_epi32(x), 
            _mm512_set1_epi32(SHalf));

        __m512i rectTopY = _mm512_sub_epi32(
            _mm512_set_epi32(
                0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15),
            _mm512_set1_epi32(SHalf));
        __m512i rectBottomY = _mm512_add_epi32(
            _mm512_set_epi32(
                0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15),
            _mm512_set1_epi32(SHalf));

        rectLeftX = _mm512_max_epi32(rectLeftX, _mm512_set1_epi32(0));
        rectRightX = _mm512_min_epi32(rectRightX, _mm512_set1_epi32(width - 1));
        rectTopY = _mm512_max_epi32(rectTopY, _mm512_set1_epi32(0));
        rectBottomY = _mm512_min_epi32(rectBottomY, _mm512_set1_epi32(height - 1));

        __m512i rectWidth = _mm512_sub_epi32(rectRightX, rectLeftX);
        __m512i rectHeight = _mm512_sub_epi32(rectBottomY, rectTopY);
        __m512i count = _mm512_mullo_epi32(rectWidth, rectHeight);

        __m512i rectBottomRightIndex = _mm512_add_epi32(
            _mm512_mullo_epi32(rectBottomY, 
                _mm512_set1_epi32(width)), rectRightX);
        __m512i rectTopRightIndex = _mm512_add_epi32(
            _mm512_mullo_epi32(rectTopY,
                _mm512_set1_epi32(width)), rectRightX);

        __m512i rectBottomLeftIndex = _mm512_add_epi32(
            _mm512_mullo_epi32(rectBottomY, 
                _mm512_set1_epi32(width)), rectLeftX);
        __m512i rectTopLeftIndex = _mm512_add_epi32(
            _mm512_mullo_epi32(rectTopY, 
                _mm512_set1_epi32(width)), rectLeftX);

        __m512i sum_bottom_right_int = _mm512_i64gather_epi64(rectBottomRightIndex, integralImage, 8);

        __m512i sum_top_right_int = _mm512_i64gather_epi64(rectTopRightIndex, integralImage, 8);
        __m512i sum_bottom_left_int = _mm512_i64gather_epi64(rectBottomLeftIndex, integralImage, 8);
        __m512i sum_top_left_int = _mm512_i64gather_epi64(rectTopLeftIndex, integralImage, 8);

        __m512i sum = _mm512_sub_epi64(_mm512_sub_epi64(sum_bottom_right_int, sum_top_right_int), _mm512_sub_epi64(sum_bottom_left_int, sum_top_left_int));
        __m512i src_values = _mm512_i32gather_epi32(y_indices, src, 1);
        __m512i src_values_extended = _mm512_cvtepi32_epi64(*(__m256i*)src_values.m512i_i64);

        __m512i src_mult = _mm512_mullo_epi64(src_values_extended, _mm512_cvtepi32_epi64(*(__m256i*)count.m512i_i64));

        __m512d t_vec = _mm512_set1_pd(_T);
        __m512d sum_double = _mm512_cvtepi64_pd(sum);
        __m512d sum_t = _mm512_mul_pd(sum_double, t_vec);
        __m512i sum_t_int = _mm512_cvtpd_epi64(sum_t);

        __mmask16 mask = _mm512_cmpgt_epi64_mask(src_mult, sum_t_int); // сравнение

        _mm512_mask_mov_epi32(*(__m512i*)(res + x), mask, _mm512_set1_epi32(255)); //маска из 0 или 255
        _mm512_mask_mov_epi32(*(__m512i*)(res + x), _mm512_knot(mask), _mm512_set1_epi32(0));
    }
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
            int32 width, int32 heigh,
            int32 _S, int32 _T)
        {
            const long height = heigh * 4;
            const int32 SHalf = _S / 2;
            
            ulong* integralImage = (ulong*)aligned_malloc(width * height * 4, 32);

            int32 x, y;
            for (x = 0; x < width; ++x) {
                long long suml = 0;
                __m512i sum = _mm512_set1_epi32(0);
                for (y = 0; y + 15 < height; y += 16) {
                    __m512i index = GetVectorized512BitIndex(width, x, y);
                    
                    __m512i srcSum;
                    ExtractByIndices(src, index, &srcSum);

                    __m128i low = _mm512_castsi512_si128(srcSum);
                    __m128i high = _mm512_extracti32x4_epi32(srcSum, 1);

                    __m128i lo_64_01 = _mm_unpacklo_epi32(low, _mm_setzero_si128()); // 1, 0, 2, 0, 3, 0, 4, 0
                    __m128i hi_64_01 = _mm_unpackhi_epi32(low, _mm_setzero_si128()); // 5, 0, 6, 0, 7, 0, 8, 0

                    __m128i lo_64_23 = _mm_unpacklo_epi32(high, _mm_setzero_si128()); // 1, 0, 2, 0, 3, 0, 4, 0
                    __m128i hi_64_23 = _mm_unpackhi_epi32(high, _mm_setzero_si128()); // 5, 0, 6, 0, 7, 0, 8, 0

                    for (int32 i = 0; i < 16; ++i) {
                        int32 indx = index.m512i_i32[i];
                        suml += src[indx];
                    }

                 //   std::cout << "Indexes Array: " << '\n';

                   // for (int32 i = 0; i < 16; ++i) {
                  //      std::cout << index.m512i_i32[i] << " ";
                  //  }

                    if (y == 0) {
                        // Запись в массив по два 32-битных значения, упакованных в ulong
                        _mm_storel_epi64((__m128i*) (integralImage + 0), lo_64_01);
                        _mm_storel_epi64((__m128i*) (integralImage + 1), hi_64_01);
                        _mm_storel_epi64((__m128i*) (integralImage + 2), lo_64_23);
                        _mm_storel_epi64((__m128i*) (integralImage + 3), hi_64_23);
                    }
                    else {
                        for (int32 i = 0; i < 16; ++i) {
                            int32 indx = index.m512i_i32[i];
                           // if (indx <= 0)
                             //   continue;
                         //   std::cout << "index: " << indx << " " << "integralImage[indx - 1]: " 
                           //     << integralImage[indx - 1] << " " << "suml: " << suml << '\n';
                            integralImage[indx] = integralImage[indx - 1] + suml;
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
            
            ProcessImage(src, width, height, integralImage, res, SHalf, _T);
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
                std::cout << "Index no SIMD: " << index << '\n';

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
    return BradleyHelperAvx2(
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