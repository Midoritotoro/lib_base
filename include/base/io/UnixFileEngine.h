#pragma once

#if defined(OS_MAC) || defined(OS_LINUX)

#include <base/io/AbstractFileEngine.h>

namespace base::io {
	class UnixFileEngine final : public AbstractFileEngine {
	public:
		UnixFileEngine();
		UnixFileEngine(const std::string& path);

		UnixFileEngine(
			not_null<FILE*> file,
			bool tryToExtractPathFromDescriptor = false);

		UnixFileEngine(
			not_null<FILE*> file,
			const std::string& path);

		~UnixFileEngine() override;

		void setFileName(const std::string& path) override;
		void setFileDescriptor(not_null<FILE*> file) override;

		[[nodiscard]] bool isOpened() const noexcept override;

		[[nodiscard]] FILE* fileDescriptor() const noexcept override;
		[[nodiscard]] std::string path() const noexcept override;

		[[nodiscard]] bool exists(const std::string& path) override;

		void find(
			const base_string& path,
			std::vector<base_string>& output,
			const FileFilter& filter = {},
			bool recurse = true) override;

		void find(
			std::vector<base_string>& output,
			const FileFilter& filter = {},
			bool recurse = true) override;

		void close() override;

		[[nodiscard]] std::string absolutePathFromDescriptor(FILE* descriptor) override;

		[[nodiscard]] bool open(
			const std::string& path,
			FileOpenModes mode) override;
		[[nodiscard]] bool open(
			const std::string& path,
			const char* mode) override;

		[[nodiscard]] bool rename(const std::string& newFileName) override;
		[[nodiscard]] bool rename(
			const std::string& oldFileName,
			const std::string& newFileName) override;

		[[nodiscard]] bool rewind(sizetype position) override;
		[[nodiscard]] bool rewind(FilePositions position) override;

		[[nodiscard]] void remove() override;
		[[nodiscard]] void remove(const std::string& path) override;

		[[nodiscard]] sizetype read(
			_SAL2_Out_writes_bytes_(sizeInBytes) void* outBuffer,
			_SAL2_In_ sizetype sizeInBytes) override;

		[[nodiscard]] sizetype fileSize() const noexcept override;
	private:
		FILE* _desc = nullptr;
		std::string _path = "";
	};
} // namespace base::io
#endif