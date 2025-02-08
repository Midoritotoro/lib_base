#pragma once 


#include "../../../core/Time.h"
#include "VideoFormat.h"

#define PICTURE_SW_SIZE_MAX                 (UINT32_C(1) << 28) /* 256MB: 8K * 8K * 4*/
#define PICTURE_PLANE_MAX					(5)

namespace Threads {
	struct ancillary;
	struct atomic_rc_t;
}

namespace FFmpeg {
	enum
	{
		Y_PLANE = 0,
		U_PLANE = 1,
		V_PLANE = 2,
		A_PLANE = 3,
	};

	struct video_format_t;
	struct picture_context_t
	{
		void (*destroy)(struct picture_context_t*);
		struct picture_context_t* (*copy)(struct picture_context_t*);
		struct video_context* vctx;
	};

	struct plane_t {
		uint8_t* p_pixels;                        /**< Start of the plane's data */

		/* Variables used for fast memcpy operations */
		int i_lines;           /**< Number of lines, including margins */
		int i_pitch;           /**< Number of bytes in a line, including margins */

		/** Size of a macropixel, defaults to 1 */
		int i_pixel_pitch;

		/* Variables used for pictures with margins */
		int i_visible_lines;            /**< How many visible lines are there? */
		int i_visible_pitch;            /**< How many bytes for visible pixels are there? */

	};

	struct picture_t {
		/**
		 * The properties of the picture
		 */
		struct video_format_t format;

		plane_t         p[PICTURE_PLANE_MAX];     /**< description of the planes */
		int             i_planes;                /**< number of allocated planes */

		/** \name Picture management properties
		 * These properties can be modified using the video output thread API,
		 * but should never be written directly */
		 /**@{*/
		Time::time      date;                                  /**< display date */
		bool            b_force;
		bool            b_still;
		/**@}*/

		/** \name Picture dynamic properties
		 * Those properties can be changed by the decoder
		 * @{
		 */
		bool            b_progressive;          /**< is it a progressive frame? */
		bool            b_top_field_first;             /**< which field is first */

		bool            b_multiview_left_eye; /**< left eye or right eye in multiview */

		unsigned int    i_nb_fields;                  /**< number of displayed fields */
		struct picture_context_t* context;      /**< video format-specific data pointer */
		/**@}*/

		/** Private data - the video output plugin might want to put stuff here to
		 * keep track of the picture */
		void* p_sys;

		/** Next picture in a FIFO a pictures */
		struct picture_t* p_next;

		::Threads::atomic_rc_t refs;

		picture_t(const picture_t& other) {
			format = other.format;
			for (int i = 0; i < PICTURE_PLANE_MAX; i++) {
				p[i] = other.p[i];
			}
			i_planes = other.i_planes;
			date = other.date;
			b_force = other.b_force;
			b_still = other.b_still;
			b_progressive = other.b_progressive;
			b_top_field_first = other.b_top_field_first;
			b_multiview_left_eye = other.b_multiview_left_eye;
			i_nb_fields = other.i_nb_fields;
			context = other.context;
			p_sys = other.p_sys;
			p_next = other.p_next;
			refs = refs;
		}
	};


	struct picture_priv_t
	{
		picture_t picture;
		struct
		{
			void (*destroy)(picture_t*);
			void* opaque;
		} gc;

		/** Private ancillary struct. Don't use it directly, but use it via
		 * picture_AttachAncillary() and picture_GetAncillary(). */
		Threads::ancillary** ancillaries;
	};

	struct picture_buffer_t
	{
		int fd;
		void* base;
		size_t size;
		off_t offset;
	};

	struct picture_priv_buffer_t {
		picture_priv_t   priv;
		picture_buffer_t res;
	};

	struct picture_resource_t
	{
		void* p_sys;
		void (*pf_destroy)(picture_t*);

		/* Plane resources
		 * XXX all fields MUST be set to the right value.
		 */
		struct
		{
			uint8_t* p_pixels;  /**< Start of the plane's data */
			int i_lines;        /**< Number of lines, including margins */
			int i_pitch;        /**< Number of bytes in a line, including margins */
		} p[PICTURE_PLANE_MAX];

	};

	void PictureRelease(picture_t* picture);
	void PictureDestroyContext(picture_t* p_picture);

	void PictureDestroyFromFormat(picture_t* pic);
	[[nodiscard]] void* PictureAllocate(int* fdp, size_t size);

	void PictureDestroyDummy(picture_t* p_picture);

	void PictureDestroy(picture_t* picture);
	void PictureDeallocate(
		int fd,
		void* base,
		size_t size);

	[[nodiscard]] int PictureSetup(
		picture_t* p_picture,
		const video_format_t* fmt);

	[[nodiscard]] picture_t* PictureNewFromFormat(const video_format_t* fmt);

	[[nodiscard]] bool PictureInitPrivate(
		const video_format_t* p_fmt,
		picture_priv_t* priv,
		const picture_resource_t* p_resource);

	[[nodiscard]] picture_t* PictureNew(
		fourcc_t i_chroma, int i_width,
		int i_height, int i_sar_num,
		int i_sar_den);

	void PictureCopyPixels(
		picture_t* p_dst,
		const picture_t* p_src);
	void PlaneCopyPixels(plane_t* p_dst, const plane_t* p_src);

	void PictureCopyProperties(picture_t* p_dst, const picture_t* p_src);
	void PictureCopy(picture_t* p_dst, const picture_t* p_src);
} // namespace FFmpeg