#include <base/media/ffmpeg/video/Picture.h>

#include <base/concurrent/common/CommonAncillary.h>
#include <base/media/ffmpeg/video/Fourcc.h>

#include <base/media/ffmpeg/video/Object.h>
#include <base/media/ffmpeg/video/Config.h>

#include <base/media/ffmpeg/video/Chroma.h>


namespace base::media::ffmpeg::video {
    picture_t* PictureNew(
        fourcc_t i_chroma, int i_width,
        int i_height, int i_sar_num,
        int i_sar_den)
    {
        video_format_t fmt;

        VideoFormatInit(&fmt, 0);
        VideoFormatSetup(&fmt, i_chroma, i_width, i_height,
            i_width, i_height, i_sar_num, i_sar_den);

        return PictureNewFromFormat(&fmt);
    }

    void PictureDestroy(picture_t* picture)
    {
        Assert(AtomicRcDec(&picture->refs) == 0);

        PictureDestroyContext(picture);

        picture_priv_t* priv = container_of(picture, picture_priv_t, picture);
        assert(priv->gc.destroy != NULL);

        priv->gc.destroy(picture);

        Threads::AncillaryArrayClear(&priv->ancillaries);
        VideoFormatClean(&picture->format);
        free(priv);
    }

    void PictureRelease(picture_t* picture)
    {
        if (Threads::AtomicRcDec(&picture->refs))
            PictureDestroy(picture);
    }

    void PictureDestroyContext(picture_t* p_picture)
    {
        picture_context_t* ctx = p_picture->context;
        if (ctx != NULL)
        {
            video_context* vctx = ctx->vctx;
            ctx->destroy(ctx);
            if (vctx)
                VideoContextRelease(vctx);
            p_picture->context = NULL;
        }
    }


    void PictureDestroyFromFormat(picture_t* pic)
    {
        picture_buffer_t* res = (picture_buffer_t*)pic->p_sys;

        if (res != NULL)
            PictureDeallocate(res->fd, res->base, res->size);
    }

    void* PictureAllocate(int* fdp, size_t size)
    {
        assert((size % 64) == 0);
        *fdp = -1;
        return _aligned_malloc(64, size);
    }

    void PictureDestroyDummy(picture_t* p_picture) {
        (void)p_picture;
    }

    void PictureDeallocate(int fd, void* base, size_t size)
    {
        assert(fd == -1);
        aligned_free(base);
        assert((size % 64) == 0);
    }

    int PictureSetup(picture_t* p_picture, const video_format_t* fmt)
    {
        const chroma_description_t* p_dsc =
            FourccGetChromaDescription(fmt->i_chroma);
        if (unlikely(!p_dsc))
            return EGENERIC;

        /* Store default values */
        p_picture->i_planes = 0;
        for (unsigned i = 0; i < ARRAY_SIZE(p_picture->p); i++)
        {
            plane_t* p = &p_picture->p[i];
            p->p_pixels = NULL;
            p->i_pixel_pitch = 0;
        }

        p_picture->i_nb_fields = 2;

        VideoFormatSetup(&p_picture->format, fmt->i_chroma, fmt->i_width, fmt->i_height,
            fmt->i_visible_width, fmt->i_visible_height,
            fmt->i_sar_num, fmt->i_sar_den);
        if (fmt->i_x_offset < fmt->i_width &&
            fmt->i_y_offset < fmt->i_height &&
            fmt->i_visible_width  > 0 && fmt->i_x_offset + fmt->i_visible_width <= fmt->i_width &&
            fmt->i_visible_height > 0 && fmt->i_y_offset + fmt->i_visible_height <= fmt->i_height)
            VideoFormatCopyCrop(&p_picture->format, fmt);

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

        if (unlikely(ckd_add(&width, fmt->i_width, i_modulo_w - 1))
            || unlikely(ckd_add(&height, fmt->i_height, i_modulo_h - 1)))
            return EGENERIC;

        width = width / i_modulo_w * i_modulo_w;
        height = height / i_modulo_h * i_modulo_h;

        /* plane_t uses 'int'. */
        if (unlikely(width > INT_MAX) || unlikely(height > INT_MAX))
            return EGENERIC;

        for (unsigned i = 0; i < p_dsc->plane_count; i++)
        {
            plane_t* p = &p_picture->p[i];
            const rational_t* h = &p_dsc->p[i].h;
            const rational_t* w = &p_dsc->p[i].w;

            /* A plane cannot be over-sampled. This could lead to overflow. */
            assert(h->den >= h->num);
            assert(w->den >= w->num);

            p->i_lines = height * h->num / h->den;
            p->i_visible_lines = (fmt->i_visible_height + (h->den - 1)) / h->den * h->num;

            p->i_pitch = width * w->num / w->den * p_dsc->pixel_size;
            p->i_visible_pitch = (fmt->i_visible_width + (w->den - 1)) / w->den * w->num
                * p_dsc->pixel_size;
            p->i_pixel_pitch = p_dsc->pixel_size;

            assert((p->i_pitch % 64) == 0);
        }
        p_picture->i_planes = p_dsc->plane_count;

        return SUCCESS;
    }

    bool PictureInitPrivate(
        const video_format_t* p_fmt,
        picture_priv_t* priv,
        const picture_resource_t* p_resource)
    {
        picture_t* p_picture = &priv->picture;

        memset(p_picture, 0, sizeof(*p_picture));
        p_picture->date = TICK_INVALID;

        VideoFormatCopy(&p_picture->format, p_fmt);
        /* Make sure the real dimensions are a multiple of 16 */
        if (PictureSetup(p_picture, p_fmt))
        {
            VideoFormatClean(&p_picture->format);
            return false;
        }

        Threads::AtomicRcInit(&p_picture->refs);
        priv->gc.opaque = NULL;

        p_picture->p_sys = p_resource->p_sys;

        if (p_resource->pf_destroy != NULL)
            priv->gc.destroy = p_resource->pf_destroy;
        else
            priv->gc.destroy = PictureDestroyDummy;

        Threads::AncillaryArrayInit(&priv->ancillaries);

        return true;
    }


    picture_t* PictureNewFromFormat(const video_format_t* fmt)
    {
        static_assert(offsetof(struct picture_priv_buffer_t, priv) == 0,
            "misplaced picture_priv_t, destroy won't work");

        struct picture_priv_buffer_t* privbuf = (picture_priv_buffer_t*)malloc(sizeof(*privbuf));
        if (UNLIKELY(privbuf == NULL))
            return NULL;

        picture_buffer_t* res = &privbuf->res;

        picture_resource_t pic_res = {
            .p_sys = res,
            .pf_destroy = PictureDestroyFromFormat,
        };

        picture_priv_t* priv = &privbuf->priv;

        const auto error = [=] {
            VideoFormatClean(&priv->picture.format);
            free(privbuf);
           };

        if (!PictureInitPrivate(fmt, priv, &pic_res)) {
            error();
            return NULL;
        }

        picture_t* pic = &priv->picture;
        if (pic->i_planes == 0) {
            pic->p_sys = NULL; // not compatible with picture_DestroyFromFormat
            return pic;
        }

        /* Calculate how big the new image should be */
        assert(pic->i_planes <= PICTURE_PLANE_MAX);
        size_t plane_sizes[PICTURE_PLANE_MAX];
        size_t pic_size = 0;

        for (int i = 0; i < pic->i_planes; i++)
        {
            const plane_t* p = &pic->p[i];

            if (UNLIKELY(ckd_mul(&plane_sizes[i], static_cast<size_t>(p->i_pitch), static_cast<size_t>(p->i_lines)))
                || (UNLIKELY(ckd_add(&pic_size, pic_size, plane_sizes[i])))) {
                error();
                return NULL;
            }
                
        }

        if (UNLIKELY(pic_size >= PICTURE_SW_SIZE_MAX)) {
            error();
            return NULL;
        }

        unsigned char* buf = (uchar*)PictureAllocate(&res->fd, pic_size);
        if (UNLIKELY(buf == NULL))
        {
            error();
            return NULL;
        }

        res->base = buf;
        res->size = pic_size;
        res->offset = 0;

        /* Fill the p_pixels field for each plane */
        for (int i = 0; i < pic->i_planes; i++)
        {
            pic->p[i].p_pixels = buf;
            buf += plane_sizes[i];
        }

        return pic;
    }

    void PlaneCopyPixels(plane_t* p_dst, const plane_t* p_src)
    {
        const unsigned i_width = std::min(p_dst->i_visible_pitch,
            p_src->i_visible_pitch);
        const unsigned i_height = std::min(p_dst->i_visible_lines,
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

    void PictureCopyProperties(picture_t* p_dst, const picture_t* p_src)
    {
        p_dst->date = p_src->date;
        p_dst->b_force = p_src->b_force;
        p_dst->b_still = p_src->b_still;

        p_dst->b_progressive = p_src->b_progressive;
        p_dst->i_nb_fields = p_src->i_nb_fields;
        p_dst->b_top_field_first = p_src->b_top_field_first;

        const picture_priv_t* src_priv = container_of(p_src, picture_priv_t, picture);
        picture_priv_t* dst_priv = container_of(p_dst, picture_priv_t, picture);

        Threads::AncillaryArrayDup(&dst_priv->ancillaries, &src_priv->ancillaries);
    }

    void PictureCopy(
        picture_t* p_dst,
        const picture_t* p_src)
    {
        PictureCopyPixels(p_dst, p_src);
        PictureCopyProperties(p_dst, p_src);
    }

    void PictureCopyPixels(
        picture_t* p_dst,
        const picture_t* p_src)
    {
        for (int i = 0; i < p_src->i_planes; i++)
            PlaneCopyPixels(p_dst->p + i, p_src->p + i);

        assert(p_dst->context == NULL);

        if (p_src->context != NULL)
            p_dst->context = p_src->context->copy(p_src->context);
    }
} // namespace base::media::ffmpeg::video