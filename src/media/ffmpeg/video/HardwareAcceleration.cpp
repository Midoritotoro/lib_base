#include <base/media/ffmpeg/video/HardwareAcceleration.h>
#include <qDebug>


bool InitHw(AVCodecContext* context, AVHWDeviceType type) {
	AVCodecContext* parent = static_cast<AVCodecContext*>(context->opaque);

	auto hwDeviceContext = (AVBufferRef*)nullptr;
	const auto error = av_hwdevice_ctx_create(
		&hwDeviceContext,
		type,
		nullptr,
		nullptr,
		0);

	if (error < 0 || !hwDeviceContext)
		return false;

	qDebug() << QString("Video Info: "
		"Попытка \"%1\" применить аппаратное ускорения для \"%2\"."
	).arg(
		av_hwdevice_get_type_name(type),
		context->codec->name);

	if (parent->hw_device_ctx)
		av_buffer_unref(&parent->hw_device_ctx);
	
	parent->hw_device_ctx = av_buffer_ref(hwDeviceContext);
	av_buffer_unref(&hwDeviceContext);

	context->hw_device_ctx = parent->hw_device_ctx;
	return true;
}

enum AVPixelFormat GetHwFormat(
	AVCodecContext* context,
	const enum AVPixelFormat* formats) 
{
	const auto has = [&](enum AVPixelFormat format) {
		const enum AVPixelFormat* pixelFormat = nullptr;

		for (pixelFormat = formats; *pixelFormat != AV_PIX_FMT_NONE; pixelFormat++)
			if (*pixelFormat == format)
				return true;
			
		return false;
	};

#if defined Q_OS_WIN || defined Q_OS_MAC
	const auto list = std::array{
#ifdef Q_OS_WIN
		AV_PIX_FMT_D3D11,
		AV_PIX_FMT_DXVA2_VLD,
		AV_PIX_FMT_CUDA,
#elif defined Q_OS_MAC // Q_OS_WIN
		AV_PIX_FMT_VIDEOTOOLBOX,
#endif // Q_OS_WIN || Q_OS_MAC
	};
#endif // Q_OS_WIN || Q_OS_MAC
	for (const auto format : list) {
		if (!has(format))
			continue;
		
		const auto type = [&] {
			switch (format) {
#ifdef Q_OS_WIN
			case AV_PIX_FMT_D3D11: return AV_HWDEVICE_TYPE_D3D11VA;
			case AV_PIX_FMT_DXVA2_VLD: return AV_HWDEVICE_TYPE_DXVA2;
			case AV_PIX_FMT_CUDA: return AV_HWDEVICE_TYPE_CUDA;
#elif defined Q_OS_MAC // Q_OS_WIN
			case AV_PIX_FMT_VIDEOTOOLBOX:
				return AV_HWDEVICE_TYPE_VIDEOTOOLBOX;
#endif // Q_OS_WIN || Q_OS_MAC
			}
			return AV_HWDEVICE_TYPE_NONE;
			}();
		if (type == AV_HWDEVICE_TYPE_NONE && context->hw_device_ctx)
			av_buffer_unref(&context->hw_device_ctx);
		else if (type != AV_HWDEVICE_TYPE_NONE && !InitHw(context, type))
			continue;
		return format;
	}
	enum AVPixelFormat result = AV_PIX_FMT_NONE;
	for (const enum AVPixelFormat* pixelFormat = formats; *pixelFormat != AV_PIX_FMT_NONE; pixelFormat++) {
		result = *pixelFormat;
	}
	return result;
}

template <AVPixelFormat Required>
enum AVPixelFormat GetFormatImplementation(
	AVCodecContext* ctx,
	const enum AVPixelFormat* pix_fmts) 
{
	const enum AVPixelFormat* pixelFormat = nullptr;

	for (pixelFormat = pix_fmts; *pixelFormat != -1; pixelFormat++)
		if (*pixelFormat == Required)
			return *pixelFormat;
		
	return AV_PIX_FMT_NONE;
}