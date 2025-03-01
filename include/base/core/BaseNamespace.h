#pragma once 

#define __BASE_ENABLE_INLINE_NAMESPACES

#ifdef __BASE_ENABLE_INLINE_NAMESPACES
#  define INLINE_NAMESPACE inline
#else
#  define INLINE_NAMESPACE
#endif



#ifndef __BASE_THREADS_NAMESPACE_BEGIN
#define __BASE_THREADS_NAMESPACE_BEGIN namespace base {	INLINE_NAMESPACE namespace threads
#endif // __BASE_THREADS_NAMESPACE_BEGIN

#ifndef __BASE_THREADS_NAMESPACE_END
#define __BASE_THREADS_NAMESPACE_END }}
#endif // __BASE_THREADS_NAMESPACE_END



#ifndef __BASE_CORE_NAMESPACE_BEGIN 
#define __BASE_CORE_NAMESPACE_BEGIN namespace base { INLINE_NAMESPACE namespace core {
#endif // __BASE_CORE_NAMESPACE_BEGIN

#ifndef __BASE_CORE_NAMESPACE_END 
#define __BASE_CORE_NAMESPACE_END }}
#endif // __BASE_CORE_NAMESPACE_END



#ifndef __BASE_IO_NAMESPACE_BEGIN 
#define __BASE_IO_NAMESPACE_BEGIN namespace base { INLINE_NAMESPACE namespace io {
#endif // __BASE_IO_NAMESPACE_BEGIN

#ifndef __BASE_IO_NAMESPACE_END 
#define __BASE_IO_NAMESPACE_END }}
#endif // __BASE_IO_NAMESPACE_END



#ifndef __BASE_IMAGES_NAMESPACE_BEGIN 
#define __BASE_IMAGES_NAMESPACE_BEGIN namespace base { INLINE_NAMESPACE namespace images {
#endif // __BASE_IMAGES_NAMESPACE_BEGIN

#ifndef __BASE_IMAGES_NAMESPACE_END 
#define __BASE_IMAGES_NAMESPACE_END }}
#endif // __BASE_IO_NAMESPACE_END



#ifndef __BASE_SYSTEM_NAMESPACE_BEGIN 
#define __BASE_SYSTEM_NAMESPACE_BEGIN namespace base { INLINE_NAMESPACE namespace system { 
#endif // __BASE_SYSTEM_NAMESPACE_BEGIN

#ifndef __BASE_SYSTEM_NAMESPACE_END 
#define __BASE_SYSTEM_NAMESPACE_END }}
#endif // __BASE_IO_NAMESPACE_END



#ifndef __BASE_UTILITY_NAMESPACE_BEGIN 
#define __BASE_UTILITY_NAMESPACE_BEGIN	namespace base { INLINE_NAMESPACE namespace utility {
#endif // __BASE_UTILITY_NAMESPACE_BEGIN

#ifndef __BASE_UTILITY_NAMESPACE_END 
#define __BASE_UTILITY_NAMESPACE_END }}
#endif // __BASE_UTILITY_NAMESPACE_END



#ifndef __BASE_MEDIA_NAMESPACE_BEGIN 
#define __BASE_MEDIA_NAMESPACE_BEGIN namespace base { INLINE_NAMESPACE namespace media {
#endif // __BASE_MEDIA_NAMESPACE_BEGIN

#ifndef __BASE_MEDIA_NAMESPACE_END 
#define __BASE_MEDIA_NAMESPACE_END }}
#endif // __BASE_MEDIA_NAMESPACE_END



#ifndef __BASE_MEDIA_FFMPEG_NAMESPACE_BEGIN 
#define __BASE_MEDIA_FFMPEG_NAMESPACE_BEGIN	namespace base { INLINE_NAMESPACE namespace media { INLINE_NAMESPACE namespace ffmpeg {	
#endif // __BASE_MEDIA_FFMPEG_NAMESPACE_BEGIN

#ifndef __BASE_MEDIA_FFMPEG_NAMESPACE_END 
#define __BASE_MEDIA_FFMPEG_NAMESPACE_END }}}
#endif // __BASE_MEDIA_FFMPEG_NAMESPACE_END



#ifndef __BASE_MEDIA_FFMPEG_AUDIO_NAMESPACE_BEGIN 
#define __BASE_MEDIA_FFMPEG_AUDIO_NAMESPACE_BEGIN namespace base { INLINE_NAMESPACE namespace media { INLINE_NAMESPACE namespace ffmpeg { INLINE_NAMESPACE namespace audio {
#endif // __BASE_MEDIA_FFMPEG_AUDIO_NAMESPACE_BEGIN

#ifndef __BASE_MEDIA_FFMPEG_AUDIO_NAMESPACE_END 
#define __BASE_MEDIA_FFMPEG_AUDIO_NAMESPACE_END	}}}}
#endif // __BASE_MEDIA_FFMPEG_AUDIO_NAMESPACE_END



#ifndef __BASE_MEDIA_FFMPEG_VIDEO_NAMESPACE_BEGIN 
#define __BASE_MEDIA_FFMPEG_VIDEO_NAMESPACE_BEGIN namespace base { INLINE_NAMESPACE namespace media {	INLINE_NAMESPACE namespace ffmpeg {	INLINE_NAMESPACE namespace video {
#endif // __BASE_MEDIA_FFMPEG_VIDEO_NAMESPACE_BEGIN

#ifndef __BASE_MEDIA_FFMPEG_VIDEO_NAMESPACE_END
#define __BASE_MEDIA_FFMPEG_VIDEO_NAMESPACE_END	}}}}
#endif // __BASE_MEDIA_FFMPEG_VIDEO_NAMESPACE_END



#ifndef __BASE_QT_COMMON_NAMESPACE_BEGIN 
#define __BASE_QT_COMMON_NAMESPACE_BEGIN namespace base { INLINE_NAMESPACE namespace qt { INLINE_NAMESPACE namespace common {
#endif // __BASE_QT_COMMON_NAMESPACE_BEGIN

#ifndef __BASE_QT_COMMON_NAMESPACE_END
#define __BASE_QT_COMMON_NAMESPACE_END }}}
#endif // __BASE_QT_COMMON_NAMESPACE_END



#ifndef __BASE_QT_UI_NAMESPACE_BEGIN 
#define __BASE_QT_UI_NAMESPACE_BEGIN namespace base { INLINE_NAMESPACE namespace qt { INLINE_NAMESPACE namespace ui {
#endif // __BASE_QT_UI_NAMESPACE_BEGIN

#ifndef __BASE_QT_UI_NAMESPACE_END
#define __BASE_QT_UI_NAMESPACE_END }}}
#endif // __BASE_QT_UI_NAMESPACE_END



#ifndef __BASE_QT_STYLE_NAMESPACE_BEGIN 
#define __BASE_QT_STYLE_NAMESPACE_BEGIN namespace base { INLINE_NAMESPACE namespace qt { INLINE_NAMESPACE namespace style {
#endif // __BASE_QT_STYLE_NAMESPACE_BEGIN

#ifndef __BASE_QT_STYLE_NAMESPACE_END
#define __BASE_QT_STYLE_NAMESPACE_END }}}
#endif // __BASE_QT_STYLE_NAMESPACE_END
