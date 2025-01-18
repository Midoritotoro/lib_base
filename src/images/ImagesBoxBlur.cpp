#include <base/images/ImagesBoxBlur.h>
#include <base/Types.h>


namespace base::images {
    Image boxBlurImage(
        const Image& image,
        const Rect& rect,
        int radius)
    {
        static constexpr int tab[] = {
            14, 10, 8, 6, 5, 5, 4, 3, 3, 
            3, 3, 2, 2, 2, 2, 2, 2 
        };

        const auto alpha = (radius < 1) 
            ? 16 : (radius > 17) 
            ? 1 : tab[radius - 1];

        auto result = image.convertToFormat(Image::Format_ARGB32_Premultiplied);

        const auto r1 = rect.top();
        const auto r2 = rect.bottom();

        const auto c1 = rect.left();
        const auto c2 = rect.right();

        const auto bpl = result.bytesPerLine();

        int rgba[4];
        uchar* p;

        auto i1 = 0;
        auto i2 = 3;

        for (auto col = c1; col <= c2; col++) {
            p = result.scanLine(r1) + col * 4;
            for (auto i = i1; i <= i2; i++)
                rgba[i] = p[i] << 4;

            p += bpl;
            for (auto j = r1; j < r2; j++, p += bpl)
                for (auto i = i1; i <= i2; i++)
                    p[i] = static_cast<uchar>((rgba[i] += ((p[i] << 4) - rgba[i]) * alpha / 16) >> 4);
        }

        for (auto row = r1; row <= r2; row++) {
            p = result.scanLine(row) + c1 * 4;
            for (auto i = i1; i <= i2; i++)
                rgba[i] = p[i] << 4;

            p += 4;
            for (auto j = c1; j < c2; j++, p += 4)
                for (auto i = i1; i <= i2; i++)
                    p[i] = static_cast<uchar>((rgba[i] += ((p[i] << 4) - rgba[i]) * alpha / 16) >> 4);
        }

        for (auto col = c1; col <= c2; col++) {
            p = result.scanLine(r2) + col * 4;
            for (auto i = i1; i <= i2; i++)
                rgba[i] = p[i] << 4;

            p -= bpl;
            for (auto j = r1; j < r2; j++, p -= bpl)
                for (auto i = i1; i <= i2; i++)
                    p[i] = static_cast<uchar>((rgba[i] += ((p[i] << 4) - rgba[i]) * alpha / 16) >> 4);
        }

        for (auto row = r1; row <= r2; row++) {
            p = result.scanLine(row) + c2 * 4;
            for (auto i = i1; i <= i2; i++)
                rgba[i] = p[i] << 4;

            p -= 4;
            for (auto j = c1; j < c2; j++, p -= 4)
                for (auto i = i1; i <= i2; i++)
                    p[i] = static_cast<uchar>((rgba[i] += ((p[i] << 4) - rgba[i]) * alpha / 16) >> 4);
        }

        return result;
    }

    Image boxBlurImage(
        const Image& _image,
        int _radius)
    {
        return boxBlurImage(
            _image,
            _image.rect(),
            _radius);
    }
} // namespace base::images