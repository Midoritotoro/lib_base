#pragma once 

#include <base/system/AbstractFileEngine.h>

#if defined(OS_WIN)

namespace base::system {
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

			~WindowsFileEngine() override;

			void setFileName(const std::string& path) override;
			void setFileDescriptor(not_null<FILE*> file) override;

			[[nodiscard]] FILE* fileDescriptor() const noexcept override;
			[[nodiscard]] std::string path() const noexcept override;

			[[nodiscard]] bool exists(const std::string& path) override;
			[[nodiscard]] void find(
					const std::string& path,
					const FileFilter& filter,
					std::vector<std::string>& output,
					bool recurse = true) override;

			[[nodiscard]] std::string absolutePathFromDescriptor(FILE* descriptor) override;
		private:
			FILE* _desc = nullptr;
			std::string _path = "";
	};
} // namespace base::system

#endif
