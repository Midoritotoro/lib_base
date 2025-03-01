#include <base/media/ffmpeg/video/Chroma.h>
#include <base/media/ffmpeg/video/Fourcc.h>

__BASE_MEDIA_FFMPEG_VIDEO_NAMESPACE_BEGIN

constexpr static struct
{
    fourcc_t  i_chroma;
    enum AVPixelFormat i_chroma_id;
    video_color_range_t range;

} chromaTable[] =
{
    /* Planar YUV formats */
    {CODEC_I444, AV_PIX_FMT_YUV444P,  COLOR_RANGE_UNDEF },
    {CODEC_I444, AV_PIX_FMT_YUVJ444P,  COLOR_RANGE_FULL },

    {CODEC_I440, AV_PIX_FMT_YUV440P,  COLOR_RANGE_UNDEF },
    {CODEC_I440, AV_PIX_FMT_YUVJ440P,  COLOR_RANGE_FULL },

    {CODEC_I422, AV_PIX_FMT_YUV422P,  COLOR_RANGE_UNDEF },
    {CODEC_I422, AV_PIX_FMT_YUVJ422P,  COLOR_RANGE_FULL },

    {CODEC_I420, AV_PIX_FMT_YUV420P,  COLOR_RANGE_UNDEF },
    {CODEC_YV12, AV_PIX_FMT_YUV420P,  COLOR_RANGE_UNDEF },
    {CODEC_I420, AV_PIX_FMT_YUVJ420P,  COLOR_RANGE_FULL },
    {CODEC_I411, AV_PIX_FMT_YUV411P,  COLOR_RANGE_UNDEF },
    {CODEC_I411, AV_PIX_FMT_YUVJ411P,  COLOR_RANGE_FULL },
    {CODEC_I410, AV_PIX_FMT_YUV410P,  COLOR_RANGE_UNDEF },

    {CODEC_NV12, AV_PIX_FMT_NV12,  COLOR_RANGE_UNDEF },
    {CODEC_NV21, AV_PIX_FMT_NV21,  COLOR_RANGE_UNDEF },

    {CODEC_I420_9L, AV_PIX_FMT_YUV420P9LE,  COLOR_RANGE_UNDEF },
    {CODEC_I420_9B, AV_PIX_FMT_YUV420P9BE,  COLOR_RANGE_UNDEF },
    {CODEC_I420_10L, AV_PIX_FMT_YUV420P10LE,  COLOR_RANGE_UNDEF },
    {CODEC_I420_10B, AV_PIX_FMT_YUV420P10BE,  COLOR_RANGE_UNDEF },
#ifdef AV_PIX_FMT_YUV420P12 /* 54, 17, 100 */
    {CODEC_I420_12L, AV_PIX_FMT_YUV420P12LE,  COLOR_RANGE_UNDEF },
    {CODEC_I420_12B, AV_PIX_FMT_YUV420P12BE,  COLOR_RANGE_UNDEF },
#endif
    {CODEC_I420_16L, AV_PIX_FMT_YUV420P16LE,  COLOR_RANGE_UNDEF },
    {CODEC_I420_16B, AV_PIX_FMT_YUV420P16BE,  COLOR_RANGE_UNDEF },
#ifdef AV_PIX_FMT_P010LE
    {CODEC_P010, AV_PIX_FMT_P010LE,  COLOR_RANGE_UNDEF },
#endif
#ifdef AV_PIX_FMT_P016LE
    {CODEC_P016, AV_PIX_FMT_P016LE,  COLOR_RANGE_UNDEF },
#endif

    {CODEC_I422_9L, AV_PIX_FMT_YUV422P9LE,  COLOR_RANGE_UNDEF },
    {CODEC_I422_9B, AV_PIX_FMT_YUV422P9BE,  COLOR_RANGE_UNDEF },
    {CODEC_I422_10L, AV_PIX_FMT_YUV422P10LE,  COLOR_RANGE_UNDEF },
    {CODEC_I422_10B, AV_PIX_FMT_YUV422P10BE,  COLOR_RANGE_UNDEF },
    {CODEC_I422_16L, AV_PIX_FMT_YUV422P16LE,  COLOR_RANGE_UNDEF },
    {CODEC_I422_16B, AV_PIX_FMT_YUV422P16BE,  COLOR_RANGE_UNDEF },
#ifdef AV_PIX_FMT_YUV422P12 /* 54, 17, 100 */
    {CODEC_I422_12L, AV_PIX_FMT_YUV422P12LE,  COLOR_RANGE_UNDEF },
    {CODEC_I422_12B, AV_PIX_FMT_YUV422P12BE,  COLOR_RANGE_UNDEF },
#endif

    {CODEC_YUV420A, AV_PIX_FMT_YUVA420P,  COLOR_RANGE_UNDEF },
    {CODEC_YUV422A, AV_PIX_FMT_YUVA422P,  COLOR_RANGE_UNDEF },
    {CODEC_YUVA,    AV_PIX_FMT_YUVA444P,  COLOR_RANGE_UNDEF },

    {CODEC_YUVA_444_10L, AV_PIX_FMT_YUVA444P10LE,  COLOR_RANGE_UNDEF },
    {CODEC_YUVA_444_10B, AV_PIX_FMT_YUVA444P10BE,  COLOR_RANGE_UNDEF },

    {CODEC_YUVA_444_12L, AV_PIX_FMT_YUVA444P12LE,  COLOR_RANGE_UNDEF },
    {CODEC_YUVA_444_12B, AV_PIX_FMT_YUVA444P12BE,  COLOR_RANGE_UNDEF },

    {CODEC_I444_9L, AV_PIX_FMT_YUV444P9LE,  COLOR_RANGE_UNDEF },
    {CODEC_I444_9B, AV_PIX_FMT_YUV444P9BE,  COLOR_RANGE_UNDEF },
    {CODEC_I444_10L, AV_PIX_FMT_YUV444P10LE,  COLOR_RANGE_UNDEF },
    {CODEC_I444_10B, AV_PIX_FMT_YUV444P10BE,  COLOR_RANGE_UNDEF },
#ifdef AV_PIX_FMT_YUV444P12 /* 54, 17, 100 */
    {CODEC_I444_12L, AV_PIX_FMT_YUV444P12LE,  COLOR_RANGE_UNDEF },
    {CODEC_I444_12B, AV_PIX_FMT_YUV444P12BE,  COLOR_RANGE_UNDEF },
#endif
    {CODEC_I444_16L, AV_PIX_FMT_YUV444P16LE,  COLOR_RANGE_UNDEF },
    {CODEC_I444_16B, AV_PIX_FMT_YUV444P16BE,  COLOR_RANGE_UNDEF },

    /* Packed YUV formats */
    {CODEC_YUYV, AV_PIX_FMT_YUYV422,  COLOR_RANGE_UNDEF },
    {CODEC_UYVY, AV_PIX_FMT_UYVY422,  COLOR_RANGE_UNDEF },
    {CODEC_YVYU, AV_PIX_FMT_YVYU422,  COLOR_RANGE_UNDEF },
    /* Packed RGB formats */
    {CODEC_RGB233, AV_PIX_FMT_RGB8,  COLOR_RANGE_UNDEF },
    {CODEC_BGR233, AV_PIX_FMT_BGR8,  COLOR_RANGE_UNDEF },

    {CODEC_RGB565BE, AV_PIX_FMT_RGB565BE,  COLOR_RANGE_UNDEF },
    {CODEC_RGB565LE, AV_PIX_FMT_RGB565LE,  COLOR_RANGE_UNDEF },
    {CODEC_BGR565BE, AV_PIX_FMT_BGR565BE,  COLOR_RANGE_UNDEF },
    {CODEC_BGR565LE, AV_PIX_FMT_BGR565LE,  COLOR_RANGE_UNDEF },

    {CODEC_RGB555BE, AV_PIX_FMT_RGB555BE,  COLOR_RANGE_UNDEF },
    {CODEC_RGB555LE, AV_PIX_FMT_RGB555LE,  COLOR_RANGE_UNDEF },
    {CODEC_BGR555BE, AV_PIX_FMT_BGR555BE,  COLOR_RANGE_UNDEF },
    {CODEC_BGR555LE, AV_PIX_FMT_BGR555LE,  COLOR_RANGE_UNDEF },

    {CODEC_RGB24, AV_PIX_FMT_RGB24,  COLOR_RANGE_UNDEF },
    {CODEC_BGR24, AV_PIX_FMT_BGR24,  COLOR_RANGE_UNDEF },

    {CODEC_RGBX, AV_PIX_FMT_RGB0,  COLOR_RANGE_UNDEF },
    {CODEC_XRGB, AV_PIX_FMT_0RGB,  COLOR_RANGE_UNDEF },
    {CODEC_BGRX, AV_PIX_FMT_BGR0,  COLOR_RANGE_UNDEF },
    {CODEC_XBGR, AV_PIX_FMT_0BGR,  COLOR_RANGE_UNDEF },

    {CODEC_RGBA, AV_PIX_FMT_RGBA,  COLOR_RANGE_UNDEF },
    {CODEC_ARGB, AV_PIX_FMT_ARGB,  COLOR_RANGE_UNDEF },
    {CODEC_BGRA, AV_PIX_FMT_BGRA,  COLOR_RANGE_UNDEF },
    {CODEC_ABGR, AV_PIX_FMT_ABGR,  COLOR_RANGE_UNDEF },

#ifdef WORDS_BIGENDIAN
    {CODEC_RGBA64, AV_PIX_FMT_RGBA64BE,  COLOR_RANGE_UNDEF },
#else /* !WORDS_BIGENDIAN */
    {CODEC_RGBA64, AV_PIX_FMT_RGBA64LE,  COLOR_RANGE_UNDEF },
#endif /* !WORDS_BIGENDIAN */

#ifdef AV_PIX_FMT_X2BGR10
    {CODEC_RGBA10LE, AV_PIX_FMT_X2BGR10,  COLOR_RANGE_UNDEF },
#endif

    {CODEC_GREY, AV_PIX_FMT_GRAY8,  COLOR_RANGE_UNDEF},
#ifdef AV_PIX_FMT_GRAY10
    {CODEC_GREY_10L, AV_PIX_FMT_GRAY10LE,  COLOR_RANGE_UNDEF},
    {CODEC_GREY_10B, AV_PIX_FMT_GRAY10BE,  COLOR_RANGE_UNDEF},
#endif
#ifdef AV_PIX_FMT_GRAY12
    {CODEC_GREY_12L, AV_PIX_FMT_GRAY12LE,  COLOR_RANGE_UNDEF},
    {CODEC_GREY_12B, AV_PIX_FMT_GRAY12BE,  COLOR_RANGE_UNDEF},
#endif
    {CODEC_GREY_16L, AV_PIX_FMT_GRAY16LE,  COLOR_RANGE_UNDEF},
    {CODEC_GREY_16B, AV_PIX_FMT_GRAY16BE,  COLOR_RANGE_UNDEF},

    /* Paletized RGB */
    {CODEC_RGBP, AV_PIX_FMT_PAL8,  COLOR_RANGE_UNDEF},

    {CODEC_GBR_PLANAR, AV_PIX_FMT_GBRP,  COLOR_RANGE_UNDEF },
    {CODEC_GBR_PLANAR_9L, AV_PIX_FMT_GBRP9LE,  COLOR_RANGE_UNDEF },
    {CODEC_GBR_PLANAR_9B, AV_PIX_FMT_GBRP9BE,  COLOR_RANGE_UNDEF },
    {CODEC_GBR_PLANAR_10L, AV_PIX_FMT_GBRP10LE,  COLOR_RANGE_UNDEF },
    {CODEC_GBR_PLANAR_10B, AV_PIX_FMT_GBRP10BE,  COLOR_RANGE_UNDEF },
#ifdef AV_PIX_FMT_GBRP12 /* 55, 24, 0 / 51, 74, 100 */
    {CODEC_GBR_PLANAR_12L, AV_PIX_FMT_GBRP12LE,  COLOR_RANGE_UNDEF },
    {CODEC_GBR_PLANAR_12B, AV_PIX_FMT_GBRP12BE,  COLOR_RANGE_UNDEF },
#endif
#ifdef AV_PIX_FMT_GBRP14 /* ffmpeg only */
    {CODEC_GBR_PLANAR_14L, AV_PIX_FMT_GBRP14LE,  COLOR_RANGE_UNDEF },
    {CODEC_GBR_PLANAR_14B, AV_PIX_FMT_GBRP14BE,  COLOR_RANGE_UNDEF },
#endif
    {CODEC_GBR_PLANAR_16L, AV_PIX_FMT_GBRP16LE,  COLOR_RANGE_UNDEF },
    {CODEC_GBR_PLANAR_16B, AV_PIX_FMT_GBRP16BE,  COLOR_RANGE_UNDEF },
    {CODEC_GBRA_PLANAR, AV_PIX_FMT_GBRAP,  COLOR_RANGE_UNDEF },
#ifdef AV_PIX_FMT_GBRAP10 /* 56, 1, 0 / 55, 25, 100 */
    {CODEC_GBRA_PLANAR_10L, AV_PIX_FMT_GBRAP10LE,  COLOR_RANGE_UNDEF },
    {CODEC_GBRA_PLANAR_10B, AV_PIX_FMT_GBRAP10BE,  COLOR_RANGE_UNDEF },
#endif
#ifdef AV_PIX_FMT_GBRAP12 /* 55, 25, 0, 19, 100 */
    {CODEC_GBRA_PLANAR_12L, AV_PIX_FMT_GBRAP12LE,  COLOR_RANGE_UNDEF },
    {CODEC_GBRA_PLANAR_12B, AV_PIX_FMT_GBRAP12BE,  COLOR_RANGE_UNDEF },
#endif
    {CODEC_GBRA_PLANAR_16L, AV_PIX_FMT_GBRAP16LE,  COLOR_RANGE_UNDEF },
    {CODEC_GBRA_PLANAR_16B, AV_PIX_FMT_GBRAP16BE,  COLOR_RANGE_UNDEF },

    /* XYZ */
    {CODEC_XYZ_12L, AV_PIX_FMT_XYZ12LE,  COLOR_RANGE_UNDEF },
    {CODEC_XYZ_12B, AV_PIX_FMT_XYZ12BE,  COLOR_RANGE_UNDEF },
};

static constexpr Chroma::ChromaDescription chromaDescriptionList[] = {
    { CODEC_I411,                  PLANAR_8(3, 4, 1) },
    { CODEC_I410,                  PLANAR_8(3, 4, 4) },
    { CODEC_I420,                  PLANAR_8(3, 2, 2) },
    { CODEC_YV12,                  PLANAR_8(3, 2, 2) },
    { CODEC_NV12,                  SEMIPLANAR(2, 2, 8) },
    { CODEC_NV21,                  SEMIPLANAR(2, 2, 8) },
    { CODEC_I422,                  PLANAR_8(3, 2, 1) },
    { CODEC_NV16,                  SEMIPLANAR(2, 1, 8) },
    { CODEC_NV61,                  SEMIPLANAR(2, 1, 8) },
    { CODEC_I440,                  PLANAR_8(3, 1, 2) },
    { CODEC_I444,                  PLANAR_8(3, 1, 1) },
    { CODEC_NV24,                  SEMIPLANAR(1, 1, 8) },
    { CODEC_NV42,                  SEMIPLANAR(1, 1, 8) },
    { CODEC_YUVA,                  PLANAR_8(4, 1, 1) },
    { CODEC_YUV420A,               PLANAR_8(4, 2, 2) },
    { CODEC_YUV422A,               PLANAR_8(4, 2, 1) },

    { CODEC_GBR_PLANAR,            PLANAR_8(3, 1, 1) },
    { CODEC_GBR_PLANAR_9L,         PLANAR_16(3, 1, 1, 9) },
    { CODEC_GBR_PLANAR_9B,         PLANAR_16(3, 1, 1, 9) },
    { CODEC_GBR_PLANAR_10L,        PLANAR_16(3, 1, 1, 10) },
    { CODEC_GBR_PLANAR_10B,        PLANAR_16(3, 1, 1, 10) },
    { CODEC_GBR_PLANAR_12L,        PLANAR_16(3, 1, 1, 12) },
    { CODEC_GBR_PLANAR_12B,        PLANAR_16(3, 1, 1, 12) },
    { CODEC_GBR_PLANAR_14L,        PLANAR_16(3, 1, 1, 14) },
    { CODEC_GBR_PLANAR_14B,        PLANAR_16(3, 1, 1, 14) },
    { CODEC_GBR_PLANAR_16L,        PLANAR_16(3, 1, 1, 16) },
    { CODEC_GBR_PLANAR_16B,        PLANAR_16(3, 1, 1, 16) },
    { CODEC_GBRA_PLANAR,           PLANAR_8(4, 1, 1) },
    { CODEC_GBRA_PLANAR_10L,       PLANAR_16(4, 1, 1, 10) },
    { CODEC_GBRA_PLANAR_10B,       PLANAR_16(4, 1, 1, 10) },
    { CODEC_GBRA_PLANAR_12L,       PLANAR_16(4, 1, 1, 12) },
    { CODEC_GBRA_PLANAR_12B,       PLANAR_16(4, 1, 1, 12) },
    { CODEC_GBRA_PLANAR_16L,       PLANAR_16(4, 1, 1, 16) },
    { CODEC_GBRA_PLANAR_16B,       PLANAR_16(4, 1, 1, 16) },

    { CODEC_I420_16L,              PLANAR_16(3, 2, 2, 16) },
    { CODEC_I420_16B,              PLANAR_16(3, 2, 2, 16) },
    { CODEC_I420_12L,              PLANAR_16(3, 2, 2, 12) },
    { CODEC_I420_12B,              PLANAR_16(3, 2, 2, 12) },
    { CODEC_I420_10L,              PLANAR_16(3, 2, 2, 10) },
    { CODEC_I420_10B,              PLANAR_16(3, 2, 2, 10) },
    { CODEC_I420_9L,               PLANAR_16(3, 2, 2,  9) },
    { CODEC_I420_9B,               PLANAR_16(3, 2, 2,  9) },
    { CODEC_I422_16L,              PLANAR_16(3, 2, 1, 16) },
    { CODEC_I422_16B,              PLANAR_16(3, 2, 1, 16) },
    { CODEC_I422_12L,              PLANAR_16(3, 2, 1, 12) },
    { CODEC_I422_12B,              PLANAR_16(3, 2, 1, 12) },
    { CODEC_I422_10L,              PLANAR_16(3, 2, 1, 10) },
    { CODEC_I422_10B,              PLANAR_16(3, 2, 1, 10) },
    { CODEC_I422_9L,               PLANAR_16(3, 2, 1,  9) },
    { CODEC_I422_9B,               PLANAR_16(3, 2, 1,  9) },
    { CODEC_I444_12L,              PLANAR_16(3, 1, 1, 12) },
    { CODEC_I444_12B,              PLANAR_16(3, 1, 1, 12) },
    { CODEC_I444_10L,              PLANAR_16(3, 1, 1, 10) },
    { CODEC_I444_10B,              PLANAR_16(3, 1, 1, 10) },
    { CODEC_I444_9L,               PLANAR_16(3, 1, 1,  9) },
    { CODEC_I444_9B,               PLANAR_16(3, 1, 1,  9) },
    { CODEC_I444_16L,              PLANAR_16(3, 1, 1, 16) },
    { CODEC_I444_16B,              PLANAR_16(3, 1, 1, 16) },
    { CODEC_YUVA_444_10L,          PLANAR_16(4, 1, 1, 10) },
    { CODEC_YUVA_444_10B,          PLANAR_16(4, 1, 1, 10) },
    { CODEC_YUVA_444_12L,          PLANAR_16(4, 1, 1, 12) },
    { CODEC_YUVA_444_12B,          PLANAR_16(4, 1, 1, 12) },
    { CODEC_P010,                  SEMIPLANAR(2, 2, 10) },
    { CODEC_P016,                  SEMIPLANAR(2, 2, 16) },

    { CODEC_YUYV,                  PACKED_FMT(2, 16) },
    { CODEC_YVYU,                  PACKED_FMT(2, 16) },
    { CODEC_UYVY,                  PACKED_FMT(2, 16) },
    { CODEC_VYUY,                  PACKED_FMT(2, 16) },
    { CODEC_YUV2,                  PACKED_FMT(2, 16) },
    { CODEC_RGB233,                PACKED_FMT(1, 8) },
    { CODEC_BGR233,                PACKED_FMT(1, 8) },
    { CODEC_RGB332,                PACKED_FMT(1, 8) },
    { CODEC_YUVP,                  PACKED_FMT(1, 8) },
    { CODEC_RGBP,                  PACKED_FMT(1, 8) },
    { CODEC_GREY,                  PACKED_FMT(1, 8) },
    { CODEC_GREY_10L,              PACKED_FMT(2, 10) },
    { CODEC_GREY_10B,              PACKED_FMT(2, 10) },
    { CODEC_GREY_12L,              PACKED_FMT(2, 12) },
    { CODEC_GREY_12B,              PACKED_FMT(2, 12) },
    { CODEC_GREY_16L,              PACKED_FMT(2, 16) },
    { CODEC_GREY_16B,              PACKED_FMT(2, 16) },

    { CODEC_RGB555BE,              PACKED_FMT(2, 15) },
    { CODEC_RGB555LE,              PACKED_FMT(2, 15) },
    { CODEC_BGR555LE,              PACKED_FMT(2, 15) },
    { CODEC_BGR555BE,              PACKED_FMT(2, 15) },
    { CODEC_RGB565LE,              PACKED_FMT(2, 16) },
    { CODEC_RGB565BE,              PACKED_FMT(2, 16) },
    { CODEC_BGR565LE,              PACKED_FMT(2, 16) },
    { CODEC_BGR565BE,              PACKED_FMT(2, 16) },
    { CODEC_RGB24,                 PACKED_FMT(3, 24) },
    { CODEC_BGR24,                 PACKED_FMT(3, 24) },
    { CODEC_RGBX,                  PACKED_FMT(4, 24) },
    { CODEC_XRGB,                  PACKED_FMT(4, 24) },
    { CODEC_BGRX,                  PACKED_FMT(4, 24) },
    { CODEC_XBGR,                  PACKED_FMT(4, 24) },
    { CODEC_RGBA,                  PACKED_FMT(4, 32) },
    { CODEC_ARGB,                  PACKED_FMT(4, 32) },
    { CODEC_BGRA,                  PACKED_FMT(4, 32) },
    { CODEC_ABGR,                  PACKED_FMT(4, 32) },
    { CODEC_RGBA10LE,              PACKED_FMT(4, 32) },
    { CODEC_RGBA64,                PACKED_FMT(8, 64) },
    { CODEC_VUYA,                  PACKED_FMT(4, 32) },
    { CODEC_Y210,                  PACKED_FMT(4, 32) },
    { CODEC_Y410,                  PACKED_FMT(4, 32) },

    { CODEC_Y211,                 1, { {{1,4}, {1,1}} }, 4, 32 },
    { CODEC_XYZ_12L,               PACKED_FMT(6, 48) },
    { CODEC_XYZ_12B,               PACKED_FMT(6, 48) },

    { CODEC_VDPAU_VIDEO,           FAKE_FMT() },
    { CODEC_VDPAU_OUTPUT,          FAKE_FMT() },
    { CODEC_ANDROID_OPAQUE,        FAKE_FMT() },
    { CODEC_MMAL_OPAQUE,           FAKE_FMT() },
    { CODEC_D3D9_OPAQUE,           FAKE_FMT() },
    { CODEC_D3D11_OPAQUE,          FAKE_FMT() },
    { CODEC_D3D9_OPAQUE_10B,       FAKE_FMT() },
    { CODEC_D3D11_OPAQUE_10B,      FAKE_FMT() },
    { CODEC_D3D11_OPAQUE_RGBA,     FAKE_FMT() },
    { CODEC_D3D11_OPAQUE_BGRA,     FAKE_FMT() },
    { CODEC_D3D11_OPAQUE_ALPHA,    FAKE_FMT() },

    { CODEC_NVDEC_OPAQUE_16B,      FAKE_FMT() },
    { CODEC_NVDEC_OPAQUE_10B,      FAKE_FMT() },
    { CODEC_NVDEC_OPAQUE,          FAKE_FMT() },

    { CODEC_NVDEC_OPAQUE_444,      FAKE_FMT() },
    { CODEC_NVDEC_OPAQUE_444_16B,  FAKE_FMT() },

    { CODEC_CVPX_NV12,             FAKE_FMT() },
    { CODEC_CVPX_UYVY,             FAKE_FMT() },
    { CODEC_CVPX_I420,             FAKE_FMT() },
    { CODEC_CVPX_BGRA,             FAKE_FMT() },

    { CODEC_CVPX_P010,             FAKE_FMT() },

    { CODEC_GST_MEM_OPAQUE,        FAKE_FMT() },

    { CODEC_VAAPI_420,             FAKE_FMT() },
    { CODEC_VAAPI_420_10BPP,       FAKE_FMT() },
};

int Chroma::GetChroma(
    video_format_t* fmt,
    enum AVPixelFormat i_ffmpeg_chroma)
{
    for (size_t i = 0; i < ARRAY_SIZE(chromaTable); i++) {
        if (chromaTable[i].i_chroma_id == i_ffmpeg_chroma) {
            fmt->i_chroma = chromaTable[i].i_chroma;

            if (chromaTable[i].range != COLOR_RANGE_UNDEF)
                fmt->color_range = chromaTable[i].range;

            return SUCCESS;
        }
    }
    return EGENERIC;
}

enum AVPixelFormat Chroma::FindFfmpegChroma(
    fourcc_t fourcc,
    bool* uv_flipped)
{
    for (size_t i = 0; i < ARRAY_SIZE(chromaTable); i++)
        if (chromaTable[i].i_chroma == fourcc) {
            *uv_flipped = fourcc == CODEC_YV12;
            return chromaTable[i].i_chroma_id;
        }

    return AV_PIX_FMT_NONE;
}


const Chroma::ChromaDescription*
    Chroma::FourccGetChromaDescription(fourcc_t i_fourcc)
{
    for (size_t i = 0; i < ARRAY_SIZE(chromaDescriptionList); i++)
        if (chromaDescriptionList[i].fcc == i_fourcc)
            return &chromaDescriptionList[i];

    return nullptr;
}

__BASE_MEDIA_FFMPEG_VIDEO_NAMESPACE_END