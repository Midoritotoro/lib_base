#pragma once

extern "C" {
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
	#include <libavcodec/hwaccels.h>
} // extern "C"

using GetFormatMethod = enum AVPixelFormat(*)(
	struct AVCodecContext* s,
	const enum AVPixelFormat* fmt);

struct HwAccelDescriptor {
	GetFormatMethod getFormat = nullptr;
	AVPixelFormat format = AV_PIX_FMT_NONE;
};

[[nodiscard]] bool InitHw(
	AVCodecContext* context,
	AVHWDeviceType type);

[[nodiscard]] enum AVPixelFormat GetHwFormat(
	AVCodecContext* context,
	const enum AVPixelFormat* formats);

template <AVPixelFormat Required>
[[nodiscard]] enum AVPixelFormat GetFormatImplementation(
	AVCodecContext* ctx,
	const enum AVPixelFormat* pix_fmts);