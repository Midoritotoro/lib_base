#pragma once 

#include <base/images/formats/AbstractFormatHandler.h>


__BASE_IMAGES_NAMESPACE_BEGIN

class PngHandler;
class BmpHandler;

class JpegHandler final : public AbstractFormatHandler {
public:
	void write(
		ImageData* data,
		const char* path) override;

	void read(ImageData* data) override;

	void convertToFormat(
		ImageData* data,
		const char* format) override;

	virtual base_nodiscard const char* format() const noexcept;
};

__BASE_IMAGES_NAMESPACE_END
