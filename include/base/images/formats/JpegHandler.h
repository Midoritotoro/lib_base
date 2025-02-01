#pragma once 

#include <base/images/formats/AbstractFormatHandler.h>


namespace base::images {
	class JpegHandler final : public AbstractFormatHandler {
	public:
		void write(
			ImageData* data,
			const char* path) override;

		[[nodiscard]] AbstractFormatHandler* convertToFormat(
			ImageData* data,
			const char* format) override;
	};
} // namespace base::images