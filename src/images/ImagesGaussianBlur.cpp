#include <base/images/ImagesGaussianBlur.h>

#define STB_IMAGE_IMPLEMENTATION
#include <base/images/StbImage.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <base/images/StbImageWrite.h>

#include <iostream>
#include <cstring>

#include <chrono>
#include <type_traits>

#include <algorithm>
#include <cmath>

#include <vector>


namespace base::images {
    namespace {
        enum Policy { EXTEND, KERNEL_CROP };

        template<typename T, int C, Policy P = KERNEL_CROP>
        void horizontal_blur(const T* in, T* out, const int w, const int h, const int r)
        {
            float iarr = 1.f / (r + r + 1);
#pragma omp parallel for
            for (int i = 0; i < h; i++)
            {
                int ti = i * w, li = ti, ri = ti + r;
                float fv[C], lv[C], val[C];

                for (int ch = 0; ch < C; ++ch)
                {
                    fv[ch] = P == EXTEND ? in[ti * C + ch] : 0; // unused with kcrop policy
                    lv[ch] = P == EXTEND ? in[(ti + w - 1) * C + ch] : 0; // unused with kcrop policy
                    val[ch] = P == EXTEND ? (r + 1) * fv[ch] : 0;
                }

                // initial acucmulation
                for (int j = 0; j < r; j++)
                    for (int ch = 0; ch < C; ++ch)
                    {
                        val[ch] += in[(ti + j) * C + ch];
                    }

                // left border - filter kernel is incomplete
                for (int j = 0; j <= r; j++, ri++, ti++)
                    for (int ch = 0; ch < C; ++ch)
                    {
                        val[ch] += P == EXTEND ? in[ri * C + ch] - fv[ch] : in[ri * C + ch];
                        out[ti * C + ch] = P == EXTEND ? val[ch] * iarr : val[ch] / (r + j + 1);
                    }

                // center of the image - filter kernel is complete
                for (int j = r + 1; j < w - r; j++, ri++, ti++, li++)
                    for (int ch = 0; ch < C; ++ch)
                    {
                        val[ch] += in[ri * C + ch] - in[li * C + ch];
                        out[ti * C + ch] = val[ch] * iarr;
                    }

                // right border - filter kernel is incomplete
                for (int j = w - r; j < w; j++, ti++, li++)
                    for (int ch = 0; ch < C; ++ch)
                    {
                        val[ch] += P == EXTEND ? lv[ch] - in[li * C + ch] : -in[li * C + ch];
                        out[ti * C + ch] = P == EXTEND ? val[ch] * iarr : val[ch] / (r + w - j);
                    }
            }
        }

        //!
        //! \brief Utility template dispatcher function for horizontal_blur. Templated by buffer data type T.
        //!
        //! \param[in] in           source buffer
        //! \param[in,out] out      target buffer
        //! \param[in] w            image width
        //! \param[in] h            image height
        //! \param[in] c            image channels
        //! \param[in] r            box dimension
        //!
        template<typename T>
        void horizontal_blur(const T* in, T* out, const int w, const int h, const int c, const int r)
        {
            switch (c)
            {
            case 1: horizontal_blur<T, 1>(in, out, w, h, r); break;
            case 2: horizontal_blur<T, 2>(in, out, w, h, r); break;
            case 3: horizontal_blur<T, 3>(in, out, w, h, r); break;
            case 4: horizontal_blur<T, 4>(in, out, w, h, r); break;
            default: printf("%d channels is not supported yet. Add a specific case if possible or fall back to the generic version.", c); break;
                // default: horizontal_blur<T>(in, out, w, h, c, r); break;
            }
        }

        //!
        //! \brief This function performs a 2D tranposition of an image. The transposition is done per 
        //! block to reduce the number of cache misses and improve cache coherency for large image buffers.
        //! Templated by buffer data type T and buffer number of channels C.
        //!
        //! \param[in] in           source buffer
        //! \param[in,out] out      target buffer
        //! \param[in] w            image width
        //! \param[in] h            image height
        //!
        template<typename T, int C>
        void flip_block(const T* in, T* out, const int w, const int h)
        {
            constexpr int block = 256 / C;
#pragma omp parallel for collapse(2)
            for (int x = 0; x < w; x += block)
                for (int y = 0; y < h; y += block)
                {
                    const T* p = in + y * w * C + x * C;
                    T* q = out + y * C + x * h * C;

                    const int blockx = std::min(w, x + block) - x;
                    const int blocky = std::min(h, y + block) - y;
                    for (int xx = 0; xx < blockx; xx++)
                    {
                        for (int yy = 0; yy < blocky; yy++)
                        {
                            for (int k = 0; k < C; k++)
                                q[k] = p[k];
                            p += w * C;
                            q += C;
                        }
                        p += -blocky * w * C + C;
                        q += -blocky * C + h * C;
                    }
                }
        }
        //!
        //! \brief Utility template dispatcher function for flip_block. Templated by buffer data type T.
        //!
        //! \param[in] in           source buffer
        //! \param[in,out] out      target buffer
        //! \param[in] w            image width
        //! \param[in] h            image height
        //! \param[in] c            image channels
        //!
        template<typename T>
        void flip_block(const T* in, T* out, const int w, const int h, const int c)
        {
            switch (c)
            {
            case 1: flip_block<T, 1>(in, out, w, h); break;
            case 2: flip_block<T, 2>(in, out, w, h); break;
            case 3: flip_block<T, 3>(in, out, w, h); break;
            case 4: flip_block<T, 4>(in, out, w, h); break;
            default: printf("%d channels is not supported yet. Add a specific case if possible or fall back to the generic version.", c); break;
                // default: flip_block<T>(in, out, w, h, c); break;
            }
        }

        //!
        //! \brief this function converts the standard deviation of 
        //! Gaussian blur into a box radius for each box blur pass. For 
        //! further details please refer to :
        //! https://www.peterkovesi.com/papers/FastGaussianSmoothing.pdf
        //!
        //! \param[out] boxes   box radiis for kernel sizes of 2*boxes[i]+1
        //! \param[in] sigma    Gaussian standard deviation
        //! \param[in] n        number of box blur pass
        //!
        void sigma_to_box_radius(int boxes[], const float sigma, const int n)
        {
            // ideal filter width
            float wi = std::sqrt((12 * sigma * sigma / n) + 1);
            int wl = wi; // no need std::floor  
            if (wl % 2 == 0) wl--;
            int wu = wl + 2;

            float mi = (12 * sigma * sigma - n * wl * wl - 4 * n * wl - 3 * n) / (-4 * wl - 4);
            int m = mi + 0.5f; // avoid std::round by adding 0.5f and cast to integer type

            for (int i = 0; i < n; i++)
                boxes[i] = ((i < m ? wl : wu) - 1) / 2;
        }

        template<typename T>
        void fast_gaussian_blur_N(T*& in, T*& out, const int w, const int h, const int c, const float sigma, const int n)
        {
            // compute box kernel sizes
            std::vector<int> boxes(n);
            sigma_to_box_radius(boxes.data(), sigma, n);

            // perform N horizontal blur passes
            for (int i = 0; i < n; ++i)
            {
                horizontal_blur(in, out, w, h, c, boxes[i]);
                std::swap(in, out);
            }

            // flip buffer
            flip_block(in, out, w, h, c);
            std::swap(in, out);

            // perform N horizontal blur passes
            for (int i = 0; i < n; ++i)
            {
                horizontal_blur(in, out, h, w, c, boxes[i]);
                std::swap(in, out);
            }

            // flip buffer
            flip_block(in, out, h, w, c);
        }
    } // namespace 

    template<typename T>
    void FastGaussianBlur(
        T*& in, T*& out, const int w, 
        const int h, const int c,
        const float sigma, const int n)
    {

        if (n != 3)
            return fast_gaussian_blur_N(in, out, w, h, c, sigma, n);

        // compute box kernel sizes
        int n = 3;
        int boxes[3];
        sigma_to_box_radius(boxes, sigma, n);

        // perform 3 horizontal blur passes
        horizontal_blur(in, out, w, h, c, boxes[0]);
        horizontal_blur(out, in, w, h, c, boxes[1]);
        horizontal_blur(in, out, w, h, c, boxes[2]);

        // flip buffer
        flip_block(out, in, w, h, c);

        // perform 3 horizontal blur passes
        horizontal_blur(in, out, h, w, c, boxes[0]);
        horizontal_blur(out, in, h, w, c, boxes[1]);
        horizontal_blur(in, out, h, w, c, boxes[2]);

        // flip buffer
        flip_block(out, in, h, w, c);

        // swap pointers to get result in the ouput buffer 
        std::swap(in, out);
    }
} // namespace base::images