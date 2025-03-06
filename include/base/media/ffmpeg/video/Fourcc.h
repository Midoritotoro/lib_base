#pragma once 

#include <base/media/ffmpeg/video/FourccCodecs.h>
#include <base/core/utility/Algorithm.h>

#include <base/core/BaseNamespace.h>


__BASE_MEDIA_FFMPEG_VIDEO_NAMESPACE_BEGIN

enum es_format_category_e {
	UNKNOWN_ES = 0x00,
	VIDEO_ES,
	AUDIO_ES,
	SPU_ES,
	DATA_ES,
};

NODISCARD int FourccCmp(
	const void* key,
	const void* ent);

NODISCARD fourcc_t FourccLookup(
	fourcc_t fourcc,
	const char** dsc,
	const struct fourcc_mapping* mapv, size_t mapc,
	const struct fourcc_desc* dscv, size_t dscc);

NODISCARD fourcc_t FourccLookupVideo(
	fourcc_t fourcc,
	const char** dsc);

NODISCARD fourcc_t FourccLookupAudio(
	fourcc_t fourcc,
	const char** dsc);
NODISCARD fourcc_t FourccLookupSpu(
	fourcc_t fourcc,
	const char** dsc);

NODISCARD fourcc_t FourccLookupCat(
	fourcc_t fourcc,
	const char** dsc,
	int cat);
NODISCARD fourcc_t FourccGetCodec(int cat, fourcc_t fourcc);


__BASE_MEDIA_FFMPEG_VIDEO_NAMESPACE_END
