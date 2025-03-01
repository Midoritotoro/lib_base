#include <base/media/ffmpeg/video/Picture.h>
#include <base/media/ffmpeg/video/Fourcc.h>

#include <base/media/ffmpeg/video/Chroma.h>
#include <base/system/SystemConfig.h>

__BASE_MEDIA_FFMPEG_VIDEO_NAMESPACE_BEGIN

template <typename type>
inline bool ckd_add(type* r, type a, type b)
{
    *r = a + b;
    return ((type)(a + b)) < a;
}

template <typename type>
inline bool ckd_sub(type* r, type a, type b)
{
    *r = a - b;
    return a < b;
}

template <typename type>
inline bool ckd_mul(type* r, type a, type b)
{
    if (b == 0) 
        return true;

    *r = a * b;
    return a > (INT_MAX / b);
}

Frame::~Frame() {
    clean();
}

void Frame::setupFromFormat(const video_format_t* fmt)
{
    const Chroma::ChromaDescription* p_dsc =
        Chroma::FourccGetChromaDescription(fmt->i_chroma);

    if (UNLIKELY(!p_dsc))
        return;

    i_planes = 0;

    for (unsigned i = 0; i < ARRAY_SIZE(_planesDescription); i++) {
        plane_t* p = &_planesDescription[i];
        p->p_pixels = NULL;
        p->i_pixel_pitch = 0;
    }

    VideoFormatSetup(
        &format, fmt->i_chroma,
        fmt->i_width, fmt->i_height,
        fmt->i_visible_width, fmt->i_visible_height,
        fmt->i_sar_num, fmt->i_sar_den);

    if (fmt->i_x_offset < fmt->i_width &&
        fmt->i_y_offset < fmt->i_height &&
        fmt->i_visible_width  > 0 && fmt->i_x_offset + fmt->i_visible_width <= fmt->i_width &&
        fmt->i_visible_height > 0 && fmt->i_y_offset + fmt->i_visible_height <= fmt->i_height)
        VideoFormatCopyCrop(&format, fmt);

    /* We want V (width/height) to respect:
        (V * p_dsc->p[i].w.i_num) % p_dsc->p[i].w.i_den == 0
        (V * p_dsc->p[i].w.i_num/p_dsc->p[i].w.i_den * p_dsc->i_pixel_size) % 16 == 0
        Which is respected if you have
        V % lcm( p_dsc->p[0..planes].w.i_den * 16) == 0
    */

    unsigned i_modulo_w = 1;
    unsigned i_modulo_h = 1;
    unsigned i_ratio_h = 1;

    for (unsigned i = 0; i < p_dsc->plane_count; i++)
    {
        i_modulo_w = LCM(i_modulo_w, 64 * p_dsc->p[i].w.den);
        i_modulo_h = LCM(i_modulo_h, 16 * p_dsc->p[i].h.den);
        if (i_ratio_h < p_dsc->p[i].h.den)
            i_ratio_h = p_dsc->p[i].h.den;
    }
    i_modulo_h = LCM(i_modulo_h, 32);

    unsigned width, height;

    if (UNLIKELY(ckd_add(&width, fmt->i_width, i_modulo_w - 1))
        || UNLIKELY(ckd_add(&height, fmt->i_height, i_modulo_h - 1)))
        return;

    width = width / i_modulo_w * i_modulo_w;
    height = height / i_modulo_h * i_modulo_h;

    /* plane_t uses 'int'. */
    if (UNLIKELY(width > INT_MAX) || UNLIKELY(height > INT_MAX))
        return;

    for (unsigned i = 0; i < p_dsc->plane_count; i++)
    {
        plane_t* p = &_planesDescription[i];

        const rational_t* h = &p_dsc->p[i].h;
        const rational_t* w = &p_dsc->p[i].w;

        /* A plane cannot be over-sampled. This could lead to overflow. */
        assert(h->den >= h->num);
        assert(w->den >= w->num);

        p->i_lines = height * h->num / h->den;
        p->i_visible_lines = (fmt->i_visible_height + (h->den - 1)) / h->den * h->num;

        p->i_pitch = width * w->num / w->den * p_dsc->pixel_size;
        p->i_visible_pitch = (fmt->i_visible_width + 
            (w->den - 1)) / w->den * w->num
            * p_dsc->pixel_size;

        p->i_pixel_pitch = p_dsc->pixel_size;

        assert((p->i_pitch % 64) == 0);
    }

    i_planes = p_dsc->plane_count;
}

struct picture_buffer_t
{
    int fd;
    void* base;
    size_t size;
    off_t offset;
};

void Frame::newFrameFromFormat(const video_format_t* fmt)
{
    
}

void Frame::clean() {
    free_null(_planesDescription->p_pixels);
}


__BASE_MEDIA_FFMPEG_VIDEO_NAMESPACE_END
