#pragma once

#include <base/media/ffmpeg/video/VideoFormat.h>
#include <base/Utility.h>

extern "C" {
    #include <libavutil/pixfmt.h>
    #include <libavutil/avutil.h>
}


namespace base::media::ffmpeg::video {
    struct chroma_description_t {
        fourcc_t fcc;

        unsigned plane_count;

        struct {
            rational_t w;
            rational_t h;
        } p[4];

        unsigned pixel_size;        /* Number of bytes per pixel for a plane */
        unsigned pixel_bits;        /* Number of bits actually used bits per pixel for a plane */
    };

    [[nodiscard]] int GetVlcChroma(
        video_format_t* fmt,
        enum AVPixelFormat i_ffmpeg_chroma);
    [[nodiscard]] enum AVPixelFormat FindFfmpegChroma(
        fourcc_t fourcc,
        bool* uv_flipped);

    [[nodiscard]] const chroma_description_t*
        FourccGetChromaDescription(fourcc_t i_fourcc);
} // namespace base::media::ffmpeg::video