#pragma once

#include <base/media/ffmpeg/video/Picture.h>


#define MINIMUM_WIDTH               (32)

__BASE_MEDIA_FFMPEG_VIDEO_NAMESPACE_BEGIN


struct ScalerConfiguration {
    AVPixelFormat i_fmti;
    AVPixelFormat i_fmto;

    bool b_has_a;
    bool b_add_a;

    int  i_sws_flags;
    bool b_copy;

    bool b_swap_uvi;
    bool b_swap_uvo;
};

class FrameScaler {
    FrameScaler(Frame* frame);
    ~FrameScaler(); 

    void cleanMemory(filter_t* p_filter);

    int openScaler(filter_t* p_filter);
    void closeScaler(filter_t* p_filter);

    [[nodiscard]] int initScaler(filter_t* p_filter);
    [[nodiscard]] Frame* filter(
        filter_t* p_filter,
        Frame* p_pic);

    void getPixels(uint8_t* pp_pixel[4], int pi_pitch[4],
        const Chroma::ChromaDescription* desc,
        const video_format_t* fmt,
        const Frame* p_picture, unsigned planes,
        bool b_swap_uv);

    void convert(
        filter_t* p_filter, struct SwsContext* ctx,
        Frame* p_dst, Frame* p_src, int i_height,
        int i_plane_count, bool b_swap_uvi, bool b_swap_uvo);

    void fixParameters(
        enum AVPixelFormat* pi_fmt,
        bool* pb_has_a, fourcc_t fmt);
    [[nodiscard]] int getParameters(
        ScalerConfiguration* p_cfg,
        const video_format_t* p_fmti,
        const video_format_t* p_fmto,
        int i_sws_flags_default);

    void extractA(
        Frame* p_dst, 
        const Frame* p_src,
        unsigned offset);
    void injectA(
        Frame* p_dst,
        const Frame* p_src,
        unsigned offset);

    void fillA(
        Frame::plane_t* d,
        unsigned i_offset);
    void copyPad(
        Frame* p_dst,
        const Frame* p_src);

    void swapUV(
        Frame* p_dst,
        const Frame* p_src);
private:
    std::unique_ptr<Frame> _frame = nullptr;
};

__BASE_MEDIA_FFMPEG_VIDEO_NAMESPACE_END

