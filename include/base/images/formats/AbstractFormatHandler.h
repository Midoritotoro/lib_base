#pragma once 

#include <base/images/Images.h>
#include <base/core/utility/Algorithm.h>


__BASE_IMAGES_NAMESPACE_BEGIN

class AbstractFormatHandler {
public:
	virtual void write(
		ImageData* data,
		const char* path) = 0;

	virtual void read(ImageData* data) = 0;

	virtual void convertToFormat(
		ImageData* data, 
		const char* format) = 0;

	virtual base_nodiscard const char* format() const noexcept { return "Unknown"; }
};

__BASE_IMAGES_NAMESPACE_END
