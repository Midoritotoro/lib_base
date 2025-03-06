#pragma once 

#include <base/images/formats/AbstractFormatHandler.h>
#include <base/core/io/File.h>


__BASE_IMAGES_NAMESPACE_BEGIN

class BmpHandler final: public AbstractFormatHandler {
public:
	void write(
		ImageData* data,
		const char* path) override;

	void read(ImageData* data) override;

	void convertToFormat(
		ImageData* data,
		const char* format) override;

	NODISCARD const char* format() const noexcept override;
};

__BASE_IMAGES_NAMESPACE_END
