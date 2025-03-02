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

void Frame::newFrameFromFormat(const video_format_t* fmt)
{
//    static_assert(offsetof(struct picture_priv_buffer_t, priv) == 0,
//        "misplaced picture_priv_t, destroy won't work");
//
//    struct picture_priv_buffer_t* privbuf = malloc(sizeof(*privbuf));
//    if (unlikely(privbuf == NULL))
//        return NULL;
//
//    picture_buffer_t* res = &privbuf->res;
//
//    picture_resource_t pic_res = {
//        .p_sys = res,
//        .pf_destroy = picture_DestroyFromFormat,
//    };
//
//    picture_priv_t* priv = &privbuf->priv;
//    if (!picture_InitPrivate(fmt, priv, &pic_res))
//        goto error;
//
//    picture_t* pic = &priv->picture;
//    if (pic->i_planes == 0) {
//        pic->p_sys = NULL; // not compatible with picture_DestroyFromFormat
//        return pic;
//    }
//
//    /* Calculate how big the new image should be */
//    assert(pic->i_planes <= PICTURE_PLANE_MAX);
//    size_t plane_sizes[PICTURE_PLANE_MAX];
//    size_t pic_size = 0;
//
//    for (int i = 0; i < pic->i_planes; i++)
//    {
//        const plane_t* p = &pic->p[i];
//
//        if (unlikely(ckd_mul(&plane_sizes[i], p->i_pitch, p->i_lines))
//            || unlikely(ckd_add(&pic_size, pic_size, plane_sizes[i])))
//            goto error;
//    }
//
//    if (unlikely(pic_size >= PICTURE_SW_SIZE_MAX))
//        goto error;
//
//    unsigned char* buf = picture_Allocate(&res->fd, pic_size);
//    if (unlikely(buf == NULL))
//        goto error;
//
//    res->base = buf;
//    res->size = pic_size;
//    res->offset = 0;
//
//    /* Fill the p_pixels field for each plane */
//    for (int i = 0; i < pic->i_planes; i++)
//    {
//        pic->p[i].p_pixels = buf;
//        buf += plane_sizes[i];
//    }
//
//    return pic;
//error:
//    video_format_Clean(&priv->picture.format);
//    free(privbuf);
//    return NULL;
}

Frame* Frame::FrameNew(
    fourcc_t i_chroma, int i_width,
    int i_height, int i_sar_num,
    int i_sar_den)
{
  //  Frame* pic = newFrameFromFormat(&p_filter->fmt_out.video);
   // return pic;
    return nullptr;
}

void Frame::clean() {
    free_null(_planesDescription->p_pixels);
}

void Frame::planeCopyPixels(
    Frame::plane_t* p_dst, 
    const Frame::plane_t * p_src)
{
    const unsigned i_width = std::min(
        p_dst->i_visible_pitch,
        p_src->i_visible_pitch);

    const unsigned i_height = std::min(
        p_dst->i_visible_lines,
        p_src->i_visible_lines);

    /* The 2x visible pitch check does two things:
       1) Makes field plane_t's work correctly (see the deinterlacer module)
       2) Moves less data if the pitch and visible pitch differ much.
    */
    if (p_src->i_pitch == p_dst->i_pitch &&
        p_src->i_pitch < 2 * p_src->i_visible_pitch)
    {
        /* There are margins, but with the same width : perfect ! */
        memcpy(p_dst->p_pixels, p_src->p_pixels,
            p_src->i_pitch * i_height);
    }
    else
    {
        /* We need to proceed line by line */
        uint8_t* p_in = p_src->p_pixels;
        uint8_t* p_out = p_dst->p_pixels;

        assert(p_in);
        assert(p_out);

        for (int i_line = i_height; i_line--; )
        {
            memcpy(p_out, p_in, i_width);
            p_in += p_src->i_pitch;
            p_out += p_dst->i_pitch;
        }
    }
}

void Frame::frameCopyProperties(
    Frame* p_dst,
    const Frame* p_src)
{
    p_dst->date = p_src->date;
    p_dst->b_force = p_src->b_force;
    p_dst->b_still = p_src->b_still;
}

void Frame::frameCopy(
    Frame* p_dst,
    const Frame* p_src)
{
    frameCopyPixels(p_dst, p_src);
    frameCopyProperties(p_dst, p_src);
}

void Frame::frameCopyPixels(
    Frame* p_dst,
    const Frame* p_src)
{
    for (int i = 0; i < p_src->i_planes; i++)
        planeCopyPixels(p_dst->_planesDescription + i, p_src->_planesDescription + i);
}


__BASE_MEDIA_FFMPEG_VIDEO_NAMESPACE_END
