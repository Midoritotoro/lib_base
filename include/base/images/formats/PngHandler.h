#pragma once 

#include <base/images/formats/AbstractFormatHandler.h>


namespace base::images {
	class PngHandler final : public AbstractFormatHandler {
	public:
		void write(
			ImageData* data,
			const char* path) override;

		void read(
			ImageData* data,
			const char* path) override;

		void convertToFormat(
			ImageData* data,
			const char* format) override;

		[[nodiscard]] const char* format() const noexcept override;
	};
} // namespace base::images