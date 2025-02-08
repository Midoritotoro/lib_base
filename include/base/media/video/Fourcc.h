#pragma once 

#include "FourccCodecs.h"
#include "Math.h"

namespace FFmpeg {
	using fourcc_t = uint32_t;

	struct fourcc_desc {
		unsigned char alias[4];
		const char* desc;
	};

	struct fourcc_mapping {
		unsigned char alias[4];
		uint32_t fourcc;
	};

	enum es_format_category_e {
		UNKNOWN_ES = 0x00,
		VIDEO_ES,
		AUDIO_ES,
		SPU_ES,
		DATA_ES,
	};

	[[nodiscard]] int FourccCmp(
		const void* key,
		const void* ent);

	[[nodiscard]] fourcc_t FourccLookup(
		fourcc_t fourcc,
		const char** dsc,
		const struct fourcc_mapping* mapv, size_t mapc,
		const struct fourcc_desc* dscv, size_t dscc);

	[[nodiscard]] fourcc_t FourccLookupVideo(
		fourcc_t fourcc,
		const char** dsc);

	[[nodiscard]] fourcc_t FourccLookupAudio(
		fourcc_t fourcc,
		const char** dsc);
	[[nodiscard]] fourcc_t FourccLookupSpu(
		fourcc_t fourcc,
		const char** dsc);

	[[nodiscard]] fourcc_t FourccLookupCat(
		fourcc_t fourcc,
		const char** dsc,
		int cat);
	[[nodiscard]] fourcc_t FourccGetCodec(int cat, fourcc_t fourcc);
} // namespace FFmpeg