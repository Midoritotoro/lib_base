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
            const int32 s2 = _S / 2;
        }
#endif
#ifdef LIB_BASE_ENABLE_sse4_2
        void BradleyHelperSse4_2(
            uchar* src, uchar* res,
            int32 width, int32 height,
            int32 _S, int32 _T)
        {
            const int32 s2 = _S / 2;
        }
#endif
#ifdef LIB_BASE_ENABLE_sse4_1
        void BradleyHelperSse4_1(
            unsigned char* src, unsigned char* res,
            int32_t width, int32_t height,
            int32_t _S, int32_t _T)
        {
            const int32_t s2 = _S / 2;
            const int32_t h = height * sizeof(ulong);

            ulong* integral_image = (ulong*)_aligned_malloc(width * h, 16);
            Expects(integral_image != nullptr);


            std::cout << width << " " << h << '\n';

            for (int32_t j = 0; j < height; ++j) {
                __m128i sum_vec = _mm_setzero_si128();
                for (int32_t i = 0; i < width; i += 16) { // обрабатываем 16 байт за раз
                    __m128i src_vec;
                    if (i + 16 <= width) {
                        src_vec = _mm_loadu_si128((const __m128i*)(src + j * width + i));
                    }
                    else {
                        unsigned char temp[16] = { 0 };
                        for (int k = 0; k < width - i; k++) {
                            temp[k] = src[j * width + i + k];
                        }
                        src_vec = _mm_loadu_si128((const __m128i*)temp);

                    }

                    sum_vec = _mm_add_epi32(sum_vec, _mm_cvtepu8_epi32(src_vec));
                    __m128i prev;
                    if (i == 0) {
                        if (j == 0)
                            _mm_store_si128((__m128i*)(integral_image + j * width + i), sum_vec);
                        else {
                            _mm_store_si128((__m128i*)(integral_image + j * width + i),
                                _mm_add_epi64(sum_vec,
                                    _mm_load_si128((__m128i*)(integral_image + (j - 1) * width + i))));
                        }
                    }
                    else {
                        if (j == 0) {
                            prev = _mm_load_si128((const __m128i*)(integral_image + j * width + i - 16));
                            _mm_store_si128((__m128i*)(integral_image + j * width + i), _mm_add_epi64(sum_vec, prev));
                        }
                        else {
                            prev = _mm_load_si128((const __m128i*)(integral_image + j * width + i - 16));

                            _mm_store_si128((__m128i*)(integral_image + j * width + i),
                                _mm_add_epi64(sum_vec,
                                    _mm_add_epi64(prev,
                                        _mm_load_si128((const __m128i*)(integral_image + (j - 1) * width + i)))));

                            _mm_store_si128((__m128i*)(integral_image + j * width + i), 
                                _mm_sub_epi64(_mm_load_si128((const __m128i*)
                                    (integral_image + j * width + i)), 
                                    _mm_load_si128((const __m128i*)(integral_image + (j - 1) * width + i - 16))));
                        }
                    }
                }
            }


            // Локальная пороговая обработка с SSE4.1
            for (int32_t j = 0; j < height; ++j) {
                for (int32_t i = 0; i < width; ++i) {
                    int32_t index = j * width + i;
                    int32_t x1 = i - s2;
                    int32_t x2 = i + s2;
                    int32_t y1 = j - s2;
                    int32_t y2 = j + s2;

                    if (x1 < 0) x1 = 0;
                    if (x2 >= width) x2 = width - 1;
                    if (y1 < 0) y1 = 0;
                    if (y2 >= height) y2 = height - 1;

                    int32_t count = (x2 - x1 + 1) * (y2 - y1 + 1);

                    __m128i sum_vec = _mm_setzero_si128();
                    __m128i y2x2 = _mm_loadu_si128((const __m128i*)(integral_image + y2 * width + x2));
                    if (y1 > 0)
                        sum_vec = _mm_sub_epi64(sum_vec, _mm_loadu_si128((const __m128i*)(integral_image + (y1 - 1) * width + x2)));

                    if (x1 > 0)
                        sum_vec = _mm_sub_epi64(sum_vec, _mm_loadu_si128((const __m128i*)(integral_image + y2 * width + (x1 - 1))));


                    if (y1 > 0 && x1 > 0) {
                        sum_vec = _mm_add_epi64(sum_vec, _mm_loadu_si128((const __m128i*)(integral_image + (y1 - 1) * width + (x1 - 1))));

                    }
                    long sum = 0;
                    unsigned long temp[2];
                    _mm_storeu_si128((__m128i*)temp, sum_vec);
                    sum = temp[0] + temp[1];


                    if ((long)(src[index] * count) < (long)(sum * (1.0 - _T)))
                        res[index] = 0;
                    else
                        res[index] = 255;
                }
            }

            _aligned_free(integral_image);
        }
  
#endif
    void BradleyHelperNoSIMD(
        uchar* src, uchar* res,
        int32 width, int32 height,
        int32 _S, int32 _T)
    {
        const int32 s2 = _S / 2;
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

                if ((long)(src[index] * count) < (long)(sum * (1.0 - _T)))
                    res[index] = 0;
                else
                    res[index] = 255;
            }
        }
        delete[] integral_image;
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