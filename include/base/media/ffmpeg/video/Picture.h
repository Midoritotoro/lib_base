#pragma once 

#include <base/core/system/Time.h>
#include <base/media/ffmpeg/video/VideoFormat.h>

#include <base/media/ffmpeg/Utility.h>
#include <base/media/ffmpeg/video/Chroma.h>


#define PICTURE_SW_SIZE_MAX                 (UINT32_C(1) << 28) /* 256MB: 8K * 8K * 4*/
#define PICTURE_PLANE_MAX					(5)

__BASE_MEDIA_FFMPEG_VIDEO_NAMESPACE_BEGIN



enum
{
	Y_PLANE = 0,
	U_PLANE = 1,
	V_PLANE = 2,
	A_PLANE = 3,
};

class FrameScaler;

class Frame {
public:
	~Frame();

	void setupFromFormat(const video_format_t* fmt);
	void newFrameFromFormat(const video_format_t* fmt);

	base_nodiscard static Frame* FrameNew(
		fourcc_t i_chroma, int i_width,
		int i_height, int i_sar_num,
		int i_sar_den);

	void clean();
private:
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

	//bool frameInitPrivate(
	//	const video_format_t* p_fmt,
	//	picture_priv_t* priv,
	//	const picture_resource_t* p_resource);

	void planeCopyPixels(
		plane_t* p_dst, 
		const plane_t* p_src);

    void frameCopyProperties(
		Frame* p_dst,
        const Frame* p_src);

    void frameCopy(
		Frame* p_dst,
        const Frame* p_src);

    void frameCopyPixels(
        Frame* p_dst,
        const Frame* p_src);
	/**
	* The properties of the picture
	*/
	
	video_format_t format;

	plane_t _planesDescription[PICTURE_PLANE_MAX];     /**< description of the planes */
	int i_planes;                /**< number of allocated planes */

	/** \name Picture management properties
		* These properties can be modified using the video output thread API,
		* but should never be written directly */
		/**@{*/
	Time::time_t date;                                  /**< display date */
	bool b_force;
	bool b_still;

	friend class FrameScaler;
};

struct filter_sys_t {
	SwsFilter* p_filter;
	int i_sws_flags;

	video_format_t fmt_in;
	video_format_t fmt_out;

	const Chroma::ChromaDescription* desc_in;
	const Chroma::ChromaDescription* desc_out;

	SwsContext* ctx;
	SwsContext* ctxA;

	Frame* p_src_a;
	Frame* p_dst_a;

	int i_extend_factor;

	Frame* p_src_e;
	Frame* p_dst_e;

	bool b_add_a;
	bool b_copy;

	bool b_swap_uvi;
	bool b_swap_uvo;
};

struct filter_t
{
	filter_sys_t* p_sys;

	/* Input format */
	es_format_t         fmt_in;
	video_context* vctx_in;  // video filter, set by owner

	/* Output format of filter */
	es_format_t         fmt_out;
	video_context* vctx_out; // video filter, handled by the filter
	bool                b_allow_fmt_out_change;
};


__BASE_MEDIA_FFMPEG_VIDEO_NAMESPACE_END
