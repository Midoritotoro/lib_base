#pragma once

#include <base/media/ffmpeg/video/Picture.h>
#include <base/media/ffmpeg/video/Chroma.h>


extern "C" {
    #include <libswscale/swscale.h>
}


#define MINIMUM_WIDTH               (32)

namespace base::media::ffmpeg::video {
    struct filter_t
    {
        void* p_sys;

        /* Input format */
        es_format_t         fmt_in;
        struct video_context* vctx_in;  // video filter, set by owner

        /* Output format of filter */
        es_format_t         fmt_out;
        struct video_context* vctx_out; // video filter, handled by the filter
        bool                b_allow_fmt_out_change;
    };

    struct ScalerConfiguration {
        enum AVPixelFormat i_fmti;
        enum AVPixelFormat i_fmto;

        bool b_has_a;
        bool b_add_a;

        int  i_sws_flags;
        bool b_copy;

        bool b_swap_uvi;
        bool b_swap_uvo;
    };

	void Clean(filter_t* p_filter);

    int OpenScaler(filter_t* p_filter);
    void CloseScaler(filter_t* p_filter);

    void GetPixels(uint8_t* pp_pixel[4], int pi_pitch[4],
        const Chroma::ChromaDescription* desc,
        const video_format_t* fmt,
        const picture_t* p_picture, unsigned planes,
        bool b_swap_uv);

    void Convert(filter_t* p_filter, struct SwsContext* ctx,
        picture_t* p_dst, picture_t* p_src, int i_height,
        int i_plane_count, bool b_swap_uvi, bool b_swap_uvo);

	void FixParameters(
		enum AVPixelFormat* pi_fmt,
		bool* pb_has_a, fourcc_t fmt);
	[[nodiscard]] int GetParameters(ScalerConfiguration* p_cfg,
		const video_format_t* p_fmti,
		const video_format_t* p_fmto,
		int i_sws_flags_default);

    void ExtractA(picture_t* p_dst, const picture_t* p_src,
        unsigned offset);
    void InjectA(picture_t* p_dst, const picture_t* p_src,
        unsigned offset);

    void FillA(plane_t* d, unsigned i_offset);
    void CopyPad(picture_t* p_dst, const picture_t* p_src);

    void SwapUV(
        picture_t* p_dst,
        const picture_t* p_src);

	[[nodiscard]] int Init(filter_t* p_filter);
    [[nodiscard]] picture_t* Filter(
        filter_t* p_filter,
        picture_t* p_pic);
} // namespace base::media::ffmpeg::video