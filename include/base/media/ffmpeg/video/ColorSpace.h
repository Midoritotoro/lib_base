#pragma once 

#include <base/media/ffmpeg/video/Picture.h>
#include <base/media/ffmpeg/video/Chroma.h>

extern "C" {
    #include <libswscale/swscale.h>
}


namespace base::media::ffmpeg::video {
    struct filter_sys_t {
        SwsFilter* p_filter;
        int i_sws_flags;

        video_format_t fmt_in;
        video_format_t fmt_out;

        const Chroma::ChromaDescription* desc_in;
        const Chroma::ChromaDescription* desc_out;

        SwsContext* ctx;
        SwsContext* ctxA;

        Frame* p_src_a;
        Frame* p_dst_a;

        int i_extend_factor;

        Frame* p_src_e;
        Frame* p_dst_e;

        bool b_add_a;
        bool b_copy;

        bool b_swap_uvi;
        bool b_swap_uvo;
    };

    [[nodiscard]] int GetSwsColorspace(const video_format_t* format)
    {
        /* We currently map bt2020, bt709 and bt601, all other are unspecified */
        switch (format->space)
        {
        case COLOR_SPACE_BT709:
            return SWS_CS_ITU709;
        case COLOR_SPACE_BT601:
            return SWS_CS_ITU601;
        case COLOR_SPACE_BT2020:
            return SWS_CS_BT2020;
        default:
            return SWS_CS_DEFAULT;
        }
    }

    void SetColorspace(filter_sys_t* p_sys)
    {
        int input_range, output_range;
        int brightness, contrast, saturation;
        const int* input_table, * output_table;

        sws_getColorspaceDetails(p_sys->ctx, (int**)&input_table, &input_range,
            (int**)&output_table, &output_range,
            &brightness, &contrast, &saturation);

        input_range = p_sys->fmt_in.color_range == COLOR_RANGE_FULL;
        output_range = p_sys->fmt_out.color_range == COLOR_RANGE_FULL;

        input_table = sws_getCoefficients(GetSwsColorspace(&p_sys->fmt_in));
        output_table = sws_getCoefficients(GetSwsColorspace(&p_sys->fmt_out));

        sws_setColorspaceDetails(p_sys->ctx, input_table, input_range,
            output_table, output_range,
            brightness, contrast, saturation);
    }
} // namespace base::media::ffmpeg::video