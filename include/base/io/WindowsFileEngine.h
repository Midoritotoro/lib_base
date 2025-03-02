#pragma once 

#include <base/io/AbstractFileEngine.h>

#if defined(OS_WIN)

namespace base::io {
	class WindowsFileEngine final: public AbstractFileEngine {
		public:
			WindowsFileEngine();
			WindowsFileEngine(const std::string& path);

			WindowsFileEngine(
				not_null<FILE*> file,
				bool tryToExtractPathFromDescriptor = false);

			WindowsFileEngine(
				not_null<FILE*> file,
				const std::string& path);

			virtual ~WindowsFileEngine();

			void setFileName(const std::string& path) override;
			void setFileDescriptor(not_null<FILE*> file) override;

			[[nodiscard]] bool isOpened() const noexcept override;

			[[nodiscard]] FILE* fileDescriptor() const noexcept override;
			[[nodiscard]] std::string path() const noexcept override;

			static [[nodiscard]] bool exists(const std::string& path);

			static void find(
				const std::string& path,
				std::vector<std::string>& output,
				const FileFilter& filter = {},
				bool recurse = true);

			static void find(
				std::vector<std::string>& output,
				const FileFilter& filter = {},
				bool recurse = true);

			void close() override;

			[[nodiscard]] std::string absolutePathFromDescriptor(FILE* descriptor) override;

			[[nodiscard]] bool open(
				const std::string& path,
				FileOpenModes mode) override;
			[[nodiscard]] bool open(
				const std::string& path,
				const std::string& mode) override;

			[[nodiscard]] bool rename(const std::string& newFileName) override;
			static bool rename(
				const std::string& oldFileName,
				const std::string& newFileName);

			[[nodiscard]] bool rewind(int64 position) override;
			[[nodiscard]] bool rewind(FilePositions position) override;

			void remove() override;
			static void remove(const std::string& path);

			static [[nodiscard]] bool write(
				const std::string& path,
				void* inBuffer,
				sizetype sizeInBytes,
				const char* mode);

			[[nodiscard]] sizetype read(
				_SAL2_Out_writes_bytes_(sizeInBytes) void* outBuffer,
				_SAL2_In_ sizetype sizeInBytes) override;

			[[nodiscard]] ReadResult readAll() override;

			static [[nodiscard]] sizetype fileSize(const std::string& path);
		private:
			FILE* _desc = nullptr;
			std::string _path = "";
	};
} // namespace base::io

#endif
