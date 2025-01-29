#include <base/images/ImagesBlur.h>

#define STB_IMAGE_IMPLEMENTATION
#include <base/images/stb/StbImage.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <base/images/stb/StbImageWrite.h>

#include <iostream>
#include <cstring>

#include <chrono>
#include <type_traits>

#include <algorithm>
#include <cmath>

#include <vector>
#include <omp.h>


namespace base::images {
    template<typename T, int C, Policy P>
    void HorizontalBlur(
        const T* in, T* out, const int w,
        const int h, const int r)
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
    template<typename T>
    void HorizontalBlur(
        const T* in, T* out, const int w,
        const int h, const int c, const int r)
    {
        switch (c)
        {
        case 1: HorizontalBlur<T, 1>(in, out, w, h, r); break;
        case 2: HorizontalBlur<T, 2>(in, out, w, h, r); break;
        case 3: HorizontalBlur<T, 3>(in, out, w, h, r); break;
        case 4: HorizontalBlur<T, 4>(in, out, w, h, r); break;
        default: printf("%d channels is not supported yet. Add a specific case if possible or fall back to the generic version.", c); break;
            // default: HorizontalBlur<T>(in, out, w, h, c, r); break;
        }
    }

    namespace {
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
        void FlipBlock(const T* in, T* out, const int w, const int h)
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
        void FlipBlock(
            const T* in, T* out, const int w,
            const int h, const int c)
        {
            switch (c)
            {
            case 1: FlipBlock<T, 1>(in, out, w, h); break;
            case 2: FlipBlock<T, 2>(in, out, w, h); break;
            case 3: FlipBlock<T, 3>(in, out, w, h); break;
            case 4: FlipBlock<T, 4>(in, out, w, h); break;
            default: printf("%d channels is not supported yet. Add a specific case if possible or fall back to the generic version.", c); break;
                // default: FlipBlock<T>(in, out, w, h, c); break;
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
        void SigmaToBoxRadius(
            int boxes[],
            const float sigma,
            const int n)
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
        void FastGaussianBlurN(
            T*& in, T*& out, const int w, 
            const int h, const int c, 
            const float sigma, const int n)
        {
            // compute box kernel sizes
            std::vector<int> boxes(n);
            SigmaToBoxRadius(boxes.data(), sigma, n);

            // perform N horizontal blur passes
            for (int i = 0; i < n; ++i)
            {
                HorizontalBlur(in, out, w, h, c, boxes[i]);
                std::swap(in, out);
            }

            // flip buffer
            FlipBlock(in, out, w, h, c);
            std::swap(in, out);

            // perform N horizontal blur passes
            for (int i = 0; i < n; ++i)
            {
                HorizontalBlur(in, out, h, w, c, boxes[i]);
                std::swap(in, out);
            }

            // flip buffer
            FlipBlock(in, out, h, w, c);
        }
    } // namespace 

    template<typename T>
    void FastGaussianBlur(
        T*& in, T*& out, const int w, 
        const int h, const int c,
        const float sigma, const int n)
    {

        if (n != 3)
            return FastGaussianBlurN(in, out, w, h, c, sigma, n);

        // compute box kernel sizes
        int n = 3;
        int boxes[3];
        SigmaToBoxRadius(boxes, sigma, n);

        // perform 3 horizontal blur passes
        HorizontalBlur(in, out, w, h, c, boxes[0]);
        HorizontalBlur(out, in, w, h, c, boxes[1]);
        HorizontalBlur(in, out, w, h, c, boxes[2]);

        // flip buffer
        FlipBlock(out, in, w, h, c);

        // perform 3 horizontal blur passes
        HorizontalBlur(in, out, h, w, c, boxes[0]);
        HorizontalBlur(out, in, h, w, c, boxes[1]);
        HorizontalBlur(in, out, h, w, c, boxes[2]);

        // flip buffer
        FlipBlock(out, in, h, w, c);

        // swap pointers to get result in the ouput buffer 
        std::swap(in, out);
    }
} // namespace base::images