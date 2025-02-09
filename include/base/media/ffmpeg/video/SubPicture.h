#pragma once 

#include <base/media/ffmpeg/video/VideoFormat.h>


namespace base::media::ffmpeg::video {
    using ssize_t = long long;
    /**
 * Video subtitle
 *
 * Any subtitle destined to be displayed by a video output thread should
 * be stored in this structure from it's creation to it's effective display.
 * Subtitle type and flags should only be modified by the output thread. Note
 * that an empty subtitle MUST have its flags set to 0.
 */
    struct subpicture_region_t
    {
        subpicture_region_t* prev;
        subpicture_region_t* next;
    };

    struct subpicture_private_t
    {
        video_format_t src;
        video_format_t dst;
    };

    struct spu_updater_ops;
    struct subpicture_updater_t
    {
        void* sys;
        const spu_updater_ops* ops;
    };

    struct subpicture_t
    {
        /** \name Channel ID */
        /**@{*/
        ssize_t         i_channel;                    /**< subpicture channel ID */
        /**@}*/

        /** \name Type and flags
           Should NOT be modified except by the vout thread */
           /**@{*/
        int64_t         i_order;                 /** an increasing unique number */
        subpicture_t* p_next;               /**< next subtitle to be displayed */
        /**@}*/

        subpicture_region_t regions;        /**< region list composing this subtitle */

        /** \name Date properties */
        /**@{*/
        tick_t      i_start;                  /**< beginning of display date */
        tick_t      i_stop;                   /**< end of display date. Will be
                                          considered invalid if set to TICK_INVALID
                                          or less than i_start. See b_ephemer */
        bool            b_ephemer;    /**< If this flag is set to true the subtitle
                                       will be displayed until the next one appears
                                       or if i_stop is reached when it is valid */
        bool            b_fade;                               /**< enable fading */
        bool            b_subtitle;      /**< subtitle with timestamps relative to
                                                                      the video */
                                                                      /**@}*/

                                                                      /** \name Display properties
                                                                       * These properties are only indicative and may be
                                                                       * changed by the video output thread, or simply ignored depending of the
                                                                       * subtitle type. */
                                                                       /**@{*/
        unsigned     i_original_picture_width;  /**< original width of the movie */
        unsigned     i_original_picture_height;/**< original height of the movie */
        int          i_alpha;                                  /**< transparency */
        /**@}*/

        subpicture_updater_t updater;

        subpicture_private_t* p_private;    /* Reserved to the core */
    };

    struct spu_updater_ops
    {
        /** Mandatory callback called after pf_validate and doing
          * the main job of creating the subpicture regions for the
          * current video_format */
        void (*update)(subpicture_t*,
            const video_format_t* prev_src, const video_format_t* p_fmt_src,
            const video_format_t* prev_dst, const video_format_t* p_fmt_dst,
            tick_t);

        /** Optional callback for subpicture private data cleanup */
        void (*destroy)(subpicture_t*);

    };
} // namespace base::media::ffmpeg::video