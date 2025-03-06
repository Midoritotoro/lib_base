#pragma once 

#include <base/core/BaseNamespace.h>

__BASE_MEDIA_FFMPEG_VIDEO_NAMESPACE_BEGIN

enum video_orientation_t {
    ORIENT_TOP_LEFT = 0, /**< Top line represents top, left column left. */
    ORIENT_TOP_RIGHT, /**< Flipped horizontally */
    ORIENT_BOTTOM_LEFT, /**< Flipped vertically */
    ORIENT_BOTTOM_RIGHT, /**< Rotated 180 degrees */
    ORIENT_LEFT_TOP, /**< Transposed */
    ORIENT_LEFT_BOTTOM, /**< Rotated 90 degrees anti-clockwise */
    ORIENT_RIGHT_TOP, /**< Rotated 90 degrees clockwise */
    ORIENT_RIGHT_BOTTOM, /**< Anti-transposed */
    ORIENT_NORMAL = ORIENT_TOP_LEFT,
    ORIENT_TRANSPOSED = ORIENT_LEFT_TOP,
    ORIENT_ANTI_TRANSPOSED = ORIENT_RIGHT_BOTTOM,
    ORIENT_HFLIPPED = ORIENT_TOP_RIGHT,
    ORIENT_VFLIPPED = ORIENT_BOTTOM_LEFT,
    ORIENT_ROTATED_180 = ORIENT_BOTTOM_RIGHT,
    ORIENT_ROTATED_270 = ORIENT_LEFT_BOTTOM,
    ORIENT_ROTATED_90 = ORIENT_RIGHT_TOP,
};

enum video_color_primaries_t {
    COLOR_PRIMARIES_UNDEF,
    COLOR_PRIMARIES_BT601_525,
    COLOR_PRIMARIES_BT601_625,
    COLOR_PRIMARIES_BT709,
    COLOR_PRIMARIES_BT2020,
    COLOR_PRIMARIES_DCI_P3,
    COLOR_PRIMARIES_FCC1953,
};

enum video_transfer_func_t {
    TRANSFER_FUNC_UNDEF,
    TRANSFER_FUNC_LINEAR,
    TRANSFER_FUNC_SRGB /**< Gamma 2.2 */,
    TRANSFER_FUNC_BT470_BG,
    TRANSFER_FUNC_BT470_M,
    TRANSFER_FUNC_BT709,
    TRANSFER_FUNC_SMPTE_ST2084,
    TRANSFER_FUNC_SMPTE_240,
    TRANSFER_FUNC_HLG,
};

enum video_color_space_t {
    COLOR_SPACE_UNDEF,
    COLOR_SPACE_BT601,
    COLOR_SPACE_BT709,
    COLOR_SPACE_BT2020,

};

enum video_color_range_t {
    COLOR_RANGE_UNDEF,
    COLOR_RANGE_FULL,
    COLOR_RANGE_LIMITED,

};

enum video_chroma_location_t {
    CHROMA_LOCATION_UNDEF,
    CHROMA_LOCATION_LEFT,   /**< Most common in MPEG-2 Video, H.264/265 */
    CHROMA_LOCATION_CENTER, /**< Most common in MPEG-1 Video, JPEG */
    CHROMA_LOCATION_TOP_LEFT,
    CHROMA_LOCATION_TOP_CENTER,
    CHROMA_LOCATION_BOTTOM_LEFT,
    CHROMA_LOCATION_BOTTOM_CENTER,

};

enum video_multiview_mode_t {
    MULTIVIEW_2D = 0,                   /* No stereoscopy: 2D picture. */
    MULTIVIEW_STEREO_SBS,               /* Side-by-side with left eye first. */
    MULTIVIEW_STEREO_TB,                /* Top-bottom with left eye first. */
    MULTIVIEW_STEREO_ROW,               /* Row sequential with left eye first. */
    MULTIVIEW_STEREO_COL,               /* Column sequential with left eye first. */
    MULTIVIEW_STEREO_FRAME,             /* Frame sequential with left eye first. */
    MULTIVIEW_STEREO_CHECKERBOARD,      /* Checkerboard pattern with left eye first. */
};

enum video_projection_mode_t {
    PROJECTION_MODE_RECTANGULAR = 0,
    PROJECTION_MODE_EQUIRECTANGULAR,
    PROJECTION_MODE_CUBEMAP_LAYOUT_STANDARD = 0x100,
};

enum video_context_type {
    VIDEO_CONTEXT_VAAPI = 1, //!< private: vaapi_vctx* or empty
    VIDEO_CONTEXT_VDPAU,     //!< private: chroma type (YUV) or empty (RGB)
    VIDEO_CONTEXT_DXVA2,     //!< private: d3d9_video_context_t*
    VIDEO_CONTEXT_D3D11VA,   //!< private: d3d11_video_context_t*
    VIDEO_CONTEXT_AWINDOW,   //!< private: android_video_context_t*
    VIDEO_CONTEXT_NVDEC,     //!< empty
    VIDEO_CONTEXT_CVPX,      //!< private: cvpx_video_context*
    VIDEO_CONTEXT_MMAL,      //!< empty
    VIDEO_CONTEXT_GSTDECODE, //!< empty
};

enum audio_channel_type_t {
    AUDIO_CHANNEL_TYPE_BITMAP,
    AUDIO_CHANNEL_TYPE_AMBISONICS,
};

#define MULTIVIEW_STEREO_MAX                    MULTIVIEW_STEREO_CHECKERBOARD
#define ORIENT_MAX                              ((size_t)ORIENT_RIGHT_BOTTOM)

#define COLOR_PRIMARIES_SRGB                    COLOR_PRIMARIES_BT709
#define COLOR_PRIMARIES_SMTPE_170               COLOR_PRIMARIES_BT601_525
#define COLOR_PRIMARIES_SMTPE_240               COLOR_PRIMARIES_BT601_525 /* Only differs from 1e10-4 in white Y */
#define COLOR_PRIMARIES_SMTPE_RP145             COLOR_PRIMARIES_BT601_525
#define COLOR_PRIMARIES_EBU_3213                COLOR_PRIMARIES_BT601_625
#define COLOR_PRIMARIES_BT470_BG                COLOR_PRIMARIES_BT601_625

#define COLOR_PRIMARIES_BT470_M                 COLOR_PRIMARIES_FCC1953
#define COLOR_PRIMARIES_MAX                     COLOR_PRIMARIES_FCC1953

#define TRANSFER_FUNC_BT2020                    TRANSFER_FUNC_BT709
#define TRANSFER_FUNC_SMPTE_170                 TRANSFER_FUNC_BT709
#define TRANSFER_FUNC_SMPTE_274                 TRANSFER_FUNC_BT709
#define TRANSFER_FUNC_SMPTE_293                 TRANSFER_FUNC_BT709
#define TRANSFER_FUNC_SMPTE_296                 TRANSFER_FUNC_BT709

#define TRANSFER_FUNC_ARIB_B67                  TRANSFER_FUNC_HLG
#define TRANSFER_FUNC_MAX                       TRANSFER_FUNC_HLG

#define COLOR_SPACE_SRGB                        COLOR_SPACE_BT709
#define COLOR_SPACE_SMPTE_170                   COLOR_SPACE_BT601

#define COLOR_SPACE_SMPTE_240                   COLOR_SPACE_SMPTE_170

#define COLOR_SPACE_MAX                         COLOR_SPACE_BT2020

#define COLOR_RANGE_STUDIO                      COLOR_RANGE_LIMITED
#define COLOR_RANGE_MAX                         COLOR_RANGE_LIMITED

#define CHROMA_LOCATION_MAX                     CHROMA_LOCATION_BOTTOM_CENTER

#define VIDEO_PALETTE_COLORS_MAX                256
#define VIDEO_PALETTE_CLUT_COUNT                16

#define AUDIO_REPLAY_GAIN_MAX                   (2)
#define AUDIO_REPLAY_GAIN_TRACK                 (0)

#define AUDIO_REPLAY_GAIN_ALBUM                 (1)

#define FIELD_OF_VIEW_DEGREES_DEFAULT           80.f
#define FIELD_OF_VIEW_DEGREES_MAX               150.f

#define FIELD_OF_VIEW_DEGREES_MIN               20.f


__BASE_MEDIA_FFMPEG_VIDEO_NAMESPACE_END