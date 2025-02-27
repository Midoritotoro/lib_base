#include <base/media/ffmpeg/video/Scale.h>

#include <cassert>
#include <cstdlib>
#include <malloc.h>

#include <base/system/SystemInfo.h>
#include <base/media/ffmpeg/video/ColorSpace.h>

extern "C" {
    #include <libswscale/version.h>
}

#include <qDebug>

#define ALLOW_YUVP  (false)
#define OFFSET_A    (3)


namespace base::media::ffmpeg::video {
    void Clean(filter_t* p_filter)
    {
        filter_sys_t* p_sys = (filter_sys_t*)p_filter->p_sys;

        if (p_sys->p_src_e)
            PictureRelease(p_sys->p_src_e);
        if (p_sys->p_dst_e)
            PictureRelease(p_sys->p_dst_e);

        if (p_sys->p_src_a)
            PictureRelease(p_sys->p_src_a);
        if (p_sys->p_dst_a)
            PictureRelease(p_sys->p_dst_a);

        if (p_sys->ctxA)
            sws_freeContext(p_sys->ctxA);

        if (p_sys->ctx)
            sws_freeContext(p_sys->ctx);

        /* We have to set it to null has we call be called again :( */
        p_sys->ctx = NULL;
        p_sys->ctxA = NULL;

        p_sys->p_src_a = NULL;
        p_sys->p_dst_a = NULL;

        p_sys->p_src_e = NULL;
        p_sys->p_dst_e = NULL;
    }

    int OpenScaler(filter_t* p_filter)
    {
        filter_sys_t* p_sys = nullptr;
        int i_sws_mode = -1;

        if (GetParameters(nullptr,
            &p_filter->fmt_in.video,
            &p_filter->fmt_out.video, 0))
            return EGENERIC;

        /* Allocate the memory needed to store the decoder's structure */
        if ((p_filter->p_sys = p_sys = (filter_sys_t*)calloc(1, sizeof(filter_sys_t))) == NULL)
            return ENOMEM;

        switch (i_sws_mode) {
            case 0:  
                p_sys->i_sws_flags = SWS_FAST_BILINEAR; 
                break;

            case 1: 
                p_sys->i_sws_flags = SWS_BILINEAR;
                break;

            case 2:  
                p_sys->i_sws_flags = SWS_BICUBIC; 
                break;

            case 3:  
                p_sys->i_sws_flags = SWS_X; 
                break;

            case 4: 
                p_sys->i_sws_flags = SWS_POINT;
                break;

            case 5:  
                p_sys->i_sws_flags = SWS_AREA;
                break;

            case 6:  
                p_sys->i_sws_flags = SWS_BICUBLIN;
                break;

            case 7:  
                p_sys->i_sws_flags = SWS_GAUSS; 
                break;

            case 8: 
                p_sys->i_sws_flags = SWS_SINC;
                break;

            case 9:  
                p_sys->i_sws_flags = SWS_LANCZOS;
                break;

            case 10: 
                p_sys->i_sws_flags = SWS_SPLINE;
                break;

            default:
                p_sys->i_sws_flags = SWS_BICUBIC; 
                i_sws_mode = 2;
                break;
        }

        /* Misc init */
        memset(&p_sys->fmt_in, 0, sizeof(p_sys->fmt_in));
        memset(&p_sys->fmt_out, 0, sizeof(p_sys->fmt_out));

        if (Init(p_filter))
        {
            if (p_sys->p_filter)
                sws_freeFilter(p_sys->p_filter);
            free(p_sys);
            return EGENERIC;
        }

        /* */
        p_filter->ops = &filter_ops;

       /* msg_Dbg(p_filter, "%ix%i (%ix%i) chroma: %4.4s colorspace: %s -> %ix%i (%ix%i) chroma: %4.4s colorspace: %s with scaling using %s",
            p_filter->fmt_in.video.i_visible_width, p_filter->fmt_in.video.i_visible_height,
            p_filter->fmt_in.video.i_width, p_filter->fmt_in.video.i_height,
            (char*)&p_filter->fmt_in.video.i_chroma, GetColorspaceName(p_filter->fmt_in.video.space),
            p_filter->fmt_out.video.i_visible_width, p_filter->fmt_out.video.i_visible_height,
            p_filter->fmt_out.video.i_width, p_filter->fmt_out.video.i_height,
            (char*)&p_filter->fmt_out.video.i_chroma, GetColorspaceName(p_filter->fmt_out.video.space),
            ppsz_mode_descriptions[i_sws_mode]);*/

        return SUCCESS;
    }

    void CloseScaler(filter_t* p_filter)
    {
        filter_sys_t* p_sys = (filter_sys_t*)p_filter->p_sys;

        Clean(p_filter);
        if (p_sys->p_filter)
            sws_freeFilter(p_sys->p_filter);
        free(p_sys);
    }

    void GetPixels(uint8_t* pp_pixel[4], int pi_pitch[4],
        const Chroma::ChromaDescription* desc,
        const video_format_t* fmt,
        const picture_t* p_picture, unsigned planes,
        bool b_swap_uv)
    {
        unsigned i = 0;

        if (planes > (unsigned)p_picture->i_planes)
            planes = p_picture->i_planes;
        assert(!b_swap_uv || planes >= 3);

        for (; i < planes; i++)
        {
            const plane_t* p = p_picture->p + i;
            if (b_swap_uv && (i == 1 || i == 2))
                p = p_picture->p + 3 - i;

            pp_pixel[i] = p->p_pixels
                + (((fmt->i_x_offset * desc->p[i].w.num) / desc->p[i].w.den)
                    * p->i_pixel_pitch)
                + (((fmt->i_y_offset * desc->p[i].h.num) / desc->p[i].h.den)
                    * p->i_pitch);
            pi_pitch[i] = p->i_pitch;
        }

        for (; i < 4; i++)
        {
            pp_pixel[i] = NULL;
            pi_pitch[i] = 0;
        }
    }


    void Convert(filter_t* p_filter, struct SwsContext* ctx,
        picture_t* p_dst, picture_t* p_src, int i_height,
        int i_plane_count, bool b_swap_uvi, bool b_swap_uvo)
    {
        filter_sys_t* p_sys = (filter_sys_t*)p_filter->p_sys;
        uint8_t palette[AVPALETTE_SIZE];
        uint8_t* src[4], * dst[4];
        const uint8_t* csrc[4];
        int src_stride[4], dst_stride[4];

        GetPixels(src, src_stride, p_sys->desc_in, &p_filter->fmt_in.video,
            p_src, i_plane_count, b_swap_uvi);
        if (p_filter->fmt_in.video.i_chroma == CODEC_RGBP)
        {
            if (p_filter->fmt_in.video.p_palette)
            {
                const video_palette_t* p_palette = p_filter->fmt_in.video.p_palette;
                static_assert(sizeof(p_palette->palette) == AVPALETTE_SIZE,
                    "Palette size mismatch between vlc and libavutil");
                uint8_t* dstp = palette;
                for (int i = 0; i < p_palette->i_entries; i++)
                {
                    // we want ARGB in host endianess from RGBA in byte order
#if system::SystemInfo::ByteOrder == system::SystemInfo::BigEndian
                    dstp[0] = p_palette->palette[i][3];
                    dstp[1] = p_palette->palette[i][0];
                    dstp[2] = p_palette->palette[i][1];
                    dstp[3] = p_palette->palette[i][2];
#else
                    dstp[0] = p_palette->palette[i][2];
                    dstp[1] = p_palette->palette[i][1];
                    dstp[2] = p_palette->palette[i][0];
                    dstp[3] = p_palette->palette[i][3];
#endif
                    dstp += sizeof(p_palette->palette[0]);
                }
            }
            else
            {
                memset(&palette, 0, sizeof(palette));
            }
            src[1] = palette;
            src_stride[1] = 4;
        }

        GetPixels(dst, dst_stride, p_sys->desc_out, &p_filter->fmt_out.video,
            p_dst, i_plane_count, b_swap_uvo);

        for (size_t i = 0; i < ARRAY_SIZE(src); i++)
            csrc[i] = src[i];

#if LIBSWSCALE_VERSION_INT  >= ((0<<16)+(5<<8)+0)
        sws_scale(ctx, csrc, src_stride, 0, i_height,
            dst, dst_stride);
#else
        sws_scale_ordered(ctx, csrc, src_stride, 0, i_height,
            dst, dst_stride);
#endif
    }


    void FixParameters(enum AVPixelFormat* pi_fmt, bool* pb_has_a, fourcc_t fmt)
    {
        switch (fmt)
        {
        case CODEC_YUV422A:
            *pi_fmt = AV_PIX_FMT_YUVA422P;
            *pb_has_a = true;
            break;
        case CODEC_YUV420A:
            *pi_fmt = AV_PIX_FMT_YUVA420P;
            *pb_has_a = true;
            break;
        case CODEC_YUVA:
            *pi_fmt = AV_PIX_FMT_YUV444P;
            *pb_has_a = true;
            break;
        case CODEC_RGBA:
            *pi_fmt = AV_PIX_FMT_RGBA;
            *pb_has_a = true;
            break;
        case CODEC_ARGB:
            *pi_fmt = AV_PIX_FMT_ARGB;
            *pb_has_a = true;
            break;
        case CODEC_BGRA:
            *pi_fmt = AV_PIX_FMT_BGRA;
            *pb_has_a = true;
            break;
        case CODEC_ABGR:
            *pi_fmt = AV_PIX_FMT_ABGR;
            *pb_has_a = true;
            break;
        default:
            break;
        }
    }

    int GetParameters(ScalerConfiguration* p_cfg,
        const video_format_t* p_fmti,
        const video_format_t* p_fmto,
        int i_sws_flags_default)
    {
        enum AVPixelFormat i_fmti;
        enum AVPixelFormat i_fmto;

        bool b_has_ai = false;
        bool b_has_ao = false;

        int i_sws_flags = i_sws_flags_default;

        bool b_swap_uvi = false;
        bool b_swap_uvo = false;

        i_fmti = Chroma::FindFfmpegChroma(p_fmti->i_chroma, &b_swap_uvi);
        i_fmto = Chroma::FindFfmpegChroma(p_fmto->i_chroma, &b_swap_uvo);

        if (p_fmti->i_chroma == p_fmto->i_chroma)
        {
            if (p_fmti->i_chroma == CODEC_YUVP && ALLOW_YUVP)
            {
                i_fmti = i_fmto = AV_PIX_FMT_GRAY8;
                i_sws_flags = SWS_POINT;
            }
        }

        FixParameters(&i_fmti, &b_has_ai, p_fmti->i_chroma);
        FixParameters(&i_fmto, &b_has_ao, p_fmto->i_chroma);

#if !defined (__ANDROID__) && !defined(TARGET_OS_IPHONE)
        /* FIXME TODO removed when ffmpeg is fixed
         * Without SWS_ACCURATE_RND the quality is really bad for some conversions */
        switch (i_fmto)
        {
        case AV_PIX_FMT_ARGB:
        case AV_PIX_FMT_RGBA:
        case AV_PIX_FMT_ABGR:
            i_sws_flags |= SWS_ACCURATE_RND;
            break;
        default:
            break;
        }
#endif

        if (p_cfg)
        {
            p_cfg->i_fmti = i_fmti;
            p_cfg->i_fmto = i_fmto;
            p_cfg->b_has_a = b_has_ai && b_has_ao;
            p_cfg->b_add_a = (!b_has_ai) && b_has_ao;
            p_cfg->b_copy = i_fmti == i_fmto &&
                p_fmti->i_visible_width == p_fmto->i_visible_width &&
                p_fmti->i_visible_height == p_fmto->i_visible_height;
            p_cfg->b_swap_uvi = b_swap_uvi;
            p_cfg->b_swap_uvo = b_swap_uvo;
            p_cfg->i_sws_flags = i_sws_flags;
        }

        if (i_fmti < 0 || i_fmto < 0)
            return EGENERIC;

        return SUCCESS;
    }

    void ExtractA(picture_t* p_dst, const picture_t* p_src,
        unsigned offset)
    {
        plane_t* d = &p_dst->p[0];
        const plane_t* s = &p_src->p[0];

        for (unsigned y = 0; y < p_dst->format.i_height; y++)
            for (unsigned x = 0; x < p_dst->format.i_width; x++)
                d->p_pixels[y * d->i_pitch + x] = s->p_pixels[y * s->i_pitch + 4 * x + offset];
    }

    void InjectA(picture_t* p_dst, const picture_t* p_src,
        unsigned offset)
    {
        plane_t* d = &p_dst->p[0];
        const plane_t* s = &p_src->p[0];

        for (unsigned y = 0; y < p_src->format.i_height; y++)
            for (unsigned x = 0; x < p_src->format.i_width; x++)
                d->p_pixels[y * d->i_pitch + 4 * x + offset] = s->p_pixels[y * s->i_pitch + x];
    }

    void FillA(plane_t* d, unsigned i_offset)
    {
        for (int y = 0; y < d->i_visible_lines; y++)
            for (int x = 0; x < d->i_visible_pitch; x += d->i_pixel_pitch)
                d->p_pixels[y * d->i_pitch + x + i_offset] = 0xff;
    }

    void CopyPad(picture_t* p_dst, const picture_t* p_src)
    {
        PictureCopy(p_dst, p_src);
        for (int n = 0; n < p_dst->i_planes; n++)
        {
            const plane_t* s = &p_src->p[n];
            plane_t* d = &p_dst->p[n];

            for (int y = 0; y < s->i_lines && y < d->i_lines; y++)
            {
                for (int x = s->i_visible_pitch; x < d->i_visible_pitch; x += s->i_pixel_pitch)
                    memcpy(&d->p_pixels[y * d->i_pitch + x], &d->p_pixels[y * d->i_pitch + s->i_visible_pitch - s->i_pixel_pitch], s->i_pixel_pitch);
            }
        }
    }

    void SwapUV(
        picture_t* p_dst,
        const picture_t* p_src)
    {
        picture_t tmp = *p_src;

        tmp.p[1] = p_src->p[2];
        tmp.p[2] = p_src->p[1];

        PictureCopyPixels(p_dst, &tmp);
    }


    int Init(filter_t* p_filter)
    {
        filter_sys_t* p_sys = (filter_sys_t*)p_filter->p_sys;

        const video_format_t* p_fmti = &p_filter->fmt_in.video;
        video_format_t* p_fmto = &p_filter->fmt_out.video;

        if (p_fmti->orientation != p_fmto->orientation)
            return EGENERIC;

        if (VideoFormatIsSimilar(p_fmti, &p_sys->fmt_in) &&
            VideoFormatIsSimilar(p_fmto, &p_sys->fmt_out) &&
            p_sys->ctx)
        {
            return SUCCESS;
        }

        Clean(p_filter);

        /* Init with new parameters */
        ScalerConfiguration cfg;
        if (GetParameters(&cfg, p_fmti, p_fmto, p_sys->i_sws_flags))
        {
            qDebug() << QString("format not supported '%1s' %2x%3 -> '%4s' %5x%6").arg(
                (const char*)&p_fmti->i_chroma).arg(p_fmti->i_visible_width).arg(p_fmti->i_visible_height).arg(
                (const char*)&p_fmto->i_chroma).arg(p_fmto->i_visible_width).arg(p_fmto->i_visible_height);

            return EGENERIC;
        }
        if (p_fmti->i_visible_width == 0 || p_fmti->i_visible_height == 0 ||
            p_fmto->i_visible_width == 0 || p_fmto->i_visible_height == 0)
        {
            qDebug() << QString("invalid scaling: %1x%2 -> %3x%4").arg(
                p_fmti->i_visible_width, p_fmti->i_visible_height).arg(
                p_fmto->i_visible_width, p_fmto->i_visible_height);
            return EGENERIC;
        }

        p_sys->desc_in = Chroma::FourccGetChromaDescription(p_fmti->i_chroma);
        p_sys->desc_out = Chroma::FourccGetChromaDescription(p_fmto->i_chroma);
        if (p_sys->desc_in == NULL || p_sys->desc_out == NULL)
            return EGENERIC;

        if (p_sys->desc_in->plane_count == 0 || p_sys->desc_out->plane_count == 0)
            return EGENERIC;

        /* swscale does not like too small width */
        p_sys->i_extend_factor = 1;
        while (std::min(p_fmti->i_visible_width, p_fmto->i_visible_width) * p_sys->i_extend_factor < MINIMUM_WIDTH)
            p_sys->i_extend_factor++;

        const unsigned i_fmti_visible_width = p_fmti->i_visible_width * p_sys->i_extend_factor;
        const unsigned i_fmto_visible_width = p_fmto->i_visible_width * p_sys->i_extend_factor;
        for (int n = 0; n < (cfg.b_has_a ? 2 : 1); n++)
        {
            const AVPixelFormat i_fmti = n == 0 ? cfg.i_fmti : AV_PIX_FMT_GRAY8;
            const AVPixelFormat i_fmto = n == 0 ? cfg.i_fmto : AV_PIX_FMT_GRAY8;
            struct SwsContext* ctx;

            ctx = sws_getContext(i_fmti_visible_width, p_fmti->i_visible_height, i_fmti,
                i_fmto_visible_width, p_fmto->i_visible_height, i_fmto,
                cfg.i_sws_flags,
                p_sys->p_filter, NULL, 0);
            if (n == 0)
                p_sys->ctx = ctx;
            else
                p_sys->ctxA = ctx;
        }
        if (p_sys->ctxA)
        {
            p_sys->p_src_a = PictureNew(CODEC_GREY, i_fmti_visible_width, p_fmti->i_visible_height, 0, 1);
            p_sys->p_dst_a = PictureNew(CODEC_GREY, i_fmto_visible_width, p_fmto->i_visible_height, 0, 1);
        }
        if (p_sys->i_extend_factor != 1)
        {
            p_sys->p_src_e = PictureNew(p_fmti->i_chroma, i_fmti_visible_width, p_fmti->i_visible_height, 0, 1);
            p_sys->p_dst_e = PictureNew(p_fmto->i_chroma, i_fmto_visible_width, p_fmto->i_visible_height, 0, 1);

            if (p_sys->p_src_e)
                memset(p_sys->p_src_e->p[0].p_pixels, 0, p_sys->p_src_e->p[0].i_pitch * p_sys->p_src_e->p[0].i_lines);
            if (p_sys->p_dst_e)
                memset(p_sys->p_dst_e->p[0].p_pixels, 0, p_sys->p_dst_e->p[0].i_pitch * p_sys->p_dst_e->p[0].i_lines);
        }

        if (!p_sys->ctx ||
            (cfg.b_has_a && (!p_sys->ctxA || !p_sys->p_src_a || !p_sys->p_dst_a)) ||
            (p_sys->i_extend_factor != 1 && (!p_sys->p_src_e || !p_sys->p_dst_e)))
        {
            qDebug() << "could not init SwScaler and/or allocate memory";
            Clean(p_filter);
            return EGENERIC;
        }

        if (p_filter->b_allow_fmt_out_change)
        {
            /*
             * If the transformation is not homothetic we must modify the
             * aspect ratio of the output format in order to have the
             * output picture displayed correctly and not stretched
             * horizontally or vertically.
             * WARNING: this is a hack, ideally this should not be needed
             * and the vout should update its video format instead.
             */
            unsigned i_sar_num = p_fmti->i_sar_num * p_fmti->i_visible_width;
            unsigned i_sar_den = p_fmti->i_sar_den * p_fmto->i_visible_width;
            UnsignedReduce(&i_sar_num, &i_sar_den, i_sar_num, i_sar_den, 65536);
            i_sar_num *= p_fmto->i_visible_height;
            i_sar_den *= p_fmti->i_visible_height;
            UnsignedReduce(&i_sar_num, &i_sar_den, i_sar_num, i_sar_den, 65536);
            p_fmto->i_sar_num = i_sar_num;
            p_fmto->i_sar_den = i_sar_den;
        }

        p_sys->b_add_a = cfg.b_add_a;
        p_sys->b_copy = cfg.b_copy;
        p_sys->fmt_in = *p_fmti;
        p_sys->fmt_out = *p_fmto;
        p_sys->b_swap_uvi = cfg.b_swap_uvi;
        p_sys->b_swap_uvo = cfg.b_swap_uvo;

        SetColorspace(p_sys);

        return SUCCESS;
    }

    picture_t* Filter(filter_t* p_filter, picture_t* p_pic)
    {
        filter_sys_t* p_sys = (filter_sys_t*)p_filter->p_sys;
        const video_format_t* p_fmti = &p_filter->fmt_in.video;
        const video_format_t* p_fmto = &p_filter->fmt_out.video;
        picture_t* p_pic_dst;

        /* Check if format properties changed */
        if (Init(p_filter))
        {
            PictureRelease(p_pic);
            return NULL;
        }

        /* Request output picture */
        p_pic_dst = FilterNewPicture(p_filter);
        if (!p_pic_dst)
        {
            PictureRelease(p_pic);
            return NULL;
        }

        /* */
        picture_t* p_src = p_pic;
        picture_t* p_dst = p_pic_dst;
        if (p_sys->i_extend_factor != 1)
        {
            p_src = p_sys->p_src_e;
            p_dst = p_sys->p_dst_e;

            CopyPad(p_src, p_pic);
        }

        if (p_sys->b_copy && p_sys->b_swap_uvi == p_sys->b_swap_uvo)
            PictureCopyPixels(p_dst, p_src);
        else if (p_sys->b_copy)
            SwapUV(p_dst, p_src);
        else
        {
            /* Even if alpha is unused, swscale expects the pointer to be set */
            const int n_planes = !p_sys->ctxA && (p_src->i_planes == 4 ||
                p_dst->i_planes == 4) ? 4 : 3;
            Convert(p_filter, p_sys->ctx, p_dst, p_src, p_fmti->i_visible_height,
                n_planes, p_sys->b_swap_uvi, p_sys->b_swap_uvo);
        }
        if (p_sys->ctxA)
        {
            /* We extract the A plane to rescale it, and then we reinject it. */
            if (p_fmti->i_chroma == CODEC_RGBA || p_fmti->i_chroma == CODEC_BGRA)
                ExtractA(p_sys->p_src_a, p_src, OFFSET_A);
            else if (p_fmti->i_chroma == CODEC_ARGB || p_fmti->i_chroma == CODEC_ABGR)
                ExtractA(p_sys->p_src_a, p_src, 0);
            else
                PlaneCopyPixels(p_sys->p_src_a->p, p_src->p + A_PLANE);

            Convert(p_filter, p_sys->ctxA, p_sys->p_dst_a, p_sys->p_src_a,
                p_fmti->i_visible_height, 1, false, false);
            if (p_fmto->i_chroma == CODEC_RGBA || p_fmto->i_chroma == CODEC_BGRA)
                InjectA(p_dst, p_sys->p_dst_a, OFFSET_A);
            else if (p_fmto->i_chroma == CODEC_ARGB || p_fmto->i_chroma == CODEC_ABGR)
                InjectA(p_dst, p_sys->p_dst_a, 0);
            else
                PlaneCopyPixels(p_dst->p + A_PLANE, p_sys->p_dst_a->p);
        }
        else if (p_sys->b_add_a)
        {
            /* We inject a complete opaque alpha plane */
            if (p_fmto->i_chroma == CODEC_RGBA || p_fmto->i_chroma == CODEC_BGRA)
                FillA(&p_dst->p[0], OFFSET_A);
            else if (p_fmto->i_chroma == CODEC_ARGB || p_fmto->i_chroma == CODEC_ABGR)
                FillA(&p_dst->p[0], 0);
            else
                FillA(&p_dst->p[A_PLANE], 0);
        }

        if (p_sys->i_extend_factor != 1)
        {
            PictureCopyPixels(p_pic_dst, p_dst);
        }

        PictureCopyProperties(p_pic_dst, p_pic);
        PictureRelease(p_pic);

        return p_pic_dst;
    }
} // namespace base::media::ffmpeg::video