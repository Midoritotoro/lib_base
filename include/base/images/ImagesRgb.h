#pragma once 


namespace base::images {
    using Rgb = uint;
    inline constexpr Rgb kRgbMask = 0x00ffffff;

    inline constexpr int RgbRed(Rgb rgb) {
        return ((rgb >> 16) & 0xff);
    }

    inline constexpr int RgbGreen(Rgb rgb) {
        return ((rgb >> 8) & 0xff);
    }

    inline constexpr int RgbBlue(Rgb rgb) {
        return (rgb & 0xff);
    }

    inline constexpr int RgbAlpha(Rgb rgb) {
        return rgb >> 24;
    }

    inline constexpr Rgb RgbRgb(int r, int g, int b) {
        return (0xffu << 24) 
            | ((r & 0xffu) << 16) 
            | ((g & 0xffu) << 8) 
            | (b & 0xffu);
    }

    inline constexpr Rgb RgbRgba(int r, int g, int b, int a) {
        return ((a & 0xffu) << 24) 
            | ((r & 0xffu) << 16) 
            | ((g & 0xffu) << 8) 
            | (b & 0xffu);
    }

    inline constexpr int RgbGray(int r, int g, int b) {
        return (r * 11 + g * 16 + b * 5) / 32;
    }

    inline constexpr int RgbGray(Rgb rgb) {
        return RgbGray(RgbRed(rgb), RgbGreen(rgb), RgbBlue(rgb));
    }

    inline constexpr bool RgbIsGray(Rgb rgb) {
        return RgbRed(rgb) == RgbGreen(rgb) 
            && RgbRed(rgb) == RgbBlue(rgb);
    }
} // namespace base::images