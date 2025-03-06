#pragma once 

#include <base/media/ffmpeg/video/Picture.h>
#include <base/media/ffmpeg/video/Chroma.h>

extern "C" {
    #include <libswscale/swscale.h>
}


__BASE_MEDIA_FFMPEG_VIDEO_NAMESPACE_BEGIN

NODISCARD int GetSwsColorspace(const video_format_t* format)
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

__BASE_MEDIA_FFMPEG_VIDEO_NAMESPACE_END