#pragma once 

#include <base/media/ffmpeg/video/VideoHelper.h>
#include <base/media/ffmpeg/video/Fourcc.h>


__BASE_MEDIA_FFMPEG_VIDEO_NAMESPACE_BEGIN

struct video_palette_t {
    int i_entries;                                  /**< number of in-use palette entries */
    uint8_t palette[VIDEO_PALETTE_COLORS_MAX][4];   /**< 4-byte RGBA/YUVA palette */
};

struct viewpoint_t {
    float yaw;   /* yaw in degrees */
    float pitch; /* pitch in degrees */
    float roll;  /* roll in degrees */
    float fov;   /* field of view in degrees */
};

struct decoder_device_operations
{
    void (*close)(struct decoder_device*);
};

struct decoder_device
{
    const struct decoder_device_operations* ops;

    /** Private context that could be used by the "decoder device" module
        * implementation */
    void* sys;

    /** Must be set from the "decoder device" module open entry point */
    enum decoder_device_type type;

    /**
        * Could be set from the "decoder device" module open entry point and will
        * be used by hardware decoder modules.
        *
        * The type of pointer will depend of the type:
        * VAAPI: VADisplay
        * VDPAU: vdp_t *
        * DXVA2: d3d9_decoder_device_t*
        * D3D11VA: d3d11_decoder_device_t*
        * VIDEOTOOLBOX: NULL
        * AWindow: android AWindowHandler*
        * NVDEC: decoder_device_nvdec_t*
        * MMAL: MMAL_PORT_T*
        */
    void* opaque;
};

struct input_attachment_t
{
    char* psz_name;
    char* psz_mime;
    char* psz_description;

    size_t i_data;
    void* p_data;
};


struct decoder_device_priv
{
    struct decoder_device device;
};


struct video_format_t
{
    fourcc_t i_chroma;                               /**< picture chroma */

    unsigned int i_width;                                 /**< picture width */
    unsigned int i_height;                               /**< picture height */

    unsigned int i_x_offset;               /**< start offset of visible area */
    unsigned int i_y_offset;               /**< start offset of visible area */

    unsigned int i_visible_width;                 /**< width of visible area */
    unsigned int i_visible_height;               /**< height of visible area */

    unsigned int i_sar_num;                   /**< sample/pixel aspect ratio */
    unsigned int i_sar_den;

    unsigned int i_frame_rate;                     /**< frame rate numerator */
    unsigned int i_frame_rate_base;              /**< frame rate denominator */

    video_palette_t* p_palette;              /**< video palette from demuxer */
    video_orientation_t orientation;                /**< picture orientation */

    video_color_primaries_t primaries;                  /**< color primaries */

    video_transfer_func_t transfer;                   /**< transfer function */
    video_color_space_t space;                        /**< YCbCr color space */

    video_color_range_t color_range;            /**< 0-255 instead of 16-235 */
    video_chroma_location_t chroma_location;      /**< YCbCr chroma location */

    video_projection_mode_t projection_mode;            /**< projection mode */
    viewpoint_t pose;
    struct {
        /* similar to SMPTE ST 2086 mastering display color volume */
        uint16_t primaries[3 * 2]; /* G,B,R / x,y */
        uint16_t white_point[2]; /* x,y */

        uint32_t max_luminance;
        uint32_t min_luminance;
    } mastering;

    uint32_t i_cubemap_padding; /**< padding in pixels of the cube map faces */
};


struct subs_format_t
{
    /* the character encoding of the text of the subtitle.
        * all gettext recognized shorts can be used */
    char* psz_encoding;


    int  i_x_origin; /**< x coordinate of the subtitle. 0 = left */
    int  i_y_origin; /**< y coordinate of the subtitle. 0 = top */

    struct
    {
        /* the width of the original movie the spu was extracted from */
        unsigned i_original_frame_width;
        /* the height of the original movie the spu was extracted from */
        unsigned i_original_frame_height;

        /*  */
        uint32_t palette[VIDEO_PALETTE_CLUT_COUNT]; /* CLUT Palette AYVU */
        bool b_palette;
    } spu;

    struct
    {
        int i_id;
    } dvb;
    struct
    {
        uint8_t i_magazine; /* magazine value (3 bits), > 8 for any */
        uint8_t i_page;     /* BCD packet address value (4+4 bits) */
    } teletext;
    struct
    {
        uint8_t i_channel;
        /* Reorder depth of transport video, -1 for no reordering */
        int i_reorder_depth;
    } cc;
};

struct extra_languages_t
{
    char* psz_language;
    char* psz_description;
};

struct audio_format_t
{
    fourcc_t i_format;                          /**< audio format fourcc */
    unsigned int i_rate;                              /**< audio sample-rate */

    /* Describes the channels configuration of the samples (ie. number of
    * channels which are available in the buffer, and positions). */
    uint16_t     i_physical_channels;

    /* Describes the chan mode, either set from the input
    * (demux/codec/packetizer) or overridden by the user, used by audio
    * filters. */
    uint16_t     i_chan_mode;

    /* Channel type */
    audio_channel_type_t channel_type;

    /* Optional - for A/52, SPDIF and DTS types : */
    /* Bytes used by one compressed frame, depends on bitrate. */
    unsigned int i_bytes_per_frame;

    /* Number of sampleframes contained in one compressed frame. */
    unsigned int i_frame_length;
    /* Please note that it may be completely arbitrary - buffers are not
    * obliged to contain a integral number of so-called "frames". It's
    * just here for the division :
    * buffer_size = i_nb_samples * i_bytes_per_frame / i_frame_length
    */

    /* FIXME ? (used by the codecs) */
    unsigned     i_bitspersample;
    unsigned     i_blockalign;
    uint8_t      i_channels; /* must be <=32 */
};

struct audio_replay_gain_t
{
    /* true if we have the peak value */
    bool pb_peak[AUDIO_REPLAY_GAIN_MAX];
    /* peak value where 1.0 means full sample value */
    float      pf_peak[AUDIO_REPLAY_GAIN_MAX];

    /* true if we have the gain value */
    bool pb_gain[AUDIO_REPLAY_GAIN_MAX];
    /* gain value in dB */
    float      pf_gain[AUDIO_REPLAY_GAIN_MAX];
};

struct es_format_t
{
    enum es_format_category_e i_cat;    /**< ES category */
    fourcc_t    i_codec;            /**< FOURCC value as used in vlc */
    fourcc_t    i_original_fourcc;  /**< original FOURCC from the container */

    int             i_id;       /**< es identifier, where means
                                    -1: let the core mark the right id
                                    >=0: valid id */
    int             i_group;    /**< group identifier, where means:
                                    -1 : standalone
                                    >= 0 then a "group" (program) is created
                                        for each value */
    int             i_priority; /**< priority, where means:
                                    -2 : mean not selectable by the users
                                    -1 : mean not selected by default even
                                            when no other stream
                                    >=0: priority */

    char* psz_language;        /**< human-readable language name */
    char* psz_description;     /**< human-readable description of language */
    unsigned        i_extra_languages;    /**< length in bytes of extra language data pointer */
    extra_languages_t* p_extra_languages; /**< extra language data needed by some decoders */

    union {
        struct {
            audio_format_t  audio;    /**< description of audio format */
            audio_replay_gain_t audio_replay_gain; /**< audio replay gain information */
        };
        video_format_t video;     /**< description of video format */
        subs_format_t  subs;      /**< description of subtitle format */
    };

    unsigned int   i_bitrate; /**< bitrate of this ES */
    int      i_profile;       /**< codec specific information (like real audio flavor, mpeg audio layer, h264 profile ...) */
    int      i_level;         /**< codec specific information: indicates maximum restrictions on the stream (resolution, bitrate, codec features ...) */

    bool     b_packetized;  /**< whether the data is packetized (ie. not truncated) */
    int     i_extra;        /**< length in bytes of extra data pointer */
    void* p_extra;       /**< extra data needed by some decoders or muxers */
};

struct video_context_operations
{
    void (*destroy)(void* priv);
};

struct video_context
{
    decoder_device* device;
    const video_context_operations* ops;
    video_context_type private_type;
    size_t private_size;
};

struct frame_t;
struct frame_callbacks
{
    void (*free)(frame_t*);
};

struct frame_t
{
    frame_t* p_next;

    uint8_t* p_buffer; /**< Payload start */
    size_t      i_buffer; /**< Payload length */
    uint8_t* p_start; /**< Buffer start */
    size_t      i_size; /**< Buffer total size */

    uint32_t    i_flags;
    unsigned    i_nb_samples; /* Used for audio */

    uint64  i_pts;
    uint64  i_dts;
    uint64  i_length;


    const frame_callbacks* cbs;
};

//void DecoderDeviceRelease(decoder_device* device);
//[[nodiscard]] void* VideoContextGetPrivate(
//	video_context* vctx,
//	enum video_context_type type);
//
//void ViewpointInit(viewpoint_t* p_vp);
//void VideoFormatInit(
//	video_format_t* p_src,
//	fourcc_t i_chroma);
//
//void VideoFormatClean(video_format_t* p_src);

NODISCARD bool VideoFormatIsSameChroma(
	const video_format_t* f1,
	const video_format_t* f2);
NODISCARD bool VideoFormatIsSimilar(
	const video_format_t* f1,
	const video_format_t* f2);
//
//void VideoContextRelease(video_context* vctx);
//
void VideoFormatSetup(
	video_format_t* p_fmt, fourcc_t i_chroma,
	int i_width, int i_height,
	int i_visible_width, int i_visible_height,
	int i_sar_num, int i_sar_den);
void VideoFormatCopyCrop(
	video_format_t* p_dst,
	const video_format_t* p_src);

int VideoFormatCopy(
	video_format_t* p_dst,
	const video_format_t* p_src);

__BASE_MEDIA_FFMPEG_VIDEO_NAMESPACE_END
