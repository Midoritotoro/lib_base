#include <base/media/ffmpeg/video/VideoFormat.h>
#include <base/utility/Math.h>


namespace base::media::ffmpeg::video {
    void DecoderDeviceRelease(decoder_device* device)
    {
        decoder_device_priv* priv =
            container_of(device, decoder_device_priv, device);
       // if (AtomicRcDec(&priv->rc))
        {
            if (device->ops->close != NULL)
                device->ops->close(device);
            //objres_clear(OBJECT(device));
            //object_delete(OBJECT(device));
        }
    }

    void* VideoContextGetPrivate(
        video_context* vctx,
        enum video_context_type type)
    {
        if (vctx && vctx->private_type == type)
            return &vctx->_private;
        return NULL;
    }


    void VideoContextRelease(video_context* vctx)
    {
       // if (AtomicRcDec(&vctx->rc))
        {
            if (vctx->device)
                DecoderDeviceRelease(vctx->device);
            if (vctx->ops && vctx->ops->destroy)
                vctx->ops->destroy(VideoContextGetPrivate(vctx, vctx->private_type));
            free(vctx);
        }
    }


    void VideoFormatSetup(
        video_format_t* p_fmt, fourcc_t i_chroma,
        int i_width, int i_height,
        int i_visible_width, int i_visible_height,
        int i_sar_num, int i_sar_den)
    {
        p_fmt->i_chroma = FourccGetCodec(VIDEO_ES, i_chroma);
        p_fmt->i_width = i_width;
        p_fmt->i_visible_width = i_visible_width;
        p_fmt->i_height = i_height;
        p_fmt->i_visible_height = i_visible_height;
        p_fmt->i_x_offset =
            p_fmt->i_y_offset = 0;
        p_fmt->orientation = ORIENT_NORMAL;
        UnsignedReduce(&p_fmt->i_sar_num, &p_fmt->i_sar_den,
            i_sar_num, i_sar_den, 0);

    }

    void VideoFormatCopyCrop(
        video_format_t* p_dst, 
        const video_format_t* p_src)
    {
        p_dst->i_x_offset = p_src->i_x_offset;
        p_dst->i_y_offset = p_src->i_y_offset;
        p_dst->i_visible_width = p_src->i_visible_width;
        p_dst->i_visible_height = p_src->i_visible_height;
    }

    void ViewpointInit(viewpoint_t* p_vp)
    {
        p_vp->yaw = p_vp->pitch = p_vp->roll = 0.0f;
        p_vp->fov = FIELD_OF_VIEW_DEGREES_DEFAULT;
    }

    void VideoFormatInit(
        video_format_t* p_src,
        fourcc_t i_chroma)
    {
        memset(p_src, 0, sizeof(video_format_t));
        p_src->i_chroma = i_chroma;

        ViewpointInit(&p_src->pose);
    }


    void VideoFormatClean(video_format_t* p_src)
    {
        free(p_src->p_palette);
        memset(p_src, 0, sizeof(video_format_t));
    }


    bool VideoFormatIsSameChroma(
        const video_format_t* f1,
        const video_format_t* f2)
    {
        if (f1->i_chroma != f2->i_chroma)
            return false;
        return true;
    }

    bool VideoFormatIsSimilar(const video_format_t* f1,
        const video_format_t* f2)
    {
        if (!VideoFormatIsSameChroma(f1, f2))
            return false;

        if (f1->i_width != f2->i_width || f1->i_height != f2->i_height ||
            f1->i_visible_width != f2->i_visible_width ||
            f1->i_visible_height != f2->i_visible_height ||
            f1->i_x_offset != f2->i_x_offset || f1->i_y_offset != f2->i_y_offset)
            return false;

        if ((int64_t)f1->i_sar_num * f2->i_sar_den !=
            (int64_t)f2->i_sar_num * f1->i_sar_den)
            return false;

        if (f1->orientation != f2->orientation)
            return false;

        if (f1->multiview_mode != f2->multiview_mode)
            return false;

        return true;
    }

    int VideoFormatCopy(video_format_t* p_dst, const video_format_t* p_src)
    {
        memcpy(p_dst, p_src, sizeof(*p_dst));
        if (p_src->p_palette)
        {
            p_dst->p_palette = (video_palette_t*)malloc(sizeof(video_palette_t));
            if (!p_dst->p_palette)
                return ENOMEM;
            memcpy(p_dst->p_palette, p_src->p_palette, sizeof(*p_dst->p_palette));
        }
        return SUCCESS;
    }
} // namespace base::media::ffmpeg::video