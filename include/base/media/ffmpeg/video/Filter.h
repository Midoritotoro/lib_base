#pragma once 

#include <base/media/ffmpeg/video/SubPicture.h>
#include <base/media/ffmpeg/video/ColorSpace.h>


namespace base::media::ffmpeg::video {
    struct filter_t;

    struct filter_video_callbacks
    {
        picture_t* (*buffer_new)(filter_t*);
        decoder_device* (*hold_device)(object_t*, void* sys);
    };

    struct filter_audio_callbacks
    {
        struct
        {
            void (*on_changed)(filter_t*,
                const struct vlc_audio_loudness* loudness);
        } meter_loudness;
    };

    struct filter_subpicture_callbacks
    {
        subpicture_t* (*buffer_new)(filter_t*);
    };

    struct filter_owner_t
    {
        union
        {
            const filter_video_callbacks* video;
            const filter_audio_callbacks* audio;
            const filter_subpicture_callbacks* sub;
        };

        /* Input attachments
         * XXX use filter_GetInputAttachments */
        int (*pf_get_attachments)(filter_t*, input_attachment_t***, int*);

        void* sys;
    };

    struct filter_t
    {
        object_t obj;

        module_t* p_module;
        void* p_sys;

        /* Input format */
        es_format_t         fmt_in;
        struct video_context* vctx_in;  // video filter, set by owner

        /* Output format of filter */
        es_format_t         fmt_out;
        struct video_context* vctx_out; // video filter, handled by the filter
        bool                b_allow_fmt_out_change;

        /* Name of the "video filter" shortcut that is requested, can be NULL */
        const char* psz_name;
        /* Filter configuration */
        const config_chain_t* p_cfg;

        /* Implementation of filter API */
        const struct filter_operations* ops;

        /** Private structure for the owner of the filter */
        filter_owner_t      owner;
    };


    struct filter_operations
    {
        /* Operation depending on the type of filter. */
        union
        {
            /** Filter a picture (video filter) */
            picture_t* (*filter_video)(filter_t*, picture_t*);

            /** Filter an audio block (audio filter) */
            frame_t* (*filter_audio)(filter_t*, frame_t*);

            /** Blend a subpicture onto a picture (video blending) */
            void (*blend_video)(filter_t*, picture_t*, const picture_t*,
                int, int, int);

            /** Generate a subpicture (sub source) */
            subpicture_t* (*source_sub)(filter_t*, tick_t);

            /** Filter a subpicture (sub filter) */
            subpicture_t* (*filter_sub)(filter_t*, subpicture_t*);

            /** Render text (text renderer)
             *
             * \return a picture-based region or NULL
             */
            subpicture_region_t* (*render)(filter_t*,
                const subpicture_region_t*, const fourcc_t*);
        };

        union
        {
            /* TODO: video filter drain */
            /** Drain (audio filter) */
            frame_t* (*drain_audio)(filter_t*);
        };

        /** Flush
         *
         * Flush (i.e. discard) any internal buffer in a video or audio filter.
         */
        void (*flush)(filter_t*);

        /** Change viewpoint
         *
         * Pass a new viewpoint to audio filters. Filters like the spatialaudio one
         * used for Ambisonics rendering will change its output according to this
         * viewpoint.
         */
        void (*change_viewpoint)(filter_t*, const viewpoint_t*);

        /** Filter mouse state (video filter).
         *
         * If non-NULL, you must convert from output to input formats:
         * - If VLC_SUCCESS is returned, the mouse state is propagated.
         * - Otherwise, the mouse change is not propagated.
         * If NULL, the mouse state is considered unchanged and will be
         * propagated. */
        int (*video_mouse)(filter_t*, struct vlc_mouse_t*,
            const struct vlc_mouse_t* p_old);

        /** Close the filter and release its resources. */
        void (*close)(filter_t*);
    };

    typedef int (*filter_open)(filter_t*);

    [[nodiscard]] picture_t* FilterNewPicture(filter_t* p_filter);
} // namespace base::media::ffmpeg::video 