#pragma once 

#include <base/system/Time.h>
#include <base/media/ffmpeg/video/VideoFormat.h>


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


class Frame {
public:
	~Frame();

	void setupFromFormat(const video_format_t* fmt);
	void newFrameFromFormat(const video_format_t* fmt);

	void clean();
private:
	/**
	* The properties of the picture
	*/
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

__BASE_MEDIA_FFMPEG_VIDEO_NAMESPACE_END
