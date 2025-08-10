#pragma once

#include <base/media/ffmpeg/video/VideoFormat.h>
#include <base/core/utility/Math.h>

#include <base/core/BaseNamespace.h>

extern "C" {
    #include <libavutil/pixfmt.h>
    #include <libavutil/avutil.h>
}


__BASE_MEDIA_FFMPEG_VIDEO_NAMESPACE_BEGIN

class Chroma {
public:
    struct ChromaDescription {
        fourcc_t fcc;

        unsigned plane_count;

        struct {
            rational_t w;
            rational_t h;
        } p[4];

        unsigned pixel_size;        /* Number of bytes per pixel for a plane */
        unsigned pixel_bits;        /* Number of bits actually used bits per pixel for a plane */
    };

      static int GetChroma(
        video_format_t* fmt,
        enum AVPixelFormat i_ffmpeg_chroma);

      static enum AVPixelFormat FindFfmpegChroma(
        fourcc_t fourcc,
        bool* uv_flipped);

      static const ChromaDescription*
        FourccGetChromaDescription(fourcc_t i_fourcc);
};

__BASE_MEDIA_FFMPEG_VIDEO_NAMESPACE_END