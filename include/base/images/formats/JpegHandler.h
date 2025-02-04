#pragma once 

#include <base/images/formats/AbstractFormatHandler.h>


namespace base::images {
	class PngHandler;
	class BmpHandler;

	class JpegHandler final : public AbstractFormatHandler {
	public:
		void write(
			ImageData* data,
			const char* path) override;

		void read(
			ImageData* data,
			FILE* file) override;

		void convertToFormat(
			ImageData* data,
			const char* format) override;

		virtual [[nodiscard]] const char* format() const noexcept;
	};
} // namespace base::images